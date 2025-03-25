#include "matrix.h"

/**
 * @brief Matrix constructor.
 *
 * @param row Number of rows
 * @param col Number of columns
 */
Matrix::Matrix(int row, int col)
{
    matrix = std::vector<std::vector<float>>(row, std::vector<float>(col, 0));
}

Matrix::Matrix(std::vector<std::vector<float>> input)
{
    matrix = input;
}

/**
 * @brief Overload the * operator to multiply two matrices
 *
 * @param other The other matrix
 * @return Matrix The result of the multiplication
 */
Matrix Matrix::operator*(const Matrix &other) const
{
    if (matrix[0].size() != other.matrix.size())
    {
        Serial.println("Error: Matrices have incompatible dimensions for multiplication.");
        return Matrix(0, 0);
    }

    int row = matrix.size();
    int col = other.matrix[0].size();
    int innerDim = matrix[0].size();

    Matrix result(row, col);

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            for (int k = 0; k < innerDim; ++k)
            {
                result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
            }
        }
    }
    return result;
}

/**
 * @brief Overload the *= operator to multiply a matrix by a scalar
 *
 * @param val The scalar value
 */
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

/**
 * @brief Overload [] to return a reference to a row
 *
 * @param row The row index
 * @return std::vector<float>& The row vector
 */
std::vector<float> &Matrix::operator[](int row)
{
    return matrix[row];
}

/**
 * @brief Overload [] to return a reference to a row
 *
 * @param row The row index
 * @return const std::vector<float>& The row vector
 */
const std::vector<float> &Matrix::operator[](int row) const
{
    return matrix[row];
}

/**
 * @brief Overload + to add two matrices
 *
 * @param other The other matrix
 * @return Matrix The result of the addition
 */
Matrix Matrix::operator+(const Matrix &other) const
{
    int rows = matrix.size();
    int cols = matrix[0].size();

    if (rows != other.matrix.size() || cols != other.matrix[0].size())
    {
        Serial.println("Error: Matrices have incompatible dimensions for addition.");
        return Matrix(0, 0);
    }

    Matrix result(rows, cols);

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            result.matrix[y][x] = matrix[y][x] + other.matrix[y][x];
        }
    }
    return result;
}

/**
 * @brief Overload - to subtract two matrices
 *
 * @param other The other matrix
 * @return Matrix The result of the subtraction
 */
Matrix Matrix::operator-(const Matrix &other) const
{
    int rows = matrix.size();
    int cols = matrix[0].size();

    if (rows != other.matrix.size() || cols != other.matrix[0].size())
    {
        Serial.println("Error: Matrices have incompatible dimensions for subtraction.");
        return Matrix(0, 0);
    }

    Matrix result(rows, cols);

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            result.matrix[y][x] = matrix[y][x] - other.matrix[y][x];
        }
    }
    return result;
}

/**
 * @brief Print function
 */
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

/**
 * @brief Set to value
 *
 * @param val The value to set
 */
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

/**
 * @brief Set the matrix to identity
 *
 * @param scale The scale of the identity matrix
 * @param size The size of the identity matrix
 * @param y The starting row
 * @param x The starting column
 */
void Matrix::set_identity(float scale, int size, int y, int x)
{
    int rows = matrix.size() - y;
    int cols = matrix[0].size() - x;

    if (size > std::min(rows, cols))
    {
        Serial.println("Error: Size too big for the space that was provided");
        return;
    }

    if (size == 0)
    {
        size = std::min(rows, cols);
    }

    set_value(0);
    for (size_t i = 0; i < size; i++)
    {
        matrix[i + y][i + x] = scale;
    }
}

/**
 * @brief Transpose the matrix
 *
 * @return Matrix The transposed matrix
 */
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

/**
 * @brief Compute the inverse of the matrix
 *
 * @return Matrix The inverse of the matrix
 */
Matrix Matrix::inverse() const
{
    int n = matrix.size();

    // Check if the matrix is square
    if (n != matrix[0].size())
    {
        Serial.println("Error: Matrix must be square to compute inverse.");
        return Matrix(0, 0); // Return original matrix as a fallback
    }

    // Create an augmented matrix [A | I]
    Matrix augmented(n, n * 2);
    for (int y = 0; y < n; ++y)
    {
        for (int x = 0; x < n; ++x)
        {
            augmented[y][x] = matrix[y][x]; // Copy original matrix
        }
        augmented[y][n + y] = 1.0; // Identity matrix on the right
    }

    // Perform row operations to transform [A | I] -> [I | A^-1]
    for (int i = 0; i < n; ++i)
    {
        // Find the pivot row
        float pivot = augmented[i][i];
        if (pivot == 0.0)
        {
            Serial.println("Error: Singular matrix (non-invertible).");
            return Matrix(0, 0); // Return original matrix as a fallback
        }

        // Normalize pivot row
        for (int j = 0; j < 2 * n; ++j)
        {
            augmented[i][j] /= pivot;
        }

        // Eliminate all other entries in column
        for (int k = 0; k < n; ++k)
        {
            if (k != i)
            {
                float factor = augmented[k][i];
                for (int j = 0; j < 2 * n; ++j)
                {
                    augmented[k][j] -= factor * augmented[i][j];
                }
            }
        }
    }

    // Extract the inverse matrix from the augmented matrix
    Matrix inverse(n, n);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            inverse[i][j] = augmented[i][n + j]; // Copy the right half
        }
    }

    return inverse;
}

/**
 * @brief Compute the QR decomposition of the matrix
 *
 * @return std::pair<Matrix, Matrix> The Q and R matrices
 */
std::pair<Matrix, Matrix> Matrix::qrDecomposition() const
{
    int rows = matrix.size();
    int cols = matrix[0].size();

    // Initialize correct sizes
    Matrix Q(rows, cols);
    Matrix R(cols, cols);

    // Perform Gram-Schmidt
    for (int j = 0; j < cols; ++j)
    {

        // Step 1: Extract j-th column of A
        std::vector<float> v(rows);
        for (int i = 0; i < rows; ++i)
        {
            v[i] = matrix[i][j];
        }

        // Step 2: Orthogonalization
        for (int k = 0; k < j; ++k)
        {
            float dot = 0;
            for (int i = 0; i < rows; ++i)
            {
                dot += Q[i][k] * matrix[i][j];
            }
            R[k][j] = dot;
            for (int i = 0; i < rows; ++i)
            {
                v[i] -= dot * Q[i][k];
            }
        }

        // Step 3: Normalize
        float norm = 0;
        for (int i = 0; i < rows; ++i)
        {
            norm += v[i] * v[i];
        }
        norm = sqrt(norm);

        if (norm == 0)
        {
            Serial.println("Error: Zero norm encountered during QR decomposition.");
            return {Matrix(0, 0), Matrix(0, 0)};
        }

        R[j][j] = norm;
        for (int i = 0; i < rows; ++i)
        {
            Q[i][j] = v[i] / norm;
        }
    }

    return {Q, R};
}