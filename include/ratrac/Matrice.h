#pragma once

#include "ratrac/Tuple.h"

#include <iostream>
#include <ostream>
#include <tuple>
#include <vector>

namespace ratrac {
/** A matrice of multiple forms. At the moment all types are supported. */
class Matrice {
public:
  /** Not secured; every kind of matrices can be generated/exist. */
  Matrice(std::vector<std::vector<float>> matrix) : m_matrice(matrix){};

  // Accessors

  /** Returns m_matrice. */
  std::vector<std::vector<float>> matrice() const { return m_matrice; }
  /** Returns the size as a tuple<int x, int, y>.*/
  std::tuple<int, int> size() const {
    return std::tuple<int, int>(m_matrice.size(), m_matrice[0].size());
  };
  /** Returns the corresponding value(float). */
  float at(unsigned line, unsigned column) const {
    return m_matrice[line][column];
  };

  // Operators

  bool operator==(const Matrice &rhs) const {
    return m_matrice == rhs.m_matrice;
  }
  bool operator!=(const Matrice &rhs) const { return !operator==(rhs); }

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

} // namespace ratrac

/** Return something like :
Matrice {    1.0,    1.0,    1.0,    1.0},
        {    1.0,    1.0,    1.0,    1.0},
        {    1.0,    1.0,    1.0,    1.0},
        {    1.0,    1.0,    1.0,    1.0}}
*/
std::ostream &operator<<(std::ostream &os, const ratrac::Matrice &M);