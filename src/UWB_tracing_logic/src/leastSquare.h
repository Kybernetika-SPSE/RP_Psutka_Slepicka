#ifndef LEASTSQUARE_H
#define LEASTSQUARE_H

#include <vector>
#include "matrix.h"

// Solve the Least Squares using QR Decomposition
std::vector<float> solveLeastSquares(const Matrix& A, const std::vector<float>& b);

#endif