#include <Arduino.h>
#include "matrix.h"
#include "leastSquare.h"

// Compute linear equations from a group of points
std::pair<Matrix, Matrix> computeEquations(const Matrix &cords, const Matrix &distances)
{
    int rows = cords.matrix.size();    // Number of anchor points
    int cols = cords.matrix[0].size(); // Number of spatial dimensions (2 for XY, 3 for XYZ)

    // Create matrix A (rows-1 x cols) for the system of equations
    Matrix A(rows - 1, cols);
    for (int i = 1; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            A[i - 1][j] = cords[i][j] - cords[0][j];
        }
    }

    // Create vector b (rows-1 x 1) for the right-hand side of the equations
    Matrix b(rows - 1, 1);
    for (int i = 1; i < rows; ++i)
    {
        b[i - 1][0] = distances[0][0] * distances[0][0] - distances[i][0] * distances[i][0];
        for (int j = 0; j < cols; j++)
        {
            b[i - 1][0] += (cords[i][j] * cords[i][j]) - (cords[0][j] * cords[0][j]);
        }
        b[i - 1][0] /= 2.0;
    }

    return std::make_pair(A, b);
}

void setup()
{
    // Print the solution
    Serial.begin(115200);
    while (!Serial)
    {
        ; // Wait for the serial port to connect. Needed for native USB
    }

    // Define the anchor points (x, y)
    Matrix cords({{0, 0},
                  {4, 0},
                  {0, 3}});

    // Define distances to the target
    Matrix distances({{5.5},
                      {2.7},
                      {4.1}});

    // Compute the linear equations
    std::pair<Matrix, Matrix> equations = computeEquations(cords, distances);
    Matrix A = equations.first;
    Matrix b = equations.second;

    // Solve the linear equations
    Matrix x = solveLeastSquares(A, b);

    Serial.print("Solution: x = ");
    Serial.print(x[0][0]);
    Serial.print(", y = ");
    Serial.println(x[1][0]);

    return;
}

void loop()
{
    // sex
}
