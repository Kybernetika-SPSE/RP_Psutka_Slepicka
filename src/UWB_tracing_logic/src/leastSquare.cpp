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
       // Perform QR decomposition
       std::pair<Matrix, Matrix> qrResult = A.qrDecomposition();
       Matrix Q = qrResult.first;
       Matrix R = qrResult.second;
   
       // Compute Q^T * b
       Matrix Qtb = Q.transpose() * b;
   
       // Solve for x using inverse of R
       return R.inverseQR() * Qtb;
}