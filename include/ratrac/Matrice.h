#pragma once

#include <cassert>
#include <cmath>
#include <initializer_list>
#include <ostream>
#include <tuple>
#include <vector>

#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

namespace ratrac {
class Matrice;

/** A matrice of multiple forms. At the moment all types are supported. */
class Matrice {
  static_assert(std::is_floating_point<ratrac::RayTracerDataType>::value,
                "Matrice DataTy must be a floating point type.");

public:
  typedef ratrac::RayTracerDataType DataType;

  // Constructors
  // ============

  /** Not secured; every kind of matrices can be generated/exist. */
  Matrice(){};
  Matrice(const Matrice &) = default;
  Matrice(const std::vector<std::vector<DataType>> &M) : m_matrice(M) {}
  Matrice(std::vector<std::vector<DataType>> &&M)
      : m_matrice(std::move(M)) {}
  Matrice(Matrice &&M) : m_matrice(std::move(M.m_matrice)) {}
  Matrice(std::initializer_list<std::initializer_list<DataType>> il)
      : m_matrice() {
    for (typename std::initializer_list<std::initializer_list<DataType>>::iterator
             it = il.begin();
         it != il.end(); it++)
      m_matrice.push_back(*it);
  }
  /** Allocate a lines x columns uninitialized matrice. */
  Matrice(unsigned lines, unsigned columns)
      : m_matrice(lines, std::vector<DataType>(columns)) {}
  /** Allocate a lines x columns Val initialized matrice. */
  Matrice(unsigned lines, unsigned columns, DataType Val)
      : m_matrice(lines, std::vector<DataType>(columns, Val)) {}

  /** Return an identity matrice being as following:
Matrice {    1.0,    0.0,    0.0,    0.0},
      {    0.0,    1.0,    0.0,    0.0},
      {    0.0,    0.0,    1.0,    0.0},
      {    0.0,    0.0,    0.0,    1.0}}*/
  static Matrice identity() {
    return Matrice({{1., 0., 0., 0.},
                    {0., 1., 0., 0.},
                    {0., 0., 1., 0.},
                    {0., 0., 0., 1.}});
  }

// Chapter 4 transformations
// =========================

static Matrice translation(DataType x, DataType y,
                           DataType z) {
  Matrice result = Matrice::identity();
  result.set(0, 3, x);
  result.set(1, 3, y);
  result.set(2, 3, z);
  return result;
}

/** Refer at the top of p49 for visual explanations. */
static Matrice scaling(DataType x, DataType y,
                       DataType z) {
  Matrice result = Matrice::identity();
  result.set(0, 0, x);
  result.set(1, 1, y);
  result.set(2, 2, z);
  return result;
}

/** Refer at the top of p50 for visual explanations. */
static Matrice rotation_x(DataType radians) {
  Matrice result = Matrice::identity();
  result.set(1, 1, std::cos(radians));
  result.set(1, 2, -std::sin(radians));
  result.set(2, 1, std::sin(radians));
  result.set(2, 2, std::cos(radians));
  return result;
}

/** Refer at the bottom of p50 for visual explanations. */
static Matrice rotation_y(DataType radians) {
  Matrice result = Matrice::identity();
  result.set(0, 0, std::cos(radians));
  result.set(0, 2, std::sin(radians));
  result.set(2, 0, -std::sin(radians));
  result.set(2, 2, std::cos(radians));
  return result;
}

static Matrice rotation_z(DataType radians) {
  Matrice result = Matrice::identity();
  result.set(0, 0, std::cos(radians));
  result.set(0, 1, -std::sin(radians));
  result.set(1, 0, std::sin(radians));
  result.set(1, 1, std::cos(radians));
  return result;
}

/** Function that moves points proportionnaly to an axis. See image p51 for more
 * information.*/
static Matrice shearing(DataType Xy, DataType Xz,
                        DataType Yx, DataType Yz,
                        DataType Zx, DataType Zy) {
  Matrice result = Matrice::identity();
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

  Matrice &operator=(const Matrice &) = default;
  Matrice &operator=(Matrice &&) = default;
  Matrice &
  operator=(std::initializer_list<std::initializer_list<DataType>> il) {
    m_matrice.clear();
    for (typename std::initializer_list<std::initializer_list<DataType>>::iterator
             it = il.begin();
         it != il.end(); it++)
      m_matrice.push_back(*it);
    return *this;
  }

  DataType determinant() const {
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

  bool is_invertible() const { return determinant() != 0; }

  /** Returns a submatrix(a matrice with one row and column less). */
  Matrice submatrix(unsigned line, unsigned column) const {
    assert(line < getNumLines() && "line out of Matrice bounds.");
    assert(column < getNumColumns() && "column out of Matrice bounds.");

    std::vector<std::vector<Matrice::DataType>> table;
    table.reserve(getNumLines() - 1);
    for (unsigned l = 0; l < getNumLines(); l++) {
      if (l != line) {
        std::vector<Matrice::DataType> TheLine;
        TheLine.reserve(getNumColumns() - 1);
        for (unsigned c = 0; c < getNumColumns(); c++) {
          if (c != column)
            TheLine.push_back(at(l, c));
        }
        table.push_back(std::move(TheLine));
      }
    }

    return Matrice(std::move(table));
  }

  /** Returns the determinant of a submatrice; " minor is easier to say than
   * determinant of a submatrice ".
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
  Matrice &transpose() {
    std::vector<std::vector<Matrice::DataType>> future_matrice;

    // Extract column; M_column = future_line
    for (unsigned M_column = 0; M_column < getNumColumns(); M_column++) {
      future_matrice.push_back(std::vector<Matrice::DataType>());
      for (unsigned it_line = 0; it_line < getNumLines(); it_line++)
        future_matrice[M_column].push_back(matrice()[it_line][M_column]);
    }

    m_matrice = std::move(future_matrice);
    return *this;
  }

  /** Invert this Matrix in place. */
  Matrice &inverse() {
    Matrice M2(getNumColumns(), getNumLines());
    Matrice::DataType det = determinant();
    if (det != 0.0) { // Matrice is invertible
      for (unsigned row = 0; row < getNumLines(); row++)
        for (unsigned col = 0; col < getNumColumns(); col++)
          M2.set(col, row, cofactor(row, col) / det);
    } else
      assert(0 && "Matrice is not invertible.");

    *this = std::move(M2);
    return *this;
  }

  // Accessors
  // =========

  /** Returns m_matrice. */
  const std::vector<std::vector<DataType>> &matrice() const { return m_matrice; }

  /** Returns the Matrice shape as a tuple<int x, int, y>. */
  std::tuple<int, int> shape() const {
    return std::tuple<int, int>(m_matrice.size(), m_matrice[0].size());
  }

  /** Returns the Matrice number of rows. */
  unsigned getNumLines() const { return m_matrice.size(); }

  /** Returns the Matrice number of columns. */
  unsigned getNumColumns() const { return m_matrice[0].size(); }

  /** Returns the corresponding value(float). */
  DataType at(unsigned line, unsigned column) const {
    return m_matrice[line][column];
  }

  // Operators
  // =========
  bool operator==(const Matrice &rhs) const {
    return m_matrice == rhs.m_matrice;
  }

  bool approximatly_equal(const Matrice &rhs) {
    for (unsigned row = 0; row < getNumLines(); row++)
      for (unsigned column = 0; column < getNumColumns(); column++)
        if (!close_to_equal(at(row, column), rhs.at(row, column)))
          return false;
    return true;
  }

  bool operator!=(const Matrice &rhs) const {
    return !operator==(rhs);
  }

  // Both must be 4*4 matrices.
  Matrice &operator*=(const Matrice &rhs) {
    assert(getNumColumns() == rhs.getNumLines() &&
           "Matrices must have compatible shapes");
    // Create a zero filled matrix of the same size.
    std::vector<std::vector<DataType>> result(
        getNumLines(), std::vector<DataType>(getNumColumns(), DataType()));
    for (unsigned line = 0; line < getNumLines(); line++)
      for (unsigned column = 0; column < rhs.getNumColumns(); column++)
        for (unsigned it = 0; it < getNumColumns(); it++)
          result[line][column] +=
              m_matrice[line][it] * rhs.m_matrice[it][column];
    m_matrice = std::move(result);
    return *this;
  }

  // Editors
  // =======

  Matrice &set(unsigned line, unsigned column, DataType value) {
    m_matrice[line][column] = value;
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
  // #Help: Should it be an array as a matrice is of fixed size.
  std::vector<std::vector<DataType>> m_matrice;
};

// Other operators
// ===============

inline Matrice operator*(const Matrice &lhs, const Matrice &rhs) {
  Matrice tmp = lhs;
  tmp *= rhs;
  return tmp;
}

inline Tuple operator*(const Matrice &lhs, const Tuple &rhs) {
  Tuple future_tuple;
  assert(lhs.getNumLines() == lhs.getNumColumns() &&
         lhs.getNumLines() == rhs.size() &&
         "Incompatible number of rows and columns.");
  for (unsigned line = 0; line < lhs.getNumLines(); line++) {
    Matrice::DataType acc = 0.0;
    for (unsigned column = 0; column < lhs.getNumColumns(); column++)
      acc += lhs.at(line, column) * rhs[column];
    future_tuple[line] = acc;
  }

  return future_tuple;
}
// Other chapter 3 stuff
// =====================

/** Returns the determinant of 2*2, 3*3 or 4*4 matrices. */
inline Matrice::DataType determinant(const Matrice &M) {
  return M.determinant();
}

/** Returns a submatrix(a matrice with one row and column less). */
inline Matrice submatrix(const Matrice &M, unsigned line, unsigned column) {
  return M.submatrix(line, column);
}

/** Returns the determinant of a submatrice; " minor is easier to say than
 * determinant of a submatrice ".
 * Note: we would prefer to name it minor, but this fails with gcc that has a
 * macro with the same name...
 * */
inline Matrice::DataType matrixminor(const Matrice &M, unsigned line,
                                     unsigned column) {
  return M.matrixminor(line, column);
}

/** Returns the cofactor of a 3*3 matrix. */
inline Matrice::DataType cofactor(const Matrice &M, unsigned line,
                                  unsigned column) {
  return M.cofactor(line, column);
}

/** Returns the cofactor of a 3*3 matrix. */
inline bool is_invertible(const Matrice &M) {
  return M.is_invertible();
}

inline Matrice transpose(const Matrice &M) {
  Matrice tmp = M;
  return tmp.transpose();
}

/* Returns an inversed matrix. */
inline Matrice inverse(const Matrice &M) {
  Matrice tmp = M;
  return tmp.inverse();
}

} // namespace ratrac

/** Return something like :
  Matrice {    1.0,    1.0,    1.0,    1.0},
  {    1.0,    1.0,    1.0,    1.0},
  {    1.0,    1.0,    1.0,    1.0},
  {    1.0,    1.0,    1.0,    1.0}}
  */
std::ostream &operator<<(std::ostream &os, const ratrac::Matrice &M);
