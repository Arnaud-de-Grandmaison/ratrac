#pragma once

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

private:
  // #Help: Should it be an array as a matrice is of fixed size.
  std::vector<std::vector<float>> m_matrice;
};

} // namespace ratrac

/** Return something like :
Matrice {    1.0,    1.0,    1.0,    1.0},
        {    1.0,    1.0,    1.0,    1.0},
        {    1.0,    1.0,    1.0,    1.0},
        {    1.0,    1.0,    1.0,    1.0}}
*/
std::ostream &operator<<(std::ostream &os, const ratrac::Matrice &M);