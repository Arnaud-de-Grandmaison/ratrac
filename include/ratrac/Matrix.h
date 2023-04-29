#pragma once

#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

#include <cassert>
#include <cmath>
#include <initializer_list>
#include <ostream>
#include <tuple>
#include <vector>

namespace ratrac {
class Matrix;

/** A Matrix of multiple forms. At the moment all types are supported. */
class Matrix {
  static_assert(std::is_floating_point<ratrac::RayTracerDataType>::value,
                "Matrix DataTy must be a floating point type.");

public:
  typedef ratrac::RayTracerDataType DataType;

  // Constructors
  // ============

  /** Not secured; every kind of Matrixs can be generated/exist. */
  Matrix() {}
  Matrix(const Matrix &) = default;
  Matrix(const std::vector<std::vector<DataType>> &M) : m_Matrix(M) {}
  Matrix(std::vector<std::vector<DataType>> &&M) : m_Matrix(std::move(M)) {}
  Matrix(Matrix &&M) : m_Matrix(std::move(M.m_Matrix)) {}
  Matrix(std::initializer_list<std::initializer_list<DataType>> il)
      : m_Matrix() {
    for (typename std::initializer_list<
             std::initializer_list<DataType>>::iterator it = il.begin();
         it != il.end(); it++)
      m_Matrix.push_back(*it);
  }
  /** Allocate a lines x columns uninitialized Matrix. */
  Matrix(unsigned lines, unsigned columns)
      : m_Matrix(lines, std::vector<DataType>(columns)) {}
  /** Allocate a lines x columns Val initialized Matrix. */
  Matrix(unsigned lines, unsigned columns, DataType Val)
      : m_Matrix(lines, std::vector<DataType>(columns, Val)) {}

  /** Return an identity Matrix being as following:
Matrix {    1.0,    0.0,    0.0,    0.0},
      {    0.0,    1.0,    0.0,    0.0},
      {    0.0,    0.0,    1.0,    0.0},
      {    0.0,    0.0,    0.0,    1.0}}*/
  static Matrix identity() noexcept {
    return Matrix({{1., 0., 0., 0.},
                    {0., 1., 0., 0.},
                    {0., 0., 1., 0.},
                    {0., 0., 0., 1.}});
  }

// Chapter 4 transformations
// =========================

  static Matrix translation(DataType x, DataType y, DataType z) {
    Matrix result = Matrix::identity();
    result.set(0, 3, x);
    result.set(1, 3, y);
    result.set(2, 3, z);
    return result;
  }

  /** Refer at the top of p49 for visual explanations. */
  static Matrix scaling(DataType x, DataType y, DataType z) {
    Matrix result = Matrix::identity();
    result.set(0, 0, x);
    result.set(1, 1, y);
    result.set(2, 2, z);
    return result;
  }

  /** Refer at the top of p50 for visual explanations. */
  static Matrix rotation_x(DataType radians) {
    Matrix result = Matrix::identity();
    result.set(1, 1, std::cos(radians));
    result.set(1, 2, -std::sin(radians));
    result.set(2, 1, std::sin(radians));
    result.set(2, 2, std::cos(radians));
    return result;
  }

  /** Refer at the bottom of p50 for visual explanations. */
  static Matrix rotation_y(DataType radians) {
    Matrix result = Matrix::identity();
    result.set(0, 0, std::cos(radians));
    result.set(0, 2, std::sin(radians));
    result.set(2, 0, -std::sin(radians));
    result.set(2, 2, std::cos(radians));
    return result;
  }

  static Matrix rotation_z(DataType radians) {
    Matrix result = Matrix::identity();
    result.set(0, 0, std::cos(radians));
    result.set(0, 1, -std::sin(radians));
    result.set(1, 0, std::sin(radians));
    result.set(1, 1, std::cos(radians));
    return result;
  }

/** Function that moves points proportionally to an axis. See image p51 for more
 * information.*/
  static Matrix shearing(DataType Xy, DataType Xz, DataType Yx, DataType Yz,
                          DataType Zx, DataType Zy) {
    Matrix result = Matrix::identity();
    result.set(0, 1, Xy);
    result.set(0, 2, Xz);
    result.set(1, 0, Yx);
    result.set(1, 2, Yz);
    result.set(2, 0, Zx);
    result.set(2, 1, Zy);
    return result;
  }
  // Special operators
  // =================

  Matrix &operator=(const Matrix &) = default;
  Matrix &operator=(Matrix &&) = default;
  Matrix &
  operator=(std::initializer_list<std::initializer_list<DataType>> il) {
    m_Matrix.clear();
    for (typename std::initializer_list<std::initializer_list<DataType>>::iterator
             it = il.begin();
         it != il.end(); it++)
      m_Matrix.push_back(*it);
    return *this;
  }

  constexpr DataType determinant() const {
    if (shape() == std::tuple<int, int>(2, 2))
      return at(0, 0) * at(1, 1) - at(0, 1) * at(1, 0);

    DataType result = 0.0;
    if (shape() == std::tuple<int, int>(4, 4) ||
        shape() == std::tuple<int, int>(3, 3)) {
      for (unsigned column = 0; column < getNumColumns(); column++)
        result += at(0, column) * cofactor(0, column);
    } else
      assert(0 && "Case not handled.");

    return result;
  }

  constexpr bool is_invertible() const { return determinant() != 0; }

  /** Returns a submatrix(a Matrix with one row and column less). */
  Matrix submatrix(unsigned line, unsigned column) const {
    assert(line < getNumLines() && "line out of Matrix bounds.");
    assert(column < getNumColumns() && "column out of Matrix bounds.");

    std::vector<std::vector<Matrix::DataType>> table;
    table.reserve(getNumLines() - 1);
    for (unsigned l = 0; l < getNumLines(); l++) {
      if (l != line) {
        std::vector<Matrix::DataType> TheLine;
        TheLine.reserve(getNumColumns() - 1);
        for (unsigned c = 0; c < getNumColumns(); c++) {
          if (c != column)
            TheLine.push_back(at(l, c));
        }
        table.push_back(std::move(TheLine));
      }
    }

    return Matrix(std::move(table));
  }

  /** Returns the determinant of a subMatrix; " minor is easier to say than
   * determinant of a subMatrix ".
   * Note: we would prefer to nme it minor, but this fails with gcc that has a
   * macro with the same name...
   * */
  DataType matrixminor(unsigned line, unsigned column) const {
    return submatrix(line, column).determinant();
  }

  /** Returns the cofactor of a 3*3 matrix. */
  DataType cofactor(unsigned line, unsigned column) const {
    DataType m = matrixminor(line, column);

    return ((line + column) % 2) != 0 ? -m : m;
  }

  /** Transpose this matrix in place. */
  Matrix &transpose() {
    std::vector<std::vector<Matrix::DataType>> future_Matrix;

    // Extract column; M_column = future_line
    for (unsigned M_column = 0; M_column < getNumColumns(); M_column++) {
      future_Matrix.push_back(std::vector<Matrix::DataType>());
      for (unsigned it_line = 0; it_line < getNumLines(); it_line++)
        future_Matrix[M_column].push_back(matrix()[it_line][M_column]);
    }

    m_Matrix = std::move(future_Matrix);
    return *this;
  }

  /** Invert this Matrix in place. */
  Matrix &inverse() {
    Matrix M2(getNumColumns(), getNumLines());
    Matrix::DataType det = determinant();
    if (det != 0.0) { // Matrix is invertible
      for (unsigned row = 0; row < getNumLines(); row++)
        for (unsigned col = 0; col < getNumColumns(); col++)
          M2.set(col, row, cofactor(row, col) / det);
    } else
      assert(0 && "Matrix is not invertible.");

    *this = std::move(M2);
    return *this;
  }

  // Accessors
  // =========

  /** Returns m_Matrix. */
  const std::vector<std::vector<DataType>> &matrix() const { return m_Matrix; }

  /** Returns the Matrix shape as a tuple<int x, int, y>. */
  constexpr std::tuple<int, int> shape() const {
    return std::tuple<int, int>(m_Matrix.size(), m_Matrix[0].size());
  }

  /** Returns the Matrix number of rows. */
  constexpr unsigned getNumLines() const { return m_Matrix.size(); }

  /** Returns the Matrix number of columns. */
  constexpr unsigned getNumColumns() const { return m_Matrix[0].size(); }

  /** Returns the corresponding value(float). */
  constexpr DataType at(unsigned line, unsigned column) const {
    return m_Matrix[line][column];
  }

  // Operators
  // =========
  bool operator==(const Matrix &rhs) const {
    return m_Matrix == rhs.m_Matrix;
  }

  bool approximatly_equal(const Matrix &rhs) {
    for (unsigned row = 0; row < getNumLines(); row++)
      for (unsigned column = 0; column < getNumColumns(); column++)
        if (!close_to_equal(at(row, column), rhs.at(row, column)))
          return false;
    return true;
  }

  bool operator!=(const Matrix &rhs) const {
    return !operator==(rhs);
  }

  explicit operator std::string() const;

  // Both must be 4*4 Matrixs.
  Matrix &operator*=(const Matrix &rhs) {
    assert(getNumColumns() == rhs.getNumLines() &&
           "Matrixs must have compatible shapes");
    // Create a zero filled matrix of the same size.
    std::vector<std::vector<DataType>> result(
        getNumLines(), std::vector<DataType>(getNumColumns(), DataType()));
    for (unsigned line = 0; line < getNumLines(); line++)
      for (unsigned column = 0; column < rhs.getNumColumns(); column++)
        for (unsigned it = 0; it < getNumColumns(); it++)
          result[line][column] +=
              m_Matrix[line][it] * rhs.m_Matrix[it][column];
    m_Matrix = std::move(result);
    return *this;
  }

  // Editors
  // =======

  constexpr Matrix &set(unsigned line, unsigned column, DataType value) {
    m_Matrix[line][column] = value;
    return *this;
  }

  void rotate_x(DataType radians) { *this *= rotation_x(radians); }
  void rotate_y(DataType radians) { *this *= rotation_y(radians); }
  void rotate_z(DataType radians) { *this *= rotation_z(radians); }

  void scale(DataType x, DataType y, DataType z) { *this *= scaling(x, y, z); }
  /*
  #help: Should be working...
  void translate(DataTy x, DataTy y, DataTy z) {
    //this = this * translation(x, y, z);
    this *= translation(x, y, z);
  }*/

private:
  // #Help: Should it be an array as a Matrix is of fixed size.
  std::vector<std::vector<DataType>> m_Matrix;
};

// Other operators
// ===============

inline Matrix operator*(const Matrix &lhs, const Matrix &rhs) {
  Matrix tmp = lhs;
  tmp *= rhs;
  return tmp;
}

inline Tuple operator*(const Matrix &lhs, const Tuple &rhs) {
  Tuple future_tuple;
  assert(lhs.getNumLines() == lhs.getNumColumns() &&
         lhs.getNumLines() == rhs.size() &&
         "Incompatible number of rows and columns.");
  for (unsigned line = 0; line < lhs.getNumLines(); line++) {
    Matrix::DataType acc = 0.0;
    for (unsigned column = 0; column < lhs.getNumColumns(); column++)
      acc += lhs.at(line, column) * rhs[column];
    future_tuple[line] = acc;
  }

  return future_tuple;
}
// Other chapter 3 stuff
// =====================

/** Returns the determinant of 2*2, 3*3 or 4*4 Matrixs. */
inline Matrix::DataType determinant(const Matrix &M) {
  return M.determinant();
}

/** Returns a submatrix(a Matrix with one row and column less). */
inline Matrix submatrix(const Matrix &M, unsigned line, unsigned column) {
  return M.submatrix(line, column);
}

/** Returns the determinant of a subMatrix; " minor is easier to say than
 * determinant of a subMatrix ".
 * Note: we would prefer to name it minor, but this fails with gcc that has a
 * macro with the same name...
 * */
inline Matrix::DataType matrixminor(const Matrix &M, unsigned line,
                                     unsigned column) {
  return M.matrixminor(line, column);
}

/** Returns the cofactor of a 3*3 matrix. */
inline Matrix::DataType cofactor(const Matrix &M, unsigned line,
                                  unsigned column) {
  return M.cofactor(line, column);
}

/** Returns the cofactor of a 3*3 matrix. */
inline bool is_invertible(const Matrix &M) {
  return M.is_invertible();
}

inline Matrix transpose(const Matrix &M) {
  Matrix tmp = M;
  return tmp.transpose();
}

/* Returns an inversed matrix. */
inline Matrix inverse(const Matrix &M) {
  Matrix tmp = M;
  return tmp.inverse();
}

} // namespace ratrac

/** Return something like :
  Matrix {    1.0,    1.0,    1.0,    1.0},
  {    1.0,    1.0,    1.0,    1.0},
  {    1.0,    1.0,    1.0,    1.0},
  {    1.0,    1.0,    1.0,    1.0}}
  */
std::ostream &operator<<(std::ostream &os, const ratrac::Matrix &M);
