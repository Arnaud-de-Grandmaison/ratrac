#include "ratrac/Matrix.h"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace ratrac {

Matrix::Matrix(std::initializer_list<std::initializer_list<DataType>> il)
    : m_Matrix(new DataType[NE]), m_Rows(0), m_Columns(0) {
  assert(m_Matrix && "Allocation error");
  assert(il.size() <= NR && "Number of rows must be < NR");
  m_Rows = il.size();
  unsigned row = 0;
  for (typename std::initializer_list<std::initializer_list<DataType>>::iterator
           itr = il.begin();
       itr != il.end(); row++, itr++) {
    assert(itr->size() <= NR && "Number of rows must be < NR");
    m_Columns = itr->size() > m_Columns ? itr->size() : m_Columns;
    unsigned col = 0;
    for (typename std::initializer_list<DataType>::iterator itc = itr->begin();
         itc != itr->end(); col++, itc++)
      set(row, col, *itc);
  }
}

Matrix &
Matrix::operator=(std::initializer_list<std::initializer_list<DataType>> il) {
  if (!m_Matrix)
    m_Matrix = new DataType[NE];
  assert(m_Matrix && "Allocation error");
  m_Columns = 0;
  assert(il.size() <= NR && "Number of rows must be < NR");
  m_Rows = il.size();
  unsigned row = 0;
  for (typename std::initializer_list<std::initializer_list<DataType>>::iterator
           itr = il.begin();
       itr != il.end(); row++, itr++) {
    assert(itr->size() <= NR && "Number of rows must be < NR");
    m_Columns = itr->size() > m_Columns ? itr->size() : m_Columns;
    unsigned col = 0;
    for (typename std::initializer_list<DataType>::iterator itc = itr->begin();
         itc != itr->end(); col++, itc++)
      set(row, col, *itc);
  }
  return *this;
}

Matrix::DataType Matrix::determinant() const {
  assert(m_Rows == m_Columns && "Only square matrices are supported");
  assert(m_Rows <= NR && "Number of rows higher than supported");
  assert(m_Columns <= NC && "Number of rows higher than supported");
  if (shape() == std::tuple<int, int>(2, 2))
    return at(0, 0) * at(1, 1) - at(0, 1) * at(1, 0);

  DataType result = DataType();
  if (shape() == std::tuple<int, int>(4, 4) ||
      shape() == std::tuple<int, int>(3, 3))
    for (unsigned col = 0; col < columns(); col++)
      result += at(0, col) * cofactor(0, col);

  return result;
}

Matrix Matrix::submatrix(unsigned row, unsigned col) const {
  assert(row < rows() && "row out of Matrix bounds.");
  assert(col < columns() && "column out of Matrix bounds.");

  Matrix M(rows() - 1, columns() - 1);
  for (unsigned r = 0, ro = 0; ro < rows(); ro++)
    if (ro != row) {
      for (unsigned c = 0, co = 0; co < columns(); co++)
        if (co != col) {
          M.set(r, c, at(ro, co));
          c++;
        }
      r++;
    }

  return M;
}

Matrix &Matrix::transpose() {
  Matrix M(columns(), rows());

  for (unsigned row = 0; row < rows(); row++)
    for (unsigned col = 0; col < columns(); col++)
      M.set(col, row, at(row, col));

  *this = std::move(M);
  return *this;
}

Matrix &Matrix::inverse() {
  Matrix::DataType det = determinant();
  assert(det != 0.0 && "Matrix is not invertible.");
  Matrix M2(columns(), rows());
  if (det != 0.0) { // Matrix is invertible
    for (unsigned row = 0; row < rows(); row++)
      for (unsigned col = 0; col < columns(); col++)
        M2.set(col, row, cofactor(row, col) / det);
  }

  *this = std::move(M2);
  return *this;
}

bool Matrix::operator==(const Matrix &rhs) const {
  if (m_Rows != rhs.m_Rows || m_Columns != rhs.m_Columns)
    return false;
  if (m_Matrix == rhs.m_Matrix)
    return true;
  for (unsigned row = 0; row < m_Rows; row++)
    for (unsigned column = 0; column < m_Columns; column++)
      if (at(row, column) != rhs.at(row, column))
        return false;
  return true;
}

bool Matrix::approximatly_equal(const Matrix &rhs) const {
  assert(m_Rows == rhs.m_Rows && m_Columns == rhs.columns() &&
         "Can not compare matrices of different shapes");
  for (unsigned row = 0; row < m_Rows; row++)
    for (unsigned col = 0; col < m_Columns; col++)
      if (!close_to_equal(at(row, col), rhs.at(row, col)))
        return false;
  return true;
}

Matrix::operator std::string() const {
  std::ostringstream os;
  os << std::fixed << std::setprecision(1); // Use fixed precision.
  os << "Matrix { ";
  for (unsigned x = 0; x < rows(); x++) {
    for (unsigned y = 0; y < columns(); y++) {
      os << std::setw(6) << std::setfill(' ');
      os << at(x, y);
      if (y != columns() - 1)
        os << ", ";
    }
    if (x != rows() - 1)
      os << "},\n\t{ ";
  }
  os << "}}\n";

  return os.str();
}

Matrix &Matrix::operator*=(const Matrix &rhs) {
  assert(columns() == rhs.rows() && "Matrices must have compatible shapes");
  Matrix M(rows(), columns());
  for (unsigned row = 0; row < rows(); row++)
    for (unsigned col = 0; col < rhs.columns(); col++) {
      DataType acc = DataType();
      for (unsigned it = 0; it < columns(); it++)
        acc += at(row, it) * rhs.at(it, col);
      M.set(row, col, acc);
    }

  *this = std::move(M);
  return *this;
}

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Matrix &M) {
  os << std::string(M);

  return os;
}
