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

/** A matrice of multiple forms. At the moment all types are supported. */
template <class DataTy> class RayTracerMatrice {
  static_assert(std::is_floating_point<DataTy>::value,
                "Matrice DataTy must be a floating point type.");

public:
  // Constructors
  // ============

  /** Not secured; every kind of matrices can be generated/exist. */
  RayTracerMatrice(){};
  RayTracerMatrice(const RayTracerMatrice &) = default;
  RayTracerMatrice(const std::vector<std::vector<DataTy>> &M) : m_matrice(M) {}
  RayTracerMatrice(std::vector<std::vector<DataTy>> &&M)
      : m_matrice(std::move(M)) {}
  RayTracerMatrice(RayTracerMatrice &&M) : m_matrice(std::move(M.m_matrice)) {}
  RayTracerMatrice(std::initializer_list<std::initializer_list<DataTy>> il)
      : m_matrice() {
    for (typename std::initializer_list<std::initializer_list<DataTy>>::iterator
             it = il.begin();
         it != il.end(); it++)
      m_matrice.push_back(*it);
  }
  /** Allocate a lines x columns uninitialized matrice. */
  RayTracerMatrice(unsigned lines, unsigned columns)
      : m_matrice(lines, std::vector<DataTy>(columns)) {}
  /** Allocate a lines x columns Val initialized matrice. */
  RayTracerMatrice(unsigned lines, unsigned columns, DataTy Val)
      : m_matrice(lines, std::vector<DataTy>(columns, Val)) {}

  /** Return an identity matrice being as following:
Matrice {    1.0,    0.0,    0.0,    0.0},
      {    0.0,    1.0,    0.0,    0.0},
      {    0.0,    0.0,    1.0,    0.0},
      {    0.0,    0.0,    0.0,    1.0}}*/
  static RayTracerMatrice identity() {
    return RayTracerMatrice({{1., 0., 0., 0.},
                             {0., 1., 0., 0.},
                             {0., 0., 1., 0.},
                             {0., 0., 0., 1.}});
  }

  // Special operators
  // =================

  RayTracerMatrice &operator=(const RayTracerMatrice &) = default;
  RayTracerMatrice &operator=(RayTracerMatrice &&) = default;
  RayTracerMatrice &
  operator=(std::initializer_list<std::initializer_list<DataTy>> il) {
    m_matrice.clear();
    for (typename std::initializer_list<std::initializer_list<DataTy>>::iterator
             it = il.begin();
         it != il.end(); it++)
      m_matrice.push_back(*it);
    return *this;
  }

  typedef DataTy DataType;

  // Accessors
  // =========

  /** Returns m_matrice. */
  const std::vector<std::vector<DataTy>> &matrice() const { return m_matrice; }

  /** Returns the Matrice shape as a tuple<int x, int, y>. */
  std::tuple<int, int> shape() const {
    return std::tuple<int, int>(m_matrice.size(), m_matrice[0].size());
  }

  /** Returns the Matrice number of rows. */
  unsigned getNumLines() const { return m_matrice.size(); }

  /** Returns the Matrice number of columns. */
  unsigned getNumColumns() const { return m_matrice[0].size(); }

  /** Returns the corresponding value(float). */
  DataTy at(unsigned line, unsigned column) const {
    return m_matrice[line][column];
  }

  bool is_invertible() const { return determinant(*this) != 0; }

  // Operators
  // =========
  bool operator==(const RayTracerMatrice &rhs) const {
    return m_matrice == rhs.m_matrice;
  }

  bool approximatly_equal(const RayTracerMatrice &rhs) {
    for (unsigned row = 0; row < getNumLines(); row++)
      for (unsigned column = 0; column < getNumColumns(); column++)
        if (!close_to_equal(at(row, column), rhs.at(row, column)))
          return false;
    return true;
  }

  bool operator!=(const RayTracerMatrice &rhs) const {
    return !operator==(rhs);
  }

  // Both must be 4*4 matrices.
  RayTracerMatrice &operator*=(const RayTracerMatrice &rhs) {
    assert(getNumColumns() == rhs.getNumLines() &&
           "Matrices must have compatible shapes");
    // Create a zero filled matrix of the same size.
    std::vector<std::vector<DataTy>> result(
        getNumLines(), std::vector<DataTy>(getNumColumns(), DataTy()));
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

  RayTracerMatrice &set(unsigned line, unsigned column, DataTy value) {
    m_matrice[line][column] = value;
    return *this;
  }

  void rotate_x(DataTy radians) { this *= rotation_x(radians); }
  void rotate_y(DataTy radians) { this *= rotation_y(radians); }
  void rotate_z(DataTy radians) { this *= rotation_z(radians); }

  void scale(DataTy x, DataTy y, DataTy z) { this *= scaling(x, y, z); }
  /*
  #help: Should be working...
  void translate(DataTy x, DataTy y, DataTy z) {
    //this = this * translation(x, y, z);
    this *= translation(x, y, z);
  }*/

private:
  // #Help: Should it be an array as a matrice is of fixed size.
  std::vector<std::vector<DataTy>> m_matrice;
};

using Matrice = RayTracerMatrice<RayTracerDataType>;

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

Matrice::DataType determinant(const Matrice &M);

/** Returns a submatrix(a matrice# with one row and column less). */
inline Matrice submatrix(const Matrice &M, unsigned line, unsigned column) {
  assert(line < M.getNumLines() && "line out of Matrice bounds.");
  assert(column < M.getNumColumns() && "column out of Matrice bounds.");

  std::vector<std::vector<Matrice::DataType>> table;
  table.reserve(M.getNumLines() - 1);
  for (unsigned l = 0; l < M.getNumLines(); l++) {
    if (l != line) {
      std::vector<Matrice::DataType> TheLine;
      TheLine.reserve(M.getNumColumns() - 1);
      for (unsigned c = 0; c < M.getNumColumns(); c++) {
        if (c != column)
          TheLine.push_back(M.at(l, c));
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
inline Matrice::DataType matrixminor(const Matrice &M, unsigned line,
                                     unsigned column) {
  return determinant(submatrix(M, line, column));
}

/** Returns the cofactor of a 3*3 matrix. */
inline Matrice::DataType cofactor(const Matrice &M, unsigned line,
                                  unsigned column) {
  Matrice::DataType m = matrixminor(M, line, column);

  return ((line + column) % 2) != 0 ? -m : m;
}

inline Matrice transpose(const Matrice &M) {
  std::vector<std::vector<Matrice::DataType>> future_matrice;

  // Extract column; M_column = future_line
  for (unsigned M_column = 0; M_column < M.getNumColumns(); M_column++) {
    future_matrice.push_back(std::vector<Matrice::DataType>());
    for (unsigned it_line = 0; it_line < M.getNumLines(); it_line++)
      future_matrice[M_column].push_back(M.matrice()[it_line][M_column]);
  }
  return Matrice(std::move(future_matrice));
}

/** Returns the determinant of 2*2, 3*3 or 4*4 matrices. */
inline Matrice::DataType determinant(const Matrice &M) {
  if (M.shape() == std::tuple<int, int>(2, 2))
    return M.at(0, 0) * M.at(1, 1) - M.at(0, 1) * M.at(1, 0);

  Matrice::DataType result = 0.0;
  if (M.shape() == std::tuple<int, int>(4, 4) ||
      M.shape() == std::tuple<int, int>(3, 3)) {
    for (unsigned column = 0; column < M.getNumColumns(); column++)
      result += M.at(0, column) * cofactor(M, 0, column);
  } else
    assert(0 && "Case not handled.");

  return result;
}

/* Returns an inversed matrix. */
inline Matrice inverse(const Matrice &M) {
  Matrice M2(M.getNumColumns(), M.getNumLines());
  Matrice::DataType det = determinant(M);
  if (det != 0.0) { // Matrice is invertible
    for (unsigned row = 0; row < M.getNumLines(); row++)
      for (unsigned col = 0; col < M.getNumColumns(); col++)
        M2.set(col, row, cofactor(M, row, col) / det);
  } else
    assert(0 && "Matrice is not invertible.");
  return M2;
}

// Chapter 4 transformations
// =========================

inline Matrice translation(Matrice::DataType x, Matrice::DataType y,
                           Matrice::DataType z) {
  Matrice result = Matrice::identity();
  result.set(0, 3, x);
  result.set(1, 3, y);
  result.set(2, 3, z);
  return result;
}

/** Refer at the top of p49 for visual explanations. */
inline Matrice scaling(Matrice::DataType x, Matrice::DataType y,
                       Matrice::DataType z) {
  Matrice result = Matrice::identity();
  result.set(0, 0, x);
  result.set(1, 1, y);
  result.set(2, 2, z);
  return result;
}

/** Refer at the top of p50 for visual explanations. */
inline Matrice rotation_x(Matrice::DataType radians) {
  Matrice result = Matrice::identity();
  result.set(1, 1, std::cos(radians));
  result.set(1, 2, -std::sin(radians));
  result.set(2, 1, std::sin(radians));
  result.set(2, 2, std::cos(radians));
  return result;
}

/** Refer at the bottom of p50 for visual explanations. */
inline Matrice rotation_y(Matrice::DataType radians) {
  Matrice result = Matrice::identity();
  result.set(0, 0, std::cos(radians));
  result.set(0, 2, std::sin(radians));
  result.set(2, 0, -std::sin(radians));
  result.set(2, 2, std::cos(radians));
  return result;
}

inline Matrice rotation_z(Matrice::DataType radians) {
  Matrice result = Matrice::identity();
  result.set(0, 0, std::cos(radians));
  result.set(0, 1, -std::sin(radians));
  result.set(1, 0, std::sin(radians));
  result.set(1, 1, std::cos(radians));
  return result;
}

/** Function that moves points proportionnaly to an axis. See image p51 for more
 * information.*/
inline Matrice shearing(Matrice::DataType Xy, Matrice::DataType Xz,
                        Matrice::DataType Yx, Matrice::DataType Yz,
                        Matrice::DataType Zx, Matrice::DataType Zy) {
  Matrice result = Matrice::identity();
  result.set(0, 1, Xy);
  result.set(0, 2, Xz);
  result.set(1, 0, Yx);
  result.set(1, 2, Yz);
  result.set(2, 0, Zx);
  result.set(2, 1, Zy);
  return result;
}

} // namespace ratrac

/** Return something like :
  Matrice {    1.0,    1.0,    1.0,    1.0},
  {    1.0,    1.0,    1.0,    1.0},
  {    1.0,    1.0,    1.0,    1.0},
  {    1.0,    1.0,    1.0,    1.0}}
  */
std::ostream &operator<<(std::ostream &os, const ratrac::Matrice &M);
