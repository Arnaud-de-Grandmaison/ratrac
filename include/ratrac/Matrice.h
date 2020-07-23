#pragma once

#include "ratrac/Tuple.h"

#include <cmath>
#include <iostream>
#include <ostream>
#include <tuple>
#include <vector>
// #help: Should float be a double or in a template ?

namespace ratrac {
// template/constructor/? of functions @compiler
class Matrice;
float cofactor(const Matrice &M, const unsigned &line, const unsigned &column);
float determinant(const Matrice &M);

/** A matrice of multiple forms. At the moment all types are supported. */
class Matrice {
public:
  /** Not secured; every kind of matrices can be generated/exist. */
  Matrice(std::vector<std::vector<float>> matrix) : m_matrice(matrix){};

  // Accessors

  /** Returns m_matrice. */
  std::vector<std::vector<float>> matrice() const { return m_matrice; }
  /** Returns the size as a tuple<int x, int, y>.
  #help: should probably returns a int rather than a tuple. */
  std::tuple<int, int> size() const {
    return std::tuple<int, int>(m_matrice.size(), m_matrice[0].size());
  };
  /** Returns the corresponding value(float). */
  float at(unsigned line, unsigned column) const {
    return m_matrice[line][column];
  };
  const bool is_invertible() const { return determinant(*this) != 0; }

  // Editors

  void set(const unsigned &line, const unsigned &column, const float &value) {
    m_matrice[line][column] = value;
  }

  // Operators

  bool operator==(const Matrice &rhs) const {
    return m_matrice == rhs.m_matrice;
  }

  bool approximatly_equal(const Matrice &rhs) {
    for (int row(0); row < std::get<0>(size()); row++) {
      for (int column(0); column < std::get<1>(size()); column++) {
        if (!close_to_equal(at(row, column), rhs.at(row, column))) {
          std::cout << row << column << at(row, column) << rhs.at(row, column)
                    << std::endl;
          return false;
        }
      }
    }
    return true;
  }
  bool operator!=(const Matrice &rhs) const { return !operator==(rhs); }

  static bool close_to_equal(const float &a, const float &b) {
    const float EPSILON = 0.00001;
    return std::abs(a - b) < EPSILON;
  }
  // Both must be 4*4 matrices.
  // #help: do we have to raise an error if matrices are not of size 4x4
  //        Actually, it works for every case, except if both matrices have
  //        a diferent size.
  Matrice &operator*=(const Matrice &rhs) {
    // Copy the current matrice to get the exact same size.
    std::vector<std::vector<float>> future_matrix = m_matrice;
    std::vector<float> added_rhs;
    for (unsigned line(0); line < std::get<0>(rhs.size()); line++) {
      for (unsigned column(0); column < std::get<1>(rhs.size()); column++) {
        // Make sure the value is 0
        future_matrix[line][column] = 0;
        // Extract column
        added_rhs.clear();
        for (int it_line(0); it_line < rhs.m_matrice.size(); it_line++) {
          added_rhs.push_back(rhs.m_matrice[it_line][column]);
        }
        // Do the specific operation
        for (int it(0); it < m_matrice.size(); it++) {
          future_matrix[line][column] += m_matrice[line][it] * added_rhs[it];
        }
      }
    }
    m_matrice = future_matrix;
    return *this;
  }

private:
  // #Help: Should it be an array as a matrice is of fixed size.
  std::vector<std::vector<float>> m_matrice;
};

inline Matrice operator*(const Matrice &lhs, const Matrice &rhs) {
  Matrice tmp = lhs;
  tmp *= rhs;
  return tmp;
}

inline Tuple operator*(const Matrice &lhs, const Tuple &rhs) {
  std::vector<float> future_tuple;
  for (unsigned line(0); line < std::get<0>(lhs.size()); line++) {
    future_tuple.push_back(0);
    future_tuple[line] += lhs.at(line, 0) * rhs.x();
    future_tuple[line] += lhs.at(line, 1) * rhs.y();
    future_tuple[line] += lhs.at(line, 2) * rhs.z();
    future_tuple[line] += lhs.at(line, 3) * rhs.w();
  }
  return Tuple(future_tuple[0], future_tuple[1], future_tuple[2],
               future_tuple[3]);
}

/** Return an identity matrice being as following:
Matrice {    1.0,    0.0,    0.0,    0.0},
        {    0.0,    1.0,    0.0,    0.0},
        {    0.0,    0.0,    1.0,    0.0},
        {    0.0,    0.0,    0.0,    1.0}}*/
inline Matrice identity_matrix() {
  return Matrice(std::vector<std::vector<float>>(
      {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}));
}

inline Matrice transpose(const Matrice &M) {
  std::vector<std::vector<float>> future_matrice;

  // Extract column; M_column = future_line
  for (unsigned M_column(0); M_column < std::get<1>(M.size()); M_column++) {
    future_matrice.push_back(std::vector<float>());
    for (int it_line(0); it_line < std::get<0>(M.size()); it_line++) {
      future_matrice[M_column].push_back(M.matrice()[it_line][M_column]);
    }
  }
  return Matrice(future_matrice);
}

/** Returns the determinant of a 2*2 matrice. */
inline float determinant(const Matrice &M) {
  if (M.size() == std::tuple<int, int>(2, 2))
    return M.at(0, 0) * M.at(1, 1) - M.at(0, 1) * M.at(1, 0);
  else if (M.size() == std::tuple<int, int>(4, 4) ||
           M.size() == std::tuple<int, int>(3, 3)) {
    float result = 0;
    for (int column(0); column < std::get<1>(M.size()); column++) {
      result += M.at(0, column) * cofactor(M, 0, column);
    }
    return result;
  }
}

/** Returns a submatrix(a matrice# with one row and column less). */
inline Matrice submatrix(const Matrice &M, const unsigned line,
                         const unsigned column) {
  std::vector<std::vector<float>> current_matrix(M.matrice());

  std::vector<std::vector<float>> table;
  std::vector<float> line_vec;
  for (unsigned it_line(0); it_line < std::get<0>(M.size()); it_line++) {
    if (it_line != line) {
      for (unsigned it_column(0); it_column < std::get<1>(M.size());
           it_column++) {
        if (it_column != column) {
          line_vec.push_back(current_matrix[it_line][it_column]);
        }
      }
      table.push_back(line_vec);
      line_vec.clear();
    }
  }
  return Matrice(table);
}

/** Returns the determinant of a submatrice; " minor is easier to say than
 * determinant of a submatrice ". */
inline float minor(const Matrice &M, const unsigned &line,
                   const unsigned &column) {
  return determinant(submatrix(M, line, column));
}

/** Returns the cofactor of a 3*3 matrix.
#help: can be optimised (& ?)*/
inline float cofactor(const Matrice &M, const unsigned &line,
                      const unsigned &column) {
  if (((line + column) % 2) != 0) {
    return -minor(M, line, column);
  } else {
    return minor(M, line, column);
  }
}

/* Returns an inversed matrix. */
inline Matrice inverse(const Matrice &M) {
  if (M.is_invertible()) {
    Matrice M2 = M;
    float cofac;
    for (int row(0); row < std::get<0>(M.size()); row++) {
      for (int col(0); col < std::get<1>(M.size()); col++) {
        cofac = cofactor(M, row, col);
        M2.set(col, row, cofac / determinant(M));
      }
    }
    return M2;
  }
}

} // namespace ratrac

/** Return something like :
Matrice {    1.0,    1.0,    1.0,    1.0},
        {    1.0,    1.0,    1.0,    1.0},
        {    1.0,    1.0,    1.0,    1.0},
        {    1.0,    1.0,    1.0,    1.0}}
*/
std::ostream &operator<<(std::ostream &os, const ratrac::Matrice &M);