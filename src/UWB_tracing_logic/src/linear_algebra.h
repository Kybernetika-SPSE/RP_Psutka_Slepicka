#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <Arduino.h>
#include <vector>


class Matrix {
    public:
        std::vector<std::vector<float>> matrix;

        // Constructor
        Matrix(int row, int col);
        Matrix(std::vector<std::vector<float>> input);
    
        // Overload the * operator to multiply two matrices
        Matrix operator*(const Matrix& other) const;
    
        // Print function
        void print() const;
    };

#endif