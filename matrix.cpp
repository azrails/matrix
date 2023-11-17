#include "matrix.h"

Matrix::Matrix(): Matrix(2, 2){};

Matrix::Matrix(int rows, int cols){
    rows_ = rows;
    cols_ = cols;
    matrix_ = new double*[rows_];
    for (size_t i = 0; i != rows_; ++i) matrix_[i] = new double[cols_];
    zeroes();
}

Matrix::~Matrix(){
    for (size_t i = 0; i != rows_; ++i) delete[] matrix_[i];
    delete[] matrix_;   
}

void Matrix::zeroes(){
    for (size_t i = 0; i != rows_; ++i)
        for (size_t j = 0; j != cols_; ++j)
            matrix_[i][j] = 0;
}

int main(){
    Matrix m;
    return 0;
}