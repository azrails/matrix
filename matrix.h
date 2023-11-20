#ifndef MATRIX_H
#define MATRIX_H
#include <string>

class Matrix {
 private:
  size_t rows_, cols_;
  double** matrix_;

 protected:
  // Protected functions may be need in inheritance
  void zeroes();
  double minor(size_t s, size_t k) const;

 public:
  // Exceptions
  class DifferentMatrixSize : public std::exception {
   private:
    std::string mes_err;

   public:
    DifferentMatrixSize(std::string err) : mes_err(err){};
    const char* what() const noexcept;
  };

  class NotSquare : public std::exception {
   private:
    std::string mes_err;

   public:
    NotSquare(std::string err) : mes_err(err){};
    const char* what() const noexcept;
  };

  class ZeroDeterminant : public std::exception {
   private:
    std::string mes_err;

   public:
    ZeroDeterminant(std::string err) : mes_err(err){};
    const char* what() const noexcept;
  };
  // Constructors and destructor
  Matrix();
  Matrix(int rows, int cols);
  Matrix(const Matrix& other);
  Matrix(Matrix&& other) noexcept;
  ~Matrix();

  // Overloading operators
  Matrix& operator+=(const Matrix& other);
  Matrix& operator-=(const Matrix& other);
  Matrix& operator*=(const double num);
  Matrix& operator*=(const Matrix& other);
  Matrix& operator=(const Matrix& other);
  Matrix& operator=(Matrix&& other);
  double& operator()(size_t i, size_t j);
  const double& operator()(size_t i, size_t j) const;

  // accessors & mutators
  size_t getRows() const;
  size_t getCols() const;
  void setRows(const size_t& rows);
  void setCols(const size_t& cols);

  // Member functions
  void SumMatrix(const Matrix& other);
  void SubMatrix(const Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const Matrix& other);
  bool EqMatrix(const Matrix& other) const;
  Matrix Transpose() const;
  double Determinant() const;
  Matrix CalcComplements() const;
  Matrix InverseMatrix() const;
};

// Function overloading operators
bool operator==(const Matrix& fst, const Matrix& snd);
bool operator!=(const Matrix& fst, const Matrix& snd);
Matrix operator+(const Matrix& fst, const Matrix& snd);
Matrix operator-(const Matrix& fst, const Matrix& snd);
Matrix operator*(const Matrix& fst, const double num);
Matrix operator*(const double num, const Matrix& fst);
Matrix operator*(const Matrix& fst, const Matrix& snd);
#endif