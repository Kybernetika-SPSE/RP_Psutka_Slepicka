#include "leastSquare.h"

/**
 * @brief Solve the Least Squares using QR Decomposition
 *
 * @param A The matrix A
 * @param b The matrix b
 * @return Matrix The solution x
 */
Matrix solveLeastSquares(const Matrix &A, const Matrix &b)
{
    int rows = A.matrix.size();
    int cols = A.matrix[0].size();

    // Perform QR decomposition
    std::pair<Matrix, Matrix> qrResult = A.qrDecomposition();
    Matrix Q = qrResult.first;
    Matrix R = qrResult.second;

    // Compute Q^T * b
    Matrix Qt = Q.transpose();
    Matrix Qtb = Qt * b;

    // Initialize the solution vector x
    Matrix x(cols, 1);

    // Back-substitution to solve R * x = Q^T * b
    for (int i = cols - 1; i >= 0; --i)
    {
        if (R.matrix[i][i] == 0.0)
        {
            Serial.println("Error: Singular matrix in upper triangular solve.");
            return Matrix(0, 0);
        }

        float sum = 0.0;
        for (int j = i + 1; j < cols; ++j)
        {
            sum += R.matrix[i][j] * x[j][0];
        }
        x[i][0] = (Qtb.matrix[i][0] - sum) / R.matrix[i][i];
    }

    return x;
}