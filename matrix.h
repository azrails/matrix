#ifndef MATRIX_H
#define MATRIX_H
#include <cstddef>
#include <iostream>

class Matrix {
    private:

        int rows_, cols_;         
        double **matrix_;         

    public:
        Matrix();
        Matrix(int rows, int cols);
        Matrix(const Matrix& other);
        Matrix(Matrix&& other);  
        ~Matrix();

        //void SumMatrix(const Matrix& other); 
        void zeroes();
};

#endif