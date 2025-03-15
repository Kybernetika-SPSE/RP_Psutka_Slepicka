#include "leastSquare.h"

std::vector<float> solveLeastSquares(const Matrix& A, const std::vector<float>& b) {
    int rows = A.matrix.size();
    int cols = A.matrix[0].size();

    Matrix Q(rows, cols);              
    Matrix R(cols, cols);             
    std::pair<Matrix, Matrix> qr_result = A.qrDecomposition();
    Q = qr_result.first;
    R = qr_result.second;

    // Step 1: Compute Q^T * b
    std::vector<float> Qtb(cols, 0);
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            Qtb[i] += Q[j][i] * b[j];  // Q^T * b
        }
    }

    // Step 2: Solve Rx = Q^T * b using back substitution
    std::vector<float> x(cols, 0);  // Solution vector
    for (int i = cols - 1; i >= 0; --i) {
        x[i] = Qtb[i];
        for (int j = i + 1; j < cols; ++j) {
            x[i] -= R[i][j] * x[j];
        }
        x[i] /= R[i][i];  // Back substitution
    }

    return x;  // Return the least squares solution
}