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
void Matrix::set_identity(float scale, int size, int y, int x)
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
        matrix[i + y][i + x] = scale;
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
Matrix Matrix::inverse() const {
    int n = matrix.size();

    // Check if the matrix is square
    if (n != matrix[0].size()) {
        Serial.println("Error: Matrix must be square to compute inverse.");
        return Matrix(0, 0); // Return original matrix as a fallback
    }

    // Create an augmented matrix [A | I]
    Matrix augmented(n, n * 2);
    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < n; ++x) {
            augmented[y][x] = matrix[y][x]; // Copy original matrix
        }
        augmented[y][n + y] = 1.0; // Identity matrix on the right
    }

    // Perform row operations to transform [A | I] -> [I | A^-1]
    for (int i = 0; i < n; ++i) {
        // Find the pivot row
        float pivot = augmented[i][i];
        if (pivot == 0.0) {
            Serial.println("Error: Singular matrix (non-invertible).");
            return Matrix(0, 0); // Return original matrix as a fallback
        }

        // Normalize pivot row
        for (int j = 0; j < 2 * n; ++j) {
            augmented[i][j] /= pivot;
        }

        // Eliminate all other entries in column
        for (int k = 0; k < n; ++k) {
            if (k != i) {
                float factor = augmented[k][i];
                for (int j = 0; j < 2 * n; ++j) {
                    augmented[k][j] -= factor * augmented[i][j];
                }
            }
        }
    }

    // Extract the inverse matrix from the augmented matrix
    Matrix inverse(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverse[i][j] = augmented[i][n + j]; // Copy the right half
        }
    }

    return inverse;
}

// QR decomposition
std::pair<Matrix, Matrix> Matrix::qrDecomposition() const {
    int rows = matrix.size();
    int cols = matrix[0].size();

    Serial.println("Starting QR decomposition...");
    
    // Initialize correct sizes
    Matrix Q(rows, cols); // Should be rows x cols
    Matrix R(cols, cols); // Should be cols x cols

    // Perform Gram-Schmidt
    for (int j = 0; j < cols; ++j) {
        Serial.print("Processing column ");
        Serial.println(j);

        // Step 1: Extract j-th column of A
        std::vector<float> v(rows);
        for (int i = 0; i < rows; ++i) {
            v[i] = matrix[i][j];  
        }
        
        Serial.print("Initial column v: ");
        for (float val : v) Serial.print(val), Serial.print(" ");
        Serial.println();

        // Step 2: Orthogonalization
        for (int k = 0; k < j; ++k) {
            float dot = 0;
            for (int i = 0; i < rows; ++i) {
                dot += Q[i][k] * matrix[i][j];
            }
            R[k][j] = dot;
            for (int i = 0; i < rows; ++i) {
                v[i] -= dot * Q[i][k];  
            }
            Serial.print("Projection on Q[:, ");
            Serial.print(k);
            Serial.print("]: ");
            Serial.println(dot);
        }

        // Step 3: Normalize
        float norm = 0;
        for (int i = 0; i < rows; ++i) {
            norm += v[i] * v[i];
        }
        norm = sqrt(norm);

        if (norm == 0) {
            Serial.println("Error: Zero norm encountered during QR decomposition.");
            return {Matrix(0, 0), Matrix(0, 0)};
        }

        R[j][j] = norm;
        for (int i = 0; i < rows; ++i) {
            Q[i][j] = v[i] / norm;
        }

        Serial.print("Normalized Q[:, ");
        Serial.print(j);
        Serial.print("]: ");
        for (int i = 0; i < rows; ++i) Serial.print(Q[i][j]), Serial.print(" ");
        Serial.println();
    }

    Serial.println("QR decomposition completed!");
    return {Q, R};
}