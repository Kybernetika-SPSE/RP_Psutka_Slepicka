#include "matrix.h"

Matrix::Matrix(int row, int col)
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

void Matrix::operator*=(float val)
{
    for (size_t y = 0; y < matrix.size(); y++)
    {
        for (size_t x = 0; x < matrix[0].size(); x++)
        {
            matrix[y][x] *= val;
        }
    }
}

// Ovreloading []
std::vector<float> &Matrix::operator[](int row)
{
    return matrix[row];
}

const std::vector<float> &Matrix::operator[](int row) const
{
    return matrix[row];
}

// Overload +
Matrix Matrix::operator+(const Matrix &other) const
{
    int rows = matrix.size();
    int cols = matrix[0].size();

    if (rows != other.matrix.size() or cols != other.matrix[0].size())
    {
        Serial.println("Error: Matrices have incompatible dimensions for addition.");
        return Matrix(0, 0);
    }
    

    Matrix result(cols, rows);

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            result.matrix[y][x] = matrix[y][x] + other.matrix[y][x];
        }
    }
    return result;
}

// Overload -
Matrix Matrix::operator-(const Matrix &other) const
{
    int rows = matrix.size();
    int cols = matrix[0].size();

    if (rows != other.matrix.size() or cols != other.matrix[0].size())
    {
        Serial.println("Error: Matrices have incompatible dimensions for addition.");
        return Matrix(0, 0);
    }

    Matrix result(cols, rows);

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            result.matrix[y][x] = matrix[y][x] - other.matrix[y][x];
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

// Set to value
void Matrix::set_value(float val)
{
    for (size_t y = 0; y < matrix.size(); y++)
    {
        for (size_t x = 0; x < matrix[0].size(); x++)
        {
            matrix[y][x] = val;
        }
    }
}

// Set the matrix to identity
void Matrix::set_identity(int size, int y, int x)
{
    int rows = matrix.size() - y;
    int cols = matrix[0].size() - x;

    if (size > min(rows, cols))
    {
        Serial.println("Error: Size to big for the space that was provided");
        return;
    }

    if (size == 0)
    {
        size = min(rows, cols);
    }

    set_value(0);
    for (size_t i = 0; i < size; i++)
    {
        matrix[i + y][i + x] = 1;
    }
}

// Transpose
Matrix Matrix::transpose() const
{
    int rows = matrix.size();
    int cols = matrix[0].size();

    Matrix transposed(cols, rows);

    for (size_t y = 0; y < cols; y++)
    {
        for (size_t x = 0; x < rows; x++)
        {
            transposed[y][x] = matrix[x][y];
        }
    }
    return transposed;
}

// Inverse
Matrix Matrix::inverse() const
{
    int rows = matrix.size();
    int cols = matrix[0].size();



    Matrix result(cols, rows);

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            result.matrix[y][x] = matrix[y][x] - other.matrix[y][x];
        }
    }
    return result;
}