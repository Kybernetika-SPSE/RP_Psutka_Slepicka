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

/**
 * @brief Matrix constructor with initial values.
 *
 * @param input Initial values for the matrix
 */
Matrix::Matrix(std::vector<std::vector<float>> input)
{
    matrix = input;
}

/**
 * @brief Get the number of rows in the matrix
 *
 * @return int The number of rows
 */
int Matrix::rows() const
{
    return matrix.size();
}

/**
 * @brief Get the number of columns in the matrix
 *
 * @return int The number of columns
 */
int Matrix::cols() const
{
    return matrix[0].size();
}

/**
 * @brief Overload the * operator to multiply two matrices
 *
 * @param other The other matrix
 * @return Matrix The result of the multiplication
 */
Matrix Matrix::operator*(const Matrix &other) const
{
    if (cols() != other.rows())
    {
        Serial.println("Error: Matrices have incompatible dimensions for multiplication.");
        return Matrix(0, 0);
    }

    Matrix result(rows(), other.cols());

    for (int i = 0; i < rows(); ++i)
    {
        for (int j = 0; j < other.cols(); ++j)
        {
            for (int k = 0; k < cols(); ++k)
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
    for (size_t y = 0; y < rows(); y++)
    {
        for (size_t x = 0; x < cols(); x++)
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
    if (rows() != other.rows() || cols() != other.cols())
    {
        Serial.println("Error: Matrices have incompatible dimensions for addition.");
        return Matrix(0, 0);
    }

    Matrix result(rows(), cols());

    for (int y = 0; y < rows(); ++y)
    {
        for (int x = 0; x < cols(); ++x)
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
    if (rows() != other.rows() || cols() != other.cols())
    {
        Serial.println("Error: Matrices have incompatible dimensions for subtraction.");
        return Matrix(0, 0);
    }

    Matrix result(rows(), cols());

    for (int y = 0; y < rows(); ++y)
    {
        for (int x = 0; x < cols(); ++x)
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
    for (size_t i = 0; i < rows(); ++i)
    {
        for (size_t j = 0; j < cols(); ++j)
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
    for (size_t y = 0; y < rows(); y++)
    {
        for (size_t x = 0; x < cols(); x++)
        {
            matrix[y][x] = val;
        }
    }
}

/**
 * @brief Get a column from the matrix
 *
 * @param colIndex The index of the column to get
 * @return Matrix The column vector
 */
Matrix Matrix::getColumn(int colIndex) const
{
    Matrix columnVector(rows(), 1); // Create a column vector (rows x 1)

    for (int i = 0; i < rows(); ++i)
    {
        columnVector[i][0] = this->matrix[i][colIndex]; // Copy column values
    }

    return columnVector;
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
    if (size > std::min(rows() - y, cols() - x))
    {
        Serial.println("Error: Size too big for the space that was provided");
        return;
    }

    if (size == 0)
    {
        size = std::min(rows() - y, cols() - x);
    }

    set_value(0);
    for (size_t i = 0; i < size; i++)
    {
        matrix[i + y][i + x] = scale;
    }
}

/**
 * @brief Compute the Euclidean norm of the matrix
 *
 * @return float The Euclidean norm
 */
float Matrix::norm() const
{
    float sum = 0.0;

    for (const auto &row : matrix)
    {
        for (float val : row)
        {
            sum += val * val; // Square each element
        }
    }

    return sqrt(sum); // Take the square root
}


/**
 * @brief Transpose the matrix
 *
 * @return Matrix The transposed matrix
 */
Matrix Matrix::transpose() const
{
    Matrix transposed(cols(), rows());

    for (size_t y = 0; y < cols(); y++)
    {
        for (size_t x = 0; x < rows(); x++)
        {
            transposed[y][x] = matrix[x][y];
        }
    }
    return transposed;
}

/**
 * @brief Compute the inverse of the matrix using Gauss-Jordan elimination
 *
 * @return Matrix The inverse of the matrix
 */
Matrix Matrix::gaussJordanInverse() const
{
    int n = rows();

    // Check if the matrix is square
    if (n != cols())
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
    // Initialize correct sizes
    Matrix Q(rows(), cols());
    Matrix R(cols(), cols());

    // Perform Gram-Schmidt
    for (int j = 0; j < cols(); ++j)
    {

        // Step 1: Extract j-th column of A
        Matrix v = getColumn(j);

        // Step 2: Orthogonalization
        for (int k = 0; k < j; ++k)
        {
            float dot = 0;
            for (int i = 0; i < rows(); ++i)
            {
                dot += Q[i][k] * matrix[i][j];
            }
            R[k][j] = dot;
            for (int i = 0; i < rows(); ++i)
            {
                v[0][i] -= dot * Q[i][k];
            }
        }

        // Step 3: Normalize
        float norm = 0;
        for (int i = 0; i < rows(); ++i)
        {
            norm += v[0][i] * v[0][i];
        }
        norm = sqrt(norm);

        if (norm == 0)
        {
            Serial.println("Error: Zero norm encountered during QR decomposition.");
            return {Matrix(0, 0), Matrix(0, 0)};
        }

        R[j][j] = norm;
        for (int i = 0; i < rows(); ++i)
        {
            Q[i][j] = v[0][i] / norm;
        }
    }

    return {Q, R};
}

/**
 * @brief Compute the inverse of the matrix using QR decomposition
 *
 * @return Matrix The inverse of the matrix
 */
Matrix Matrix::inverseQR() const
{
    if (rows() != cols())
    {
        Serial.println("Error: Matrix must be square to compute inverse.");
        return Matrix(0, 0);
    }

    // Perform QR decomposition
    std::pair<Matrix, Matrix> qrResult = qrDecomposition();
    Matrix Q = qrResult.first;
    Matrix R = qrResult.second;

    // Initialize inverse matrix
    Matrix inverse(rows(), cols());

    // Solve R * X = Q^T * I (column-wise)
    Matrix Qt = Q.transpose();
    for (int j = 0; j < cols(); ++j)
    {
        Matrix e(rows(), 1);
        e[j][0] = 1.0; // Column of identity matrix

        Matrix b = Qt * e; // Compute Q^T * e_j

        // Back-substitution to solve R * x = b
        for (int i = cols() - 1; i >= 0; --i)
        {
            if (R.matrix[i][i] == 0.0)
            {
                Serial.println("Error: Singular matrix in upper triangular solve.");
                return Matrix(0, 0);
            }

            float sum = 0.0;
            for (int k = i + 1; k < cols(); ++k)
            {
                sum += R.matrix[i][k] * inverse[k][j];
            }
            inverse[i][j] = (b[i][0] - sum) / R.matrix[i][i];
        }
    }

    return inverse;
}
