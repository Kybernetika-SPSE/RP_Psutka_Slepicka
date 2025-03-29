#include "leastSquare.h"

/**
 * @brief Compute the centroid of a set of coordinates.
 *
 * @param cords Matrix of coordinates (rows x cols)
 * @return Matrix The centroid of the coordinates
 */
Matrix computeCentroid(const Matrix &cords)
{
    int rows = cords.rows();
    int cols = cords.cols();

    // Check if the matrix is empty
    if (rows == 0 || cols == 0)
    {
        Serial.println("Error computeCentroid: Empty matrix.");
        return Matrix(1, cols);
    }

    // Compute the centroid
    Matrix centroid(1, cols);
    for (int j = 0; j < cols; ++j)
    {
        float sum = 0;
        for (int i = 0; i < rows; ++i)
        {
            sum += cords[i][j];
        }
        centroid[0][j] = sum / rows;
    }

    return centroid;
}

/**
 * @brief Compute the system of equations for the least squares problem.
 *
 * @param cords Matrix of anchor point coordinates (rows x cols)
 * @param distances Matrix of distances from the anchor points (rows x 1)
 * @return std::pair<Matrix, Matrix> (A, b) where A is the matrix of coefficients and b is the right-hand side vector
 */
std::pair<Matrix, Matrix> computeEquations(const Matrix &cords, const Matrix &distances)
{
    int rows = cords.matrix.size();    // Number of anchor points
    int cols = cords.matrix[0].size(); // Number of spatial dimensions (2 for XY, 3 for XYZ)

    // Create matrix A (rows-1 x cols) for the system of equations
    Matrix A(rows - 1, cols);
    for (int i = 1; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            A[i - 1][j] = cords[i][j] - cords[0][j];
        }
    }

    // Create vector b (rows-1 x 1) for the right-hand side of the equations
    Matrix b(rows - 1, 1);
    for (int i = 1; i < rows; ++i)
    {
        b[i - 1][0] = distances[0][0] * distances[0][0] - distances[i][0] * distances[i][0];
        for (int j = 0; j < cols; j++)
        {
            b[i - 1][0] += (cords[i][j] * cords[i][j]) - (cords[0][j] * cords[0][j]);
        }
        b[i - 1][0] /= 2.0;
    }

    return std::make_pair(A, b);
}

/**
 * @brief Compute Singular Value Decomposition (SVD) using Jacobi method.
 *
 * @param A Input matrix (m x n)
 * @return std::tuple<Matrix, Matrix, Matrix> (U, Sigma, V^T)
 */
std::tuple<Matrix, Matrix, Matrix> svd(const Matrix &A)
{
    int m = A.rows();
    int n = A.cols();

    // Initialize U as A (it will be transformed)
    Matrix U = A;

    // Sigma (singular values, initially identity)
    Matrix Sigma(n, n);

    // V as identity
    Matrix V(n, n);
    V.set_identity();

    // Jacobi rotation for SVD
    for (int iter = 0; iter < 100; ++iter) // Limit iterations
    {
        bool changed = false;

        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                float aii = U.getColumn(i).norm();
                float ajj = U.getColumn(j).norm();
                float aij = (U.getColumn(i).transpose() * U.getColumn(j))[0][0];

                if (fabs(aij) > 1e-6 * sqrt(aii * ajj))
                {
                    float tau = (ajj - aii) / (2 * aij);
                    float t = (tau >= 0) ? (1 / (tau + sqrt(1 + tau * tau))) : (-1 / (-tau + sqrt(1 + tau * tau)));
                    float c = 1 / sqrt(1 + t * t);
                    float s = t * c;

                    // Apply rotation to U
                    for (int k = 0; k < m; ++k)
                    {
                        float u_ik = U[k][i];
                        float u_jk = U[k][j];

                        U[k][i] = c * u_ik - s * u_jk;
                        U[k][j] = s * u_ik + c * u_jk;
                    }

                    // Apply rotation to V
                    for (int k = 0; k < n; ++k)
                    {
                        float v_ik = V[k][i];
                        float v_jk = V[k][j];

                        V[k][i] = c * v_ik - s * v_jk;
                        V[k][j] = s * v_ik + c * v_jk;
                    }

                    changed = true;
                }
            }
        }

        if (!changed)
            break; // Converged
    }

    // Extract singular values from U columns
    for (int i = 0; i < n; ++i)
        Sigma[i][i] = U.getColumn(i).norm();

    return {U, Sigma, V.transpose()};
}

/**
 * @brief Check if the points are coplanar based on the singular values.
 *
 * @param Sigma The diagonal matrix of singular values
 * @param threshold The threshold for coplanarity
 * @return true If the points are coplanar
 * @return false If the points are not coplanar
 */
bool isCoplanar(const Matrix Sigma, float threshold)
{
    // The smallest singular value is in the last diagonal element of Sigma
    float sigmaMin = Sigma[Sigma.cols() - 1][Sigma.cols() - 1];

    // If sigmaMin is very small, points are coplanar
    return fabs(sigmaMin) < threshold;
}

/**
 * @brief Find the plane equation from the SVD results.
 *
 * @param V The matrix V from SVD
 * @param Centroid The centroid of the points
 * @return Plane The plane equation coefficients
 * @note The plane is defined as ax + by + cz + d = 0
 */
Plane findPlane(const Matrix &V, const Matrix &Centroid)
{
    Plane plane;
    plane.a = V[0][2];                                                                           // Normal x
    plane.b = V[1][2];                                                                           // Normal y
    plane.c = V[2][2];                                                                           // Normal z
    plane.d = -(plane.a * Centroid[0][0] + plane.b * Centroid[0][1] + plane.c * Centroid[0][2]); // Plane equation: ax + by + cz + d = 0

    return plane;
}

/**
 * @brief Project points onto a plane defined by the plane equation.
 *
 * @param points The points to project (rows x 3)
 * @param plane The plane equation coefficients
 * @return Matrix The projected points
 */
Matrix projectPointsOntoPlane(const Matrix &points, const Plane &plane)
{
    if (points.rows() == 0 || points.cols() != 3)
    {
        Serial.println("Error: Invalid dimensions for points.");
        return points;
    }

    float a = plane.a;
    float b = plane.b;
    float c = plane.c;
    float d = plane.d;

    Matrix projectedPoints(points.rows(), points.cols());

    for (int i = 0; i < points.rows(); ++i)
    {
        float x = points[i][0];
        float y = points[i][1];
        float z = points[i][2];

        // Calculate the distance from the point to the plane
        float lambda = (a * x + b * y + c * z + d) / (a * a + b * b + c * c);

        // Project the point onto the plane
        projectedPoints[i][0] = x - lambda * a;
        projectedPoints[i][1] = y - lambda * b;
        projectedPoints[i][2] = z - lambda * c;
    }

    return projectedPoints;
}

/**
 * @brief Convert a 3D point on a plane into 2D coordinates using plane basis vectors.
 *
 * @param point The 3D point (Matrix 1x3)
 * @param planeU First basis vector of the plane (Matrix 1x3)
 * @param planeV Second basis vector of the plane (Matrix 1x3)
 * @return Matrix The 2D coordinates (Matrix 1x2)
 */
Matrix convert3DTo2D(const Matrix &points, const Matrix &planeU, const Matrix &planeV)
{
    if (points.cols() != 3 || planeU.cols() != 3 || planeV.cols() != 3)
    {
        Serial.println("Error: Matrices must have 3 columns for 3D points.");
        return Matrix(points.rows(), 2);
    }

    Matrix result(points.rows(), 2);
    for (int i = 0; i < points.rows(); ++i)
    {
        // Project the 3D point onto the plane using the basis vectors
        result[i][0] = points[i][0] * planeU[0][0] + points[i][1] * planeU[0][1] + points[i][2] * planeU[0][2];
        result[i][1] = points[i][0] * planeV[0][0] + points[i][1] * planeV[0][1] + points[i][2] * planeV[0][2];
    }

    return result;
}

/**
 * @brief Solve the Least Squares using QR Decomposition
 *
 * @param A The matrix A
 * @param b The matrix b
 * @return Matrix The solution x
 */
Matrix solveLeastSquares(const Matrix &A, const Matrix &b)
{
    if (A.rows() == 0 || A.cols() == 0 || b.rows() == 0 || b.cols() != 1)
    {
        Serial.println("Error: Invalid dimensions for A or b.");
        return Matrix(A.cols(), 1);
    }
    if (A.rows() < A.cols())
    {
        Serial.println("Error: More variables than equations.");
        return Matrix(A.cols(), 1);
    }
    if (A.rows() != b.rows())
    {
        Serial.println("Error: Incompatible dimensions for A and b.");
        return Matrix(A.cols(), 1);
    }

    Serial.println("Performing QR decomposition...");
    // Perform QR decomposition
    std::pair<Matrix, Matrix> qrResult = A.qrDecomposition();
    Matrix Q = qrResult.first;
    Matrix R = qrResult.second;

    // Compute Q^T * b
    Matrix Qtb = Q.transpose() * b;

    // Solve for x using inverse of R
    return R.inverseQR() * Qtb;
}