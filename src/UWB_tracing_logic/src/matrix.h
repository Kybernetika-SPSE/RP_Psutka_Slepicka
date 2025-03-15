#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <Arduino.h>
#include <vector>

class Matrix
{
public:
    std::vector<std::vector<float>> matrix;

    // Constructor
    Matrix(int col, int row);
    Matrix(std::vector<std::vector<float>> input);

    // Overload the * operator to multiply two matrices
    Matrix operator*(const Matrix &other) const;
    void operator*=(float val);

    // Overload [] to return a reference to a row
    std::vector<float> &operator[](int row);
    const std::vector<float> &operator[](int row) const;

    // Overload +
    Matrix operator+(const Matrix &other) const;

    // Overload -
    Matrix operator-(const Matrix &other) const;


    // Print function
    void print() const;

    // Set to value
    void set_value(float val);

    // Set the matrix to identity
    void set_identity(float scale = 1, int size = 0, int y = 0, int x = 0);

    // Transpose
    Matrix transpose() const;

    // Inverse
    Matrix inverse() const;
};

#endif