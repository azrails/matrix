#include <gtest/gtest.h>

#include <stdexcept>
#include <utility>

#include "matrix.h"

namespace testing {
AssertionResult AssertionSuccess();
AssertionResult AssertionFailure();
}  // namespace testing

template <size_t R, size_t C>
testing::AssertionResult MatrixIsEqual(const Matrix& cls,
                                       double (&matrix)[R][C]) {
  if (cls.getRows() != R)
    return testing::AssertionFailure()
           << "Class rows_: " << cls.getRows() << "!=" << R;
  if (cls.getCols() != C)
    return testing::AssertionFailure()
           << "Class cols_: " << cls.getCols() << "!=" << C;

  for (size_t i = 0; i != R; ++i)
    for (size_t j = 0; j != C; ++j)
      if (cls(i, j) != matrix[i][j])
        return testing::AssertionFailure()
               << "Matrix element(" << i << "," << j << ") = " << cls(i, j)
               << "!=" << matrix[i][j];
  return testing::AssertionSuccess();
}

TEST(MatrixConstructorTest, TestBaseConstructor) {
  Matrix matrix;
  double m[2][2] = {{0, 0}, {0, 0}};
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 2);
}

TEST(MatrixConstructorTest, TestParametrizeConstructor) {
  Matrix matrix(4, 4);
  double m[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  EXPECT_EQ(matrix.getRows(), 4);
  EXPECT_EQ(matrix.getCols(), 4);
}

TEST(MatrixConstructorTest, TestCopyConstructor) {
  Matrix matrix(4, 4);
  double m[4][4] = {{0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}};
  for (size_t i = 0; i != 4; ++i)
    for (size_t j = 0; j != 4; ++j) matrix(i, j) = j;
  Matrix matrix_2 = matrix;
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  EXPECT_TRUE(MatrixIsEqual(matrix_2, m));
  EXPECT_EQ(matrix.getRows(), 4);
  EXPECT_EQ(matrix.getCols(), 4);
  EXPECT_EQ(matrix_2.getRows(), 4);
  EXPECT_EQ(matrix_2.getCols(), 4);
}

TEST(MatrixConstructorTest, TestMoveConstructor) {
  Matrix matrix(4, 4);
  double m[4][4] = {{0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}};
  for (size_t i = 0; i != 4; ++i)
    for (size_t j = 0; j != 4; ++j) matrix(i, j) = j;
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  Matrix matrix_2 = std::move(matrix);
  EXPECT_EQ(matrix.getRows(), 0);
  EXPECT_EQ(matrix.getCols(), 0);
  EXPECT_TRUE(MatrixIsEqual(matrix_2, m));
  EXPECT_EQ(matrix_2.getRows(), 4);
  EXPECT_EQ(matrix_2.getCols(), 4);
}

TEST(MatrixOperatorTest, TestIndexOperator) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  EXPECT_EQ(matrix(0, 0), 0);
  EXPECT_EQ(matrix(0, 1), 1);
  EXPECT_EQ(matrix(1, 0), 1);
  EXPECT_EQ(matrix(1, 1), 2);
}

TEST(MatrixOperatorTest, TestIndexOperatorOutRange) {
  Matrix matrix;
  EXPECT_THROW(matrix(5, 1), std::out_of_range);
  EXPECT_THROW(matrix(0, 2), std::out_of_range);
  try {
    matrix(5, 0);
  } catch (const std::out_of_range& ex) {
    EXPECT_STREQ("i out greater then num rows", ex.what());
  }
  try {
    matrix(0, 5);
  } catch (const std::out_of_range& ex) {
    EXPECT_STREQ("j out greater then num columns", ex.what());
  }
}

TEST(MatrixOperatorTest, TestIndexOperatorConstOverload) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  const Matrix& matrix_link = matrix;
  EXPECT_EQ(matrix_link(0, 0), 0);
  EXPECT_EQ(matrix_link(0, 1), 1);
  EXPECT_EQ(matrix_link(1, 0), 1);
  EXPECT_EQ(matrix_link(1, 1), 2);
}

TEST(MatrixOperatorTest, TestIndexOperatorConstOverloadOutBound) {
  const Matrix matrix;
  EXPECT_THROW(matrix(5, 1), std::out_of_range);
  EXPECT_THROW(matrix(0, 2), std::out_of_range);
  try {
    matrix(5, 0);
  } catch (const std::out_of_range& ex) {
    EXPECT_STREQ("i out greater then num rows", ex.what());
  }
  try {
    matrix(0, 5);
  } catch (const std::out_of_range& ex) {
    EXPECT_STREQ("j out greater then num columns", ex.what());
  }
}

TEST(MatrixOperatorTest, TestTreatmentOperator) {
  Matrix matrix(4, 4);
  double m[4][4] = {{0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}};
  for (size_t i = 0; i != 4; ++i)
    for (size_t j = 0; j != 4; ++j) matrix(i, j) = j;
  Matrix matrix_2;
  matrix_2 = matrix;
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  EXPECT_TRUE(MatrixIsEqual(matrix_2, m));
  EXPECT_EQ(matrix.getRows(), 4);
  EXPECT_EQ(matrix.getCols(), 4);
  EXPECT_EQ(matrix_2.getRows(), 4);
  EXPECT_EQ(matrix_2.getCols(), 4);
}

TEST(MatrixOperatorTest, TestTreatmentMoveOperator) {
  Matrix matrix(4, 4);
  double m[4][4] = {{0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}};
  for (size_t i = 0; i != 4; ++i)
    for (size_t j = 0; j != 4; ++j) matrix(i, j) = j;
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  Matrix matrix_2;
  matrix_2 = std::move(matrix);
  EXPECT_EQ(matrix.getRows(), 0);
  EXPECT_EQ(matrix.getCols(), 0);
  EXPECT_TRUE(MatrixIsEqual(matrix_2, m));
  EXPECT_EQ(matrix_2.getRows(), 4);
  EXPECT_EQ(matrix_2.getCols(), 4);
}

TEST(MatrixOperatorTest, TestIncSum) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  Matrix matrix_2 = matrix;
  double m[2][2] = {{0, 2}, {2, 4}};
  EXPECT_TRUE(MatrixIsEqual(matrix += matrix_2, m));
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 2);
  EXPECT_EQ(matrix_2.getRows(), 2);
  EXPECT_EQ(matrix_2.getCols(), 2);
}

TEST(MatrixOperatorTest, TestIncSumDifferentSize) {
  Matrix matrix;
  Matrix matrix_2(2, 4);
  Matrix matrix_3(4, 2);
  EXPECT_THROW(matrix += matrix_2, Matrix::DifferentMatrixSize);
  EXPECT_THROW(matrix += matrix_3, Matrix::DifferentMatrixSize);
  try {
    matrix += matrix_2;
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("cols count not equal", ex.what());
  }
  try {
    matrix += matrix_3;
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("rows count not equal", ex.what());
  }
}

TEST(MatrixOperatorTest, TestDecSum) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  Matrix matrix_2 = matrix;
  double m[2][2] = {{0, 0}, {0, 0}};
  EXPECT_TRUE(MatrixIsEqual(matrix -= matrix_2, m));
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 2);
  EXPECT_EQ(matrix_2.getRows(), 2);
  EXPECT_EQ(matrix_2.getCols(), 2);
}

TEST(MatrixOperatorTest, TestDecSumDifferentSize) {
  Matrix matrix;
  Matrix matrix_2(2, 4);
  Matrix matrix_3(4, 2);
  EXPECT_THROW(matrix -= matrix_2, Matrix::DifferentMatrixSize);
  EXPECT_THROW(matrix -= matrix_3, Matrix::DifferentMatrixSize);
  try {
    matrix -= matrix_2;
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("cols count not equal", ex.what());
  }
  try {
    matrix -= matrix_3;
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("rows count not equal", ex.what());
  }
}

TEST(MatrixOperatorTest, TestMulNumOverload) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  double m[2][2] = {{0, 2.2}, {2.2, 4.4}};
  matrix *= 2.2;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) EXPECT_NEAR(matrix(i, j), m[i][j], 10e-6);
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 2);
}

TEST(MatrixOperatorTest, TestMulMatrixOverload) {
  Matrix matrix(2, 3);
  Matrix matrix_2(3, 2);
  for (int i = 0; i != 2; ++i) {
    for (int j = 0; j != 3; ++j) {
      matrix_2(j, i) = i - j;
      matrix(i, j) = i + j;
    }
  }
  double m_mul[2][2] = {{-5, -2}, {-8, -2}};
  double m_2[3][2] = {{0, 1}, {-1, 0}, {-2, -1}};
  matrix *= matrix_2;
  EXPECT_TRUE(MatrixIsEqual(matrix, m_mul));
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 2);
  EXPECT_TRUE(MatrixIsEqual(matrix_2, m_2));
  EXPECT_EQ(matrix_2.getRows(), 3);
  EXPECT_EQ(matrix_2.getCols(), 2);
}

TEST(MatrixOperatorTest, TestMulMatrixOverloadDifferentSize) {
  Matrix matrix;
  Matrix matrix_2(4, 3);
  EXPECT_THROW(matrix *= matrix_2, Matrix::DifferentMatrixSize);
  try {
    matrix *= matrix_2;
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("cols first op operand not equal rows second op", ex.what());
  }
}

TEST(MatrixPropertyTest, TestAccessors) {
  Matrix matrix;
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 2);

  Matrix matrix_2(10, 10);
  EXPECT_EQ(matrix_2.getRows(), 10);
  EXPECT_EQ(matrix_2.getCols(), 10);

  const Matrix matrix_const;
  EXPECT_EQ(matrix_const.getRows(), 2);
  EXPECT_EQ(matrix_const.getCols(), 2);

  const Matrix matrix_const_2(10, 10);
  EXPECT_EQ(matrix_const_2.getRows(), 10);
  EXPECT_EQ(matrix_const_2.getCols(), 10);
}

TEST(MatrixPropertyTest, TestMutators) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  matrix.setRows(4);
  EXPECT_EQ(matrix.getRows(), 4);
  EXPECT_EQ(matrix.getCols(), 2);
  double m[4][2] = {{0, 1}, {1, 2}, {0, 0}, {0, 0}};
  EXPECT_TRUE(MatrixIsEqual(matrix, m));

  matrix.setCols(5);
  EXPECT_EQ(matrix.getRows(), 4);
  EXPECT_EQ(matrix.getCols(), 5);
  double m_2[4][5] = {
      {0, 1, 0, 0, 0}, {1, 2, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
  EXPECT_TRUE(MatrixIsEqual(matrix, m_2));

  matrix.setRows(2);
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 5);
  double m_3[2][5] = {{0, 1, 0, 0, 0}, {1, 2, 0, 0, 0}};
  EXPECT_TRUE(MatrixIsEqual(matrix, m_3));

  matrix.setCols(1);
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 1);
  double m_4[2][1] = {{0}, {1}};
  EXPECT_TRUE(MatrixIsEqual(matrix, m_4));
}

TEST(MatrixPropertyTest, TestMutatorsBoundary) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  matrix.setCols(0);
  EXPECT_EQ(matrix.getRows(), 0);
  EXPECT_EQ(matrix.getCols(), 0);
  EXPECT_THROW(matrix(0, 0), std::out_of_range);

  Matrix matrix_2;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix_2(i, j) = i + j;
  matrix.setRows(0);
  EXPECT_EQ(matrix.getRows(), 0);
  EXPECT_EQ(matrix.getCols(), 0);
  EXPECT_THROW(matrix(0, 0), std::out_of_range);
}

TEST(MatrixMemberFunctionTest, TestEQ) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  Matrix matrix_2 = matrix;
  EXPECT_TRUE(matrix.EqMatrix(matrix_2));
  matrix.setCols(5);
  EXPECT_FALSE(matrix.EqMatrix(matrix_2));
  matrix.setCols(0);
  matrix_2.setRows(0);
  EXPECT_TRUE(matrix.EqMatrix(matrix_2));
}

TEST(MatrixMemberFunctionTest, TestSum) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  Matrix matrix_2 = matrix;
  double m[2][2] = {{0, 2}, {2, 4}};
  matrix.SumMatrix(matrix_2);
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 2);
  EXPECT_EQ(matrix_2.getRows(), 2);
  EXPECT_EQ(matrix_2.getCols(), 2);
}

TEST(MatrixMemberFunctionTest, TestSumDifferentSize) {
  Matrix matrix;
  Matrix matrix_2(2, 4);
  Matrix matrix_3(4, 2);
  EXPECT_THROW(matrix.SumMatrix(matrix_2), Matrix::DifferentMatrixSize);
  EXPECT_THROW(matrix.SumMatrix(matrix_3), Matrix::DifferentMatrixSize);
  try {
    matrix.SumMatrix(matrix_2);
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("cols count not equal", ex.what());
  }
  try {
    matrix.SumMatrix(matrix_3);
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("rows count not equal", ex.what());
  }
}

TEST(MatrixMemberFunctionTest, TestSub) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  Matrix matrix_2 = matrix;
  double m[2][2] = {{0, 0}, {0, 0}};
  matrix.SubMatrix(matrix_2);
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 2);
  EXPECT_EQ(matrix_2.getRows(), 2);
  EXPECT_EQ(matrix_2.getCols(), 2);
}

TEST(MatrixMemberFunctionTest, TestSubDifferentSize) {
  Matrix matrix;
  Matrix matrix_2(2, 4);
  Matrix matrix_3(4, 2);
  EXPECT_THROW(matrix.SubMatrix(matrix_2), Matrix::DifferentMatrixSize);
  EXPECT_THROW(matrix.SubMatrix(matrix_3), Matrix::DifferentMatrixSize);
  try {
    matrix.SubMatrix(matrix_2);
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("cols count not equal", ex.what());
  }
  try {
    matrix.SubMatrix(matrix_3);
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("rows count not equal", ex.what());
  }
}

TEST(MatrixMemberFunctionTest, TestMulNum) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  double m[2][2] = {{0, 2.2}, {2.2, 4.4}};
  matrix.MulNumber(2.2);
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) EXPECT_NEAR(matrix(i, j), m[i][j], 10e-6);
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 2);
}

TEST(MatrixMemberFunctionTest, TestMulMatrix) {
  Matrix matrix(2, 3);
  Matrix matrix_2(3, 2);
  for (int i = 0; i != 2; ++i) {
    for (int j = 0; j != 3; ++j) {
      matrix_2(j, i) = i - j;
      matrix(i, j) = i + j;
    }
  }
  double m_mul[2][2] = {{-5, -2}, {-8, -2}};
  double m_2[3][2] = {{0, 1}, {-1, 0}, {-2, -1}};
  matrix.MulMatrix(matrix_2);
  EXPECT_TRUE(MatrixIsEqual(matrix, m_mul));
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 2);
  EXPECT_TRUE(MatrixIsEqual(matrix_2, m_2));
  EXPECT_EQ(matrix_2.getRows(), 3);
  EXPECT_EQ(matrix_2.getCols(), 2);
}

TEST(MatrixMemberFunctionTest, TestMulMatrixDifferentSize) {
  Matrix matrix;
  Matrix matrix_2(4, 3);
  EXPECT_THROW(matrix.MulMatrix(matrix_2), Matrix::DifferentMatrixSize);
  try {
    matrix.MulMatrix(matrix_2);
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("cols first op operand not equal rows second op", ex.what());
  }
}

TEST(MatrixMemberFunctionTest, TestTranspose) {
  Matrix matrix(2, 3);
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 3; ++j) matrix(i, j) = i + j;
  double m[2][3] = {{0, 1, 2}, {1, 2, 3}};
  double m_2[3][2] = {{0, 1}, {1, 2}, {2, 3}};
  Matrix matrix_2 = matrix.Transpose();
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 3);
  EXPECT_TRUE(MatrixIsEqual(matrix_2, m_2));
  EXPECT_EQ(matrix_2.getRows(), 3);
  EXPECT_EQ(matrix_2.getCols(), 2);
}

TEST(MatrixMemberFunctionTest, TestDeterminant) {
  Matrix matrix(3, 3);
  double m[3][3] = {{0, 1, 2}, {1, 2, 3}, {2, 3, 4}};
  for (size_t i = 0; i != 3; ++i)
    for (size_t j = 0; j != 3; ++j) matrix(i, j) = i + j;
  double det = matrix.Determinant();
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  EXPECT_EQ(matrix.getRows(), 3);
  EXPECT_EQ(matrix.getCols(), 3);
  EXPECT_EQ(det, 0);

  Matrix matrix_2(2, 2);
  double m_2[2][2] = {{10, 0}, {0, 1}};
  matrix_2(0, 0) = 10;
  matrix_2(0, 1) = 0;
  matrix_2(1, 0) = 0;
  matrix_2(1, 1) = 1;
  double det_2 = matrix_2.Determinant();
  EXPECT_TRUE(MatrixIsEqual(matrix_2, m_2));
  EXPECT_EQ(matrix_2.getRows(), 2);
  EXPECT_EQ(matrix_2.getCols(), 2);
  EXPECT_EQ(det_2, 10);

  Matrix matrix_3;
  double m_3[2][2] = {{0, 0}, {0, 0}};
  double det_3 = matrix_3.Determinant();
  EXPECT_TRUE(MatrixIsEqual(matrix_3, m_3));
  EXPECT_EQ(matrix_3.getRows(), 2);
  EXPECT_EQ(matrix_3.getCols(), 2);
  EXPECT_EQ(det_3, 0);
}

TEST(MatrixMemberFunctionTest, TestDeterminantNotSquare) {
  Matrix matrix(3, 2);
  EXPECT_THROW(matrix.Determinant(), Matrix::NotSquare);
  try {
    matrix.Determinant();
  } catch (const Matrix::NotSquare& ex) {
    EXPECT_STREQ("matrix is not square", ex.what());
  }
}

TEST(MatrixMemberFunctionTest, TestCompliment) {
  Matrix matrix(3, 3);
  double m[3][3] = {{0, 1, 2}, {1, 2, 3}, {2, 3, 4}};
  for (size_t i = 0; i != 3; ++i)
    for (size_t j = 0; j != 3; ++j) matrix(i, j) = i + j;
  Matrix compliment_matrix = matrix.CalcComplements();
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  double m_compl[3][3] = {{-1, 2, -1}, {2, -4, 2}, {-1, 2, -1}};
  EXPECT_TRUE(MatrixIsEqual(compliment_matrix, m_compl));
  EXPECT_EQ(compliment_matrix.getRows(), 3);
  EXPECT_EQ(compliment_matrix.getCols(), 3);
  EXPECT_EQ(matrix.getRows(), 3);
  EXPECT_EQ(matrix.getCols(), 3);

  Matrix matrix_2(2, 2);
  double m_compl_2[2][2] = {{1, 0}, {0, 10}};
  matrix_2(0, 0) = 10;
  matrix_2(0, 1) = 0;
  matrix_2(1, 0) = 0;
  matrix_2(1, 1) = 1;
  Matrix compliment_matrix_2 = matrix_2.CalcComplements();
  EXPECT_TRUE(MatrixIsEqual(compliment_matrix_2, m_compl_2));
  EXPECT_EQ(compliment_matrix_2.getRows(), 2);
  EXPECT_EQ(compliment_matrix_2.getCols(), 2);

  Matrix matrix_3;
  double m_compl_3[2][2] = {{0, 0}, {0, 0}};
  Matrix compliment_matrix_3 = matrix_3.CalcComplements();
  EXPECT_TRUE(MatrixIsEqual(compliment_matrix_3, m_compl_3));
  EXPECT_EQ(compliment_matrix_3.getRows(), 2);
  EXPECT_EQ(compliment_matrix_3.getCols(), 2);
}

TEST(MatrixMemberFunctionTest, TestComplimentNotSquare) {
  Matrix matrix(3, 2);
  EXPECT_THROW(matrix.CalcComplements(), Matrix::NotSquare);
  try {
    matrix.CalcComplements();
  } catch (const Matrix::NotSquare& ex) {
    EXPECT_STREQ("matrix is not square", ex.what());
  }
}

TEST(MatrixMemberFunctionTest, TestInverseMatrix) {
  Matrix matrix;
  double m[2][2] = {{0, 1}, {1, 2}};
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  double m_inv[2][2] = {{-2, 1}, {1, 0}};
  Matrix inverse_matrix = matrix.InverseMatrix();
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  EXPECT_TRUE(MatrixIsEqual(inverse_matrix, m_inv));
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 2);
  EXPECT_EQ(inverse_matrix.getRows(), 2);
  EXPECT_EQ(inverse_matrix.getCols(), 2);
}

TEST(MatrixMemberFunctionTest, TestInverseMatrixZeroDeterminant) {
  Matrix matrix;
  EXPECT_THROW(matrix.InverseMatrix(), Matrix::ZeroDeterminant);
  Matrix matrix_2(3, 2);
  EXPECT_THROW(matrix_2.InverseMatrix(), Matrix::NotSquare);
  try {
    matrix.InverseMatrix();
  } catch (const Matrix::ZeroDeterminant& ex) {
    EXPECT_STREQ("matrix determinant is 0", ex.what());
  }
  try {
    matrix_2.InverseMatrix();
  } catch (const Matrix::NotSquare& ex) {
    EXPECT_STREQ("matrix is not square", ex.what());
  }
}

TEST(MatrixExternalOperatorTest, TestEq) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  Matrix matrix_2 = matrix;
  EXPECT_TRUE(matrix == matrix_2);
  matrix.setCols(5);
  EXPECT_FALSE(matrix == matrix_2);
  matrix.setCols(0);
  matrix_2.setRows(0);
  EXPECT_TRUE(matrix == matrix_2);
}

TEST(MatrixExternalOperatorTest, TestNeq) {
  Matrix matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 2; ++j) matrix(i, j) = i + j;
  Matrix matrix_2 = matrix;
  EXPECT_FALSE(matrix != matrix_2);
  matrix.setCols(5);
  EXPECT_TRUE(matrix != matrix_2);
  matrix.setCols(0);
  matrix_2.setRows(0);
  EXPECT_FALSE(matrix != matrix_2);
}

TEST(MatrixExternalOperatorTest, TestSum) {
  Matrix matrix(2, 3);
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 3; ++j) matrix(i, j) = i + j;
  Matrix matrix_2 = matrix;
  Matrix matrix_3 = matrix + matrix_2;
  double m[2][3] = {{0, 2, 4}, {2, 4, 6}};
  EXPECT_TRUE(MatrixIsEqual(matrix_3, m));
}

TEST(MatrixExternalOperatorTest, TestSumDifferentSize) {
  Matrix matrix;
  Matrix matrix_2(2, 4);
  Matrix matrix_3(4, 2);
  EXPECT_THROW(matrix + matrix_2, Matrix::DifferentMatrixSize);
  EXPECT_THROW(matrix + matrix_3, Matrix::DifferentMatrixSize);
  try {
    matrix + matrix_2;
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("cols count not equal", ex.what());
  }
  try {
    matrix + matrix_3;
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("rows count not equal", ex.what());
  }
}

TEST(MatrixExternalOperatorTest, TestSub) {
  Matrix matrix(2, 3);
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 3; ++j) matrix(i, j) = i + j;
  Matrix matrix_2 = matrix;
  Matrix matrix_3 = matrix - matrix_2;
  double m[2][3] = {{0, 0, 0}, {0, 0, 0}};
  EXPECT_TRUE(MatrixIsEqual(matrix_3, m));
}

TEST(MatrixExternalOperatorTest, TestSubDifferentSize) {
  Matrix matrix;
  Matrix matrix_2(2, 4);
  Matrix matrix_3(4, 2);
  EXPECT_THROW(matrix - matrix_2, Matrix::DifferentMatrixSize);
  EXPECT_THROW(matrix - matrix_3, Matrix::DifferentMatrixSize);
  try {
    matrix - matrix_2;
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("cols count not equal", ex.what());
  }
  try {
    matrix - matrix_3;
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("rows count not equal", ex.what());
  }
}

TEST(MatrixExternalOperatorTest, TestMulNumOverload) {
  Matrix matrix(2, 3);
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 3; ++j) matrix(i, j) = i + j;
  double m[2][3] = {{0, 2.2, 4.4}, {2.2, 4.4, 6.6}};
  Matrix matrix_2 = matrix * 2.2;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 3; ++j) EXPECT_NEAR(matrix_2(i, j), m[i][j], 10e-6);
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 3);
  Matrix matrix_3 = 2.2 * matrix;
  for (size_t i = 0; i != 2; ++i)
    for (size_t j = 0; j != 3; ++j) EXPECT_NEAR(matrix_3(i, j), m[i][j], 10e-6);
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 3);
}

TEST(MatrixExternalOperatorTest, TestMulMatrixOverload) {
  Matrix matrix(2, 3);
  Matrix matrix_2(3, 2);
  for (int i = 0; i != 2; ++i) {
    for (int j = 0; j != 3; ++j) {
      matrix_2(j, i) = i - j;
      matrix(i, j) = i + j;
    }
  }
  double m_mul[2][2] = {{-5, -2}, {-8, -2}};
  double m_2[3][2] = {{0, 1}, {-1, 0}, {-2, -1}};
  double m[2][3] = {{0, 1, 2}, {1, 2, 3}};
  Matrix matrix_3 = matrix * matrix_2;
  EXPECT_TRUE(MatrixIsEqual(matrix_3, m_mul));
  EXPECT_EQ(matrix_3.getRows(), 2);
  EXPECT_EQ(matrix_3.getCols(), 2);
  EXPECT_TRUE(MatrixIsEqual(matrix_2, m_2));
  EXPECT_EQ(matrix_2.getRows(), 3);
  EXPECT_EQ(matrix_2.getCols(), 2);
  EXPECT_TRUE(MatrixIsEqual(matrix, m));
  EXPECT_EQ(matrix.getRows(), 2);
  EXPECT_EQ(matrix.getCols(), 3);
}

TEST(MatrixExternalOperatorTest, TestMulMatrixOverloadDifferentSize) {
  Matrix matrix;
  Matrix matrix_2(4, 3);
  EXPECT_THROW(matrix * matrix_2, Matrix::DifferentMatrixSize);
  try {
    matrix* matrix_2;
  } catch (const Matrix::DifferentMatrixSize& ex) {
    EXPECT_STREQ("cols first op operand not equal rows second op", ex.what());
  }
}