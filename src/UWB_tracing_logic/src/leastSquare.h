#ifndef LEASTSQUARE_H
#define LEASTSQUARE_H

#include <vector>
#include "matrix.h"

std::tuple<Matrix, Matrix, Matrix> svd(const Matrix &A);
Matrix solveLeastSquares(const Matrix &A, const Matrix &b);

#endif // LEASTSQUARE_H