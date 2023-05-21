#pragma once

#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

#include <cassert>
#include <cmath>
#include <cstring>
#include <initializer_list>
#include <ostream>
#include <tuple>

namespace ratrac {
class Matrix;

/** Square matrices of  size 2x2, 3x3 or 4x4. */
class Matrix {
  static const unsigned NR = 4;       // Maximum number of rows.
  static const unsigned NC = 4;       // Maximum  number of columns.
  static const unsigned NE = NR * NC; // Maximum number of elements.
  static_assert(NR == NC, "only square matrices are supported.");
  static_assert(std::is_floating_point<ratrac::RayTracerDataType>::value,
                "Matrix DataTy must be a floating point type.");

public:
  typedef ratrac::RayTracerDataType DataType;

  // Constructors
  // ============

  Matrix() = delete;
  Matrix(const Matrix &M)
      : m_Matrix(new DataType[NE]), m_Rows(M.m_Rows), m_Columns(M.m_Columns) {
    assert(m_Matrix && "Allocation error");
    std::memcpy(m_Matrix, M.m_Matrix, NE * sizeof(DataType));
  }
  // Matrix(const std::vector<std::vector<DataType>> &M) : m_Matrix(M) {}
  // Matrix(std::vector<std::vector<DataType>> &&M) : m_Matrix(std::move(M)) {}
  Matrix(Matrix &&M)
      : m_Matrix(M.m_Matrix), m_Rows(M.m_Rows), m_Columns(M.m_Columns) {
    M.m_Matrix = nullptr;
    M.m_Rows = 0;
    M.m_Columns = 0;
  }

  Matrix(std::initializer_list<std::initializer_list<DataType>> il);

  /** Allocate a rows x columns uninitialized Matrix. */
  Matrix(unsigned rows, unsigned columns)
      : m_Matrix(new DataType[NE]), m_Rows(rows), m_Columns(columns) {
    assert(m_Matrix && "Allocation error");
  }
  /** Allocate a rows x columns Val initialized Matrix. */
  Matrix(unsigned rows, unsigned columns, DataType Val)
      : m_Matrix(new DataType[NE]), m_Rows(rows), m_Columns(columns) {
    assert(m_Matrix && "Allocation error");
    for (unsigned row = 0; row < rows; row++)
      for (unsigned col = 0; col < columns; col++)
        set(row, col, Val);
  }

  Matrix &operator=(const Matrix &M) {
    if (!m_Matrix)
      m_Matrix = new DataType[NE];
    assert(m_Matrix && "Allocation error");
    std::memcpy(m_Matrix, M.m_Matrix, NE * sizeof(DataType));
    m_Rows = M.m_Rows;
    m_Columns = M.m_Columns;
    return *this;
  }

  Matrix &operator=(Matrix &&M) {
    if (m_Matrix)
      delete[] m_Matrix;
    m_Matrix = M.m_Matrix;
    m_Rows = M.m_Rows;
    m_Columns = M.m_Columns;
    M.m_Matrix = nullptr;
    M.m_Rows = 0;
    M.m_Columns = 0;
    return *this;
  }

  Matrix &operator=(std::initializer_list<std::initializer_list<DataType>> il);

  ~Matrix() { delete[] m_Matrix; }

  /** Get the identity Matrix */
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

  /** Function that moves points proportionally to an axis. See image p51 for
   * more information.*/
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

  DataType determinant() const;

  bool is_invertible() const { return determinant() != 0; }

  /** Returns a submatrix(a Matrix with one row and column less). */
  Matrix submatrix(unsigned row, unsigned column) const;

  /** Returns the determinant of a subMatrix; " minor is easier to say than
   * determinant of a subMatrix ".
   * Note: we would prefer to nme it minor, but this fails with gcc that has a
   * macro with the same name...
   * */
  DataType matrixminor(unsigned row, unsigned column) const {
    return submatrix(row, column).determinant();
  }

  /** Returns the cofactor of a 3*3 matrix. */
  DataType cofactor(unsigned row, unsigned column) const {
    DataType m = matrixminor(row, column);

    return ((row + column) % 2) != 0 ? -m : m;
  }

  /** Transpose this matrix in place. */
  Matrix &transpose();

  /** Invert this Matrix in place. */
  Matrix &inverse();

  // Accessors
  // =========

  /** Returns the Matrix shape as a tuple<int x, int, y>. */
  constexpr std::tuple<int, int> shape() const {
    return std::tuple<int, int>(m_Rows, m_Columns);
  }

  /** Returns the Matrix number of rows. */
  constexpr unsigned rows() const { return m_Rows; }

  /** Returns the Matrix number of columns. */
  constexpr unsigned columns() const { return m_Columns; }

  /** Returns the corresponding value(float). */
  constexpr DataType operator()(unsigned row, unsigned column) const {
    assert(row < m_Rows && "Out of bound row access");
    assert(column < m_Columns && "Out of bound column access");
    assert(m_Matrix && "Nowhere to get the data from");
    return m_Matrix[row * NC + column];
  }
  /** Returns the corresponding value(float). */
  constexpr DataType at(unsigned row, unsigned column) const {
    return this->operator()(row, column);
  }

  // Operators
  // =========
  bool operator==(const Matrix &rhs) const ;
  bool approximatly_equal(const Matrix &rhs) const;
  bool operator!=(const Matrix &rhs) const { return !operator==(rhs); }

  explicit operator std::string() const;

  Matrix &operator*=(const Matrix &rhs);

  // Editors
  // =======

  constexpr Matrix &set(unsigned row, unsigned column, DataType value) {
    assert(row < m_Rows && "Out of bound row access");
    assert(column < m_Columns && "Out of bound column access");
    assert(m_Matrix && "Nowhere to get the data from");
    m_Matrix[row * NC + column] = value;
    return *this;
  }

  // Fluent API operations: these operations allow to chain matrix operations in
  // the expected (i.e as you read them order) instead of having to write matrix
  // multiplications in the reverse order.
  Matrix &rotate_x(DataType radians) noexcept {
    Matrix T = rotation_x(radians);
    T *= *this;
    *this = std::move(T);
    return *this;
  }

  Matrix &rotate_y(DataType radians) noexcept {
    Matrix T = rotation_y(radians);
    T *= *this;
    *this = std::move(T);
    return *this;
  }

  Matrix &rotate_z(DataType radians) noexcept {
    Matrix T = rotation_z(radians);
    T *= *this;
    *this = std::move(T);
    return *this;
  }

  Matrix &scale(DataType x, DataType y, DataType z) noexcept {
    Matrix T = scaling(x, y, z);
    T *= *this;
    *this = std::move(T);
    return *this;
  }

  Matrix &translate(DataType x, DataType y, DataType z) noexcept {
    Matrix T = translation(x, y, z);
    T *= *this;
    *this = std::move(T);
    return *this;
  }

  Matrix &shear(DataType Xy, DataType Xz, DataType Yx, DataType Yz, DataType Zx,
                DataType Zy) noexcept {
    Matrix T = shearing(Xy, Xz, Yx, Yz, Zx, Zy);
    T *= *this;
    *this = std::move(T);
    return *this;
  }

private:
  DataType *m_Matrix;
  unsigned m_Rows;
  unsigned m_Columns;
};

// Other operators
// ===============

inline Matrix operator*(const Matrix &lhs, const Matrix &rhs) {
  Matrix tmp = lhs;
  tmp *= rhs;
  return tmp;
}

inline Tuple operator*(const Matrix &lhs, const Tuple &rhs) {
  Tuple T;
  assert(lhs.rows() == lhs.columns() && lhs.rows() == rhs.size() &&
         "Incompatible number of rows and columns.");
  for (unsigned row = 0; row < lhs.rows(); row++) {
    Matrix::DataType acc = Matrix::DataType();
    for (unsigned col = 0; col < lhs.columns(); col++)
      acc += lhs.at(row, col) * rhs[col];
    T[row] = acc;
  }

  return T;
}
// Other chapter 3 stuff
// =====================

/** Returns the determinant of 2*2, 3*3 or 4*4 Matrices. */
inline Matrix::DataType determinant(const Matrix &M) { return M.determinant(); }

/** Returns a submatrix(a Matrix with one row and column less). */
inline Matrix submatrix(const Matrix &M, unsigned row, unsigned col) {
  return M.submatrix(row, col);
}

/** Returns the determinant of a subMatrix; " minor is easier to say than
 * determinant of a subMatrix ".
 * Note: we would prefer to name it minor, but this fails with gcc that has a
 * macro with the same name...
 * */
inline Matrix::DataType matrixminor(const Matrix &M, unsigned row,
                                    unsigned col) {
  return M.matrixminor(row, col);
}

/** Returns the cofactor of a 3*3 matrix. */
inline Matrix::DataType cofactor(const Matrix &M, unsigned row, unsigned col) {
  return M.cofactor(row, col);
}

/** Returns the cofactor of a 3*3 matrix. */
inline bool is_invertible(const Matrix &M) { return M.is_invertible(); }

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
