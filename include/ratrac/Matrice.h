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

// template/constructor/? of functions @compiler
template <class DataTy> class RayTracerMatrice;
template <class DataTy>
DataTy cofactor(const RayTracerMatrice<DataTy> &M, unsigned line,
                unsigned column);
template <class DataTy> DataTy determinant(const RayTracerMatrice<DataTy> &M);

/** A matrice of multiple forms. At the moment all types are supported. */
template <class DataTy> class RayTracerMatrice {
public:
  /** Not secured; every kind of matrices can be generated/exist. */
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

  /** Returns m_matrice. */
  std::vector<std::vector<DataTy>> matrice() const { return m_matrice; }

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

  // Editors

  void set(unsigned line, unsigned column, DataTy value) {
    m_matrice[line][column] = value;
  }

  // Operators

  bool approximatly_equal(const RayTracerMatrice &rhs) {
    for (unsigned row = 0; row < getNumLines(); row++)
      for (unsigned column = 0; column < getNumColumns(); column++)
        if (!close_to_equal(at(row, column), rhs.at(row, column)))
          return false;
    return true;
  }
  bool operator==(const RayTracerMatrice &rhs) const {
    return m_matrice == rhs.m_matrice;
  }
  bool operator!=(const RayTracerMatrice &rhs) const {
    return !operator==(rhs);
  }

  /** Return an identity matrice being as following:
  Matrice {    1.0,    0.0,    0.0,    0.0},
          {    0.0,    1.0,    0.0,    0.0},
          {    0.0,    0.0,    1.0,    0.0},
          {    0.0,    0.0,    0.0,    1.0}}*/
  static RayTracerMatrice identity_matrix() {
    return RayTracerMatrice({{1., 0., 0., 0.},
                             {0., 1., 0., 0.},
                             {0., 0., 1., 0.},
                             {0., 0., 0., 1.}});
  }

  // Both must be 4*4 matrices.
  RayTracerMatrice &operator*=(const RayTracerMatrice &rhs) {
    // Copy the current matrice to get the exact same size.
    assert(shape() == rhs.shape() && "Matrices must have compatible shapes");
    std::vector<std::vector<DataTy>> future_matrix = m_matrice;
    std::vector<DataTy> added_rhs;
    for (unsigned line = 0; line < getNumLines(); line++) {
      for (unsigned column = 0; column < getNumColumns(); column++) {
        // Make sure the value is 0
        future_matrix[line][column] = 0;
        // Extract column
        added_rhs.clear();
        for (int it_line(0); it_line < rhs.getNumLines(); it_line++) {
          added_rhs.push_back(rhs.m_matrice[it_line][column]);
        }
        // Do the specific operation
        for (int it(0); it < getNumLines(); it++) {
          future_matrix[line][column] += m_matrice[line][it] * added_rhs[it];
        }
      }
    }
    m_matrice = future_matrix;
    return *this;
  }

private:
  // #Help: Should it be an array as a matrice is of fixed size.
  std::vector<std::vector<DataTy>> m_matrice;
};

template <class DataTy>
inline RayTracerMatrice<DataTy> operator*(const RayTracerMatrice<DataTy> &lhs,
                                          const RayTracerMatrice<DataTy> &rhs) {
  RayTracerMatrice<DataTy> tmp = lhs;
  tmp *= rhs;
  return tmp;
}

template <class DataTy>
inline Tuple operator*(const RayTracerMatrice<DataTy> &lhs, const Tuple &rhs) {
  Tuple future_tuple;
  assert(lhs.getNumLines() == lhs.getNumColumns() &&
         lhs.getNumLines() == rhs.size() &&
         "Incompatible number of rows and columns.");
  for (unsigned line = 0; line < lhs.getNumLines(); line++) {
    DataTy acc = 0.0;
    for (unsigned column = 0; column < lhs.getNumColumns(); column++)
      acc += lhs.at(line, column) * rhs[column];
    future_tuple[line] = acc;
  }

  return future_tuple;
}

template <class DataTy>
inline RayTracerMatrice<DataTy> transpose(const RayTracerMatrice<DataTy> &M) {
  std::vector<std::vector<DataTy>> future_matrice;

  // Extract column; M_column = future_line
  for (unsigned M_column = 0; M_column < M.getNumColumns(); M_column++) {
    future_matrice.push_back(std::vector<DataTy>());
    for (int it_line = 0; it_line < M.getNumLines(); it_line++)
      future_matrice[M_column].push_back(M.matrice()[it_line][M_column]);
  }
  return RayTracerMatrice<DataTy>(std::move(future_matrice));
}

/** Returns the determinant of 2*2, 3*3 or 4*4 matrices. */
template <class DataTy>
inline DataTy determinant(const RayTracerMatrice<DataTy> &M) {
  if (M.shape() == std::tuple<int, int>(2, 2))
    return M.at(0, 0) * M.at(1, 1) - M.at(0, 1) * M.at(1, 0);

  DataTy result = 0.0;
  if (M.shape() == std::tuple<int, int>(4, 4) ||
      M.shape() == std::tuple<int, int>(3, 3)) {
    for (unsigned column = 0; column < M.getNumColumns(); column++)
      result += M.at(0, column) * cofactor(M, 0, column);
  } else
    assert(0 && "Case not handled.");

  return result;
}

/** Returns a submatrix(a matrice# with one row and column less). */
template <class DataTy>
inline RayTracerMatrice<DataTy> submatrix(const RayTracerMatrice<DataTy> &M,
                                          unsigned line, unsigned column) {
  assert(line < M.getNumLines() && "line out of Matrice bounds.");
  assert(column < M.getNumColumns() && "column out of Matrice bounds.");
  std::vector<std::vector<DataTy>> table = M.matrice();
  table.erase(table.begin() + line);
  for (std::vector<DataTy> &v : table)
    v.erase(v.begin() + column);

  return RayTracerMatrice<DataTy>(table);
}

/** Returns the determinant of a submatrice; " minor is easier to say than
 * determinant of a submatrice ".
 * Note: we would prefer to nme it minor, but this fails with gcc that has a
 * macro with the same name...
 * */
template <class DataTy>
inline DataTy matrixminor(const RayTracerMatrice<DataTy> &M, unsigned line,
                    unsigned column) {
  return determinant(submatrix(M, line, column));
}

/** Returns the cofactor of a 3*3 matrix. */
template <class DataTy>
inline DataTy cofactor(const RayTracerMatrice<DataTy> &M, unsigned line,
                       unsigned column) {
  DataTy m = matrixminor(M, line, column);

  return ((line + column) % 2) != 0 ? -m : m;
}

/* Returns an inversed matrix. */
template <class DataTy>
inline RayTracerMatrice<DataTy> inverse(const RayTracerMatrice<DataTy> &M) {
  RayTracerMatrice<DataTy> M2 = M;
  if (M.is_invertible()) {
    for (unsigned row = 0; row < M.getNumLines(); row++)
      for (unsigned col = 0; col < M.getNumColumns(); col++) {
        DataTy cofac = cofactor(M, row, col);
        M2.set(col, row, cofac / determinant(M));
      }
  } else
    assert(0 && "Matrice is not invertible.");
  return M2;
}

using Matrice = RayTracerMatrice<RayTracerDataType>;
} // namespace ratrac

/** Return something like :
  Matrice {    1.0,    1.0,    1.0,    1.0},
  {    1.0,    1.0,    1.0,    1.0},
  {    1.0,    1.0,    1.0,    1.0},
  {    1.0,    1.0,    1.0,    1.0}}
  */
std::ostream &operator<<(std::ostream &os, const ratrac::Matrice &M);
