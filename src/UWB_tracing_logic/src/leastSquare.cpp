#include "leastSquare.h"

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