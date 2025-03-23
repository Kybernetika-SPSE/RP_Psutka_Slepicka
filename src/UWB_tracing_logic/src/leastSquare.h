#ifndef LEASTSQUARE_H
#define LEASTSQUARE_H

#include <vector>
#include "matrix.h"

// Solve the Least Squares using QR Decomposition
Matrix solveLeastSquares(const Matrix& A, const Matrix& b);

#endif