#ifndef LEASTSQUARE_H
#define LEASTSQUARE_H

#include <vector>
#include "matrix.h"

Matrix computeCentroid(const Matrix &cords);
std::pair<Matrix, Matrix> computeEquations(const Matrix &cords, const Matrix &distances);

std::tuple<Matrix, Matrix, Matrix> svd(const Matrix &A);
bool isCoplanar(const Matrix Sigma, float threshold = 1e-5);

Matrix solveLeastSquares(const Matrix &A, const Matrix &b);

#endif // LEASTSQUARE_H