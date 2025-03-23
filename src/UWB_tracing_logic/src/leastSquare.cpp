#include "leastSquare.h"

Matrix solveLeastSquares(const Matrix &A, const Matrix &b)
{
    Serial.println("Step 1: Printing matrix A:");
    A.print();

    Serial.println("Step 2: Printing vector b:");
    b.print();

    Serial.println("====================QR==========================");

    int rows = A.matrix.size();
    int cols = A.matrix[0].size();

    Matrix Q(rows, cols);
    Matrix R(cols, cols);
    std::pair<Matrix, Matrix> qr_result = A.qrDecomposition();
    Q = qr_result.first;
    R = qr_result.second;

    Serial.println("====================QR==========================");

    Serial.println("Step 3: Q matrix:");
    Q.print();

    Serial.println("Step 4: R matrix:");
    R.print();

    Matrix Qt = Q.transpose();

    Serial.println("Step 5: Q^T:");
    Qt.print();

    Matrix Qtb = Qt * b;
    Serial.println("Step 6: Q^T * b:");
    Qtb.print();

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

    Serial.println("Step 7: Solution x:");
    x.print();
    return x;
}