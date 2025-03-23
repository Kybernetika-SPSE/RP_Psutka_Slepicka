#include "leastSquare.h"

Matrix solveLeastSquares(const Matrix &A, const Matrix &b)
{
    int rows = A.matrix.size();
    int cols = A.matrix[0].size();

    Matrix Q(rows, cols);
    Matrix R(cols, cols);
    std::pair<Matrix, Matrix> qr_result = A.qrDecomposition();
    Q = qr_result.first;
    R = qr_result.second;

    Matrix Qt = Q.transpose();

    Matrix Qtb = Qt * b;

    Matrix x(Qtb.matrix.size(), 1);

    // Back-substitution
    for (int i = Qtb.matrix.size() - 1; i >= 0; --i)
    {
        if (R.matrix[i][i] == 0.0)
        {
            Serial.println("Error: Singular matrix in upper triangular solve.");
            return Matrix(0, 0);
        }

        float sum = 0.0;
        for (int j = i + 1; j < Qtb.matrix.size(); ++j)
        {
            sum += R.matrix[i][j] * x[j][0];
        }
        x[i][0] = (Qtb.matrix[i][0] - sum) / R.matrix[i][i];
    }

    return x;
}