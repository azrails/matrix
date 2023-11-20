#include "matrix.h"

const char* Matrix::DifferentMatrixSize::what() const noexcept {
  return mes_err.c_str();
}

const char* Matrix::NotSquare::what() const noexcept {
  return mes_err.c_str();
}

const char* Matrix::ZeroDeterminant::what() const noexcept {
  return mes_err.c_str();
}

Matrix::Matrix() : Matrix(2, 2){};

Matrix::Matrix(int rows, int cols){
  rows_ = rows;
  cols_ = cols;
  matrix_ = new double*[rows_];
  for (size_t i = 0; i != rows_; ++i) matrix_[i] = new double[cols_];
  zeroes();
}

Matrix::Matrix(const Matrix& other){
  cols_ = other.cols_;
  rows_ = other.rows_;
  matrix_ = new double*[rows_];
  for (size_t i = 0; i != rows_; ++i) {
    matrix_[i] = new double[cols_];
    for (size_t j = 0; j != cols_; ++j) matrix_[i][j] = other.matrix_[i][j];
  }
}

Matrix::Matrix(Matrix&& other) noexcept {
  cols_ = other.cols_;
  rows_ = other.rows_;
  matrix_ = other.matrix_;
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;
}

Matrix::~Matrix() {
  for (size_t i = 0; i != rows_; ++i) delete[] matrix_[i];
  delete[] matrix_;
}

void Matrix::setRows(const size_t& rows) {
  if (rows == 0) {
    for (size_t i = 0; i != rows_; ++i) delete[] matrix_[i];
    delete[] matrix_;
    matrix_ = nullptr;
    rows_ = rows;
    cols_ = 0;
  } else if (rows != rows_) {
    double** new_matrix = nullptr;
    if (rows < rows_) {
      new_matrix = new double*[rows];
      for (size_t i = 0; i != rows_; ++i) {
        if (i < rows)
          new_matrix[i] = matrix_[i];
        else
          delete[] matrix_[i];
      };
    } else if (rows > rows_) {
      new_matrix = new double*[rows];
      for (size_t i = 0; i != rows; ++i) {
        if (i < rows_)
          new_matrix[i] = matrix_[i];
        else {
          new_matrix[i] = new double[cols_];
          for (size_t j = 0; j != cols_; j++) new_matrix[i][j] = 0;
        }
      }
    }
    delete[] matrix_;
    matrix_ = new_matrix;
    rows_ = rows;
  }
}

void Matrix::setCols(const size_t& cols) {
  if (cols == 0) {
    for (size_t i = 0; i != rows_; ++i) delete[] matrix_[i];
    delete[] matrix_;
    matrix_ = nullptr;
    rows_ = 0;
    cols_ = cols;
  } else if (cols != cols_) {
    for (size_t i = 0; i != rows_; ++i) {
      double* new_column = new double[cols];
      for (size_t j = 0; j != cols; ++j) {
        if (j < cols_)
          new_column[j] = matrix_[i][j];
        else
          new_column[j] = 0;
      }
      delete[] matrix_[i];
      matrix_[i] = new_column;
    }
    cols_ = cols;
  }
}

size_t Matrix::getRows() const { return rows_; }

size_t Matrix::getCols() const { return cols_; }

bool Matrix::EqMatrix(const Matrix& other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    return false;
  }
  for (size_t i = 0; i != rows_; ++i)
    for (size_t j = 0; j != cols_; ++j)
      if (matrix_[i][j] != other.matrix_[i][j]) return false;
  return true;
}

void Matrix::SumMatrix(const Matrix& other) {
  if (rows_ != other.rows_) throw DifferentMatrixSize("rows count not equal");
  if (cols_ != other.cols_) throw DifferentMatrixSize("cols count not equal");
  for (size_t i = 0; i < rows_; ++i)
    for (size_t j = 0; j < cols_; ++j) matrix_[i][j] += other.matrix_[i][j];
}

void Matrix::SubMatrix(const Matrix& other) {
  if (rows_ != other.rows_) throw DifferentMatrixSize("rows count not equal");
  if (cols_ != other.cols_) throw DifferentMatrixSize("cols count not equal");
  for (size_t i = 0; i < rows_; ++i)
    for (size_t j = 0; j < cols_; ++j) matrix_[i][j] -= other.matrix_[i][j];
}

void Matrix::MulNumber(const double num){
  for (size_t i = 0; i < rows_; ++i)
    for (size_t j = 0; j < cols_; ++j) matrix_[i][j] *= num;
}

Matrix Matrix::Transpose() const {
  Matrix new_matrix(cols_, rows_);
  for (size_t i = 0; i < rows_; ++i)
    for (size_t j = 0; j < cols_; ++j) new_matrix.matrix_[j][i] = matrix_[i][j];
  return new_matrix;
}

void Matrix::MulMatrix(const Matrix& other){
  if (cols_ != other.rows_) throw DifferentMatrixSize("cols first op operand not equal rows second op");
  double **new_matrix = new double*[rows_];
  for (size_t i = 0; i != rows_; ++i){
    new_matrix[i] = new double[other.cols_];
    for (size_t j = 0; j != other.cols_; ++j){
      new_matrix[i][j] = 0;
      for (size_t k = 0; k != cols_; ++k)
        new_matrix[i][j] += matrix_[i][k] * other.matrix_[k][j];}
  }
  for (size_t i = 0; i != rows_; ++i) delete[] matrix_[i];
  delete[] matrix_;
  matrix_ = new_matrix;
  cols_ = other.cols_;
}

double Matrix::Determinant() const {
  if (rows_ != cols_) throw NotSquare("matrix is not square");
  double result = 1;
  double *row_ptr;
  double **new_matrix = new double*[rows_];
    for (size_t i = 0; i !=rows_; ++i){
      new_matrix[i] = new double[cols_];
      for (size_t j = 0; j != cols_; ++j) {
        new_matrix[i][j] = matrix_[i][j];
      }
    }
    double tmp = 0;
    for (size_t j = 0; j != cols_; ++j) {
      for (size_t i = j + 1; i != rows_; ++i) {
        if (new_matrix[j][j] == 0) {
          size_t non_zero_row = j;
          for (size_t k = j + 1; k < rows_; ++k) {
            if (new_matrix[k][j] != 0) {
              non_zero_row = k;
              break;
            }
          }
          if (non_zero_row != j) {
            row_ptr = new_matrix[non_zero_row];
            new_matrix[non_zero_row] = new_matrix[j];
            new_matrix[j] = row_ptr;
            result *= pow(-1, (non_zero_row - j) + (non_zero_row - j + 1));
            tmp = (-1) * (new_matrix[i][j] / new_matrix[j][j]);
          } else
            tmp = 0;
        } else
          tmp = (-1) * (new_matrix[i][j] / new_matrix[j][j]);
        for (size_t k = 0; k != cols_; k++)
          new_matrix[i][k] += tmp * new_matrix[j][k];
      }
    }
    for (size_t i = 0; i != rows_; ++i){
      result *= new_matrix[i][i];
      delete[] new_matrix[i];
    }
    delete[] new_matrix;
    return result;
}

double Matrix::minor(size_t s, size_t k) const{
  Matrix temporary(rows_ - 1, cols_ - 1);
  size_t a = 0, b = 0;
  for (size_t i = 0; i != rows_; ++i) {
    for (size_t j = 0; j != cols_; ++j) {
      if (i != s) {
        if (j != k) {
          temporary(a, b) = matrix_[i][j];
          b++;
        }
      }
    }
    if (i != s) a++;
    b = 0;
  }
  return temporary.Determinant();
}

Matrix Matrix::CalcComplements() const {
  if (rows_ != cols_) throw NotSquare("matrix is not square");
  Matrix complement_matrix(rows_, cols_);
  for (size_t i = 0; i !=  rows_; ++i)
    for (size_t j = 0; j != cols_; ++j)
      complement_matrix(i, j) = pow(-1, i + j) * minor(i, j);
  return complement_matrix;
}

Matrix Matrix::InverseMatrix() const{
  double det = Determinant();
  if (det == 0) throw ZeroDeterminant("matrix determinant is 0");
  Matrix compliment_matrix = CalcComplements().Transpose();
  Matrix inverse_matrix(rows_, cols_);
  for (size_t i = 0; i != rows_; ++i)
    for (size_t j = 0; j != cols_; ++j)
      inverse_matrix(i, j) = (1.0 / det) * compliment_matrix(i, j);
  return inverse_matrix;
}

void Matrix::zeroes() {
  for (size_t i = 0; i != rows_; ++i)
    for (size_t j = 0; j != cols_; ++j) matrix_[i][j] = 0;
}

Matrix& Matrix::operator=(const Matrix& other) {
  for (size_t i = 0; i != rows_; ++i) delete[] matrix_[i];
  delete[] matrix_;
  rows_ = other.rows_;
  cols_ = other.cols_;
  matrix_ = new double*[rows_];
  for (size_t i = 0; i != rows_; ++i) {
    matrix_[i] = new double[cols_];
    for (size_t j = 0; j != cols_; ++j) matrix_[i][j] = other.matrix_[i][j];
  }
  return *this;
}

Matrix& Matrix::operator=(Matrix&& other) {
  if (&other == this) return *this;
  for (size_t i = 0; i != rows_; ++i) delete[] matrix_[i];
  delete[] matrix_;
  cols_ = other.cols_;
  rows_ = other.rows_;
  matrix_ = other.matrix_;
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;
  return *this;
}

double& Matrix::operator()(size_t i, size_t j) {
  if (i >= rows_) throw std::out_of_range("i out greater then num rows");
  if (j >= cols_) throw std::out_of_range("j out greater then num columns");
  return matrix_[i][j];
}

const double& Matrix::operator()(size_t i, size_t j) const {
  if (i >= rows_) throw std::out_of_range("i out greater then num rows");
  if (j >= cols_) throw std::out_of_range("j out greater then num columns");
  return matrix_[i][j];
}

Matrix& Matrix::operator+=(const Matrix& other) {
  SumMatrix(other);
  return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
  SubMatrix(other);
  return *this;
}

Matrix& Matrix::operator*=(const double num){
  MulNumber(num);
  return *this;
}

Matrix& Matrix::operator*=(const Matrix& other){
  MulMatrix(other);
  return *this;
}

bool operator==(const Matrix& fst, const Matrix& snd) {
  return fst.EqMatrix(snd);
}

bool operator!=(const Matrix& fst, const Matrix& snd) { return !(fst == snd); }

Matrix operator+(const Matrix& fst, const Matrix& snd){
  Matrix new_matrix = fst;
  return new_matrix += snd;
}

Matrix operator-(const Matrix& fst, const Matrix& snd){
  Matrix new_matrix = fst;
  return new_matrix -= snd;
}

Matrix operator*(const Matrix& fst, const double num){
    Matrix new_matrix = fst;
    return new_matrix *= num;
}

Matrix operator*(const double num, const Matrix& fst){
    Matrix new_matrix = fst;
    return new_matrix *= num;
}

Matrix operator*(const Matrix& fst, const Matrix& snd){
  Matrix new_matrix = fst;
  return new_matrix *= snd;
}