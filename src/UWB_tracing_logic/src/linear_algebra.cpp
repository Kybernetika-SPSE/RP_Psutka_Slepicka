#include "linear_algebra.h"

Matrix::Matrix(int col, int row)
{
    matrix = std::vector<std::vector<float>>(row, std::vector<float>(col, 0));
};

Matrix::Matrix(std::vector<std::vector<float>> input)
{
    matrix = input;
};

// Overload the * operator to multiply two matrices
Matrix Matrix::operator*(const Matrix &other) const
{
    if (matrix[0].size() != other.matrix.size())
    {
        Serial.println("Error: Matrices have incompatible dimensions for multiplication.");
        return Matrix(0, 0);
    }

    int row = matrix.size();
    int col = other.matrix[0].size();

    Matrix result(col, row);

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            for (int k = 0; k < matrix[0].size(); ++k)
            {
                result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
            }
        }
    }
    return result;
}

// Print the matrix
void Matrix::print() const
{
    for (size_t i = 0; i < matrix.size(); ++i)
    {
        for (size_t j = 0; j < matrix[i].size(); ++j)
        {
            Serial.print(matrix[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
}
