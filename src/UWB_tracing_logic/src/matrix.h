#ifndef MATRIX_H
#define MATRIX_H

#include <Arduino.h>
#include <vector>

class Matrix
{
public:
    std::vector<std::vector<float>> matrix;
    Matrix(int row, int col);
    Matrix(std::vector<std::vector<float>> input);
    int rows() const;
    int cols() const;
    Matrix operator*(const Matrix &other) const;
    void operator*=(float val);
    std::vector<float> &operator[](int row);
    const std::vector<float> &operator[](int row) const;
    Matrix operator+(const Matrix &other) const;
    Matrix operator-(const Matrix &other) const;
    void print() const;
    void set_value(float val);
    Matrix getColumn(int colIndex) const;
    void set_identity(float scale = 1, int size = 0, int y = 0, int x = 0);
    float norm() const;
    Matrix transpose() const;
    Matrix gaussJordanInverse() const;
    std::pair<Matrix, Matrix> qrDecomposition() const;
    Matrix inverseQR() const;
};

#endif // MATRIX_H