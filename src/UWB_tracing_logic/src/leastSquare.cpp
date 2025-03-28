#include "leastSquare.h"


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
       Serial.println("A");
       A.print();
       Serial.println("b");
       b.print();

       // Perform QR decomposition
       std::pair<Matrix, Matrix> qrResult = A.qrDecomposition();
       Matrix Q = qrResult.first;
       Matrix R = qrResult.second;

       // Compute Q^T * b
       Matrix Qtb = Q.transpose() * b;

       // Solve for x using inverse of R
       return R.inverseQR() * Qtb;
}
