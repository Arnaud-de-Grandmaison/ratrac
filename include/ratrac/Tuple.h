#pragma once

#include <array>
#include <cmath>
#include <ostream>

#include "ratrac/ratrac.h"

namespace ratrac {
// Classes
// =======

/** RayTracerTuple represent the Tuples ratrac uses to manipulate point and
 * vectors. */
class Tuple {
  static_assert(std::is_floating_point<ratrac::RayTracerDataType>::value,
                "Tuple DataTy must be a floating point type.");

public:
  typedef ratrac::RayTracerDataType DataType;

  /** Initialise the Tuple/Vector4.*/
  Tuple() {}
  template <class XTy, class YTy, class ZTy, class WTy>
  Tuple(XTy x, YTy y, ZTy z, WTy w)
      : m_tuple{DataType(x), DataType(y), DataType(z), DataType(w)} {}

  /** Helpers to create points and vectors. */
  template <class XTy, class YTy, class ZTy>
  static Tuple Vector(XTy x, YTy y, ZTy z) {
    return Tuple(x, y, z, DataType(0.0));
  }

  template <class XTy, class YTy, class ZTy>
  static Tuple Point(XTy x, YTy y, ZTy z) {
    return Tuple(x, y, z, DataType(1.0));
  }

  // Accessors
  // =========

  const DataType &x() const { return m_tuple[0]; }
  const DataType &y() const { return m_tuple[1]; }
  const DataType &z() const { return m_tuple[2]; }
  const DataType &w() const { return m_tuple[3]; }

  DataType &operator[](size_t index) { return m_tuple[index]; }
  const DataType &operator[](size_t index) const { return m_tuple[index]; }

  bool isPoint() const { return m_tuple[3] == 1.0; }
  bool isVector() const { return m_tuple[3] == 0.0; }

  size_t size() const { return m_tuple.size(); }

  // Advanced vectors properties
  // ===========================

  DataType magnitude() const {
    DataType result = 0.0;
    for (const DataType &c : m_tuple)
      result += c * c;
    return std::sqrt(result);
  }

  Tuple &normalize() {
    *this /= magnitude();
    return *this;
  }

  // Operators
  // =========

  // equal/not equal

  bool operator==(const Tuple &rhs) const {
    return close_to_equal(x(), rhs.x()) && close_to_equal(y(), rhs.y()) &&
           close_to_equal(z(), rhs.z()) && close_to_equal(w(), rhs.w());
  }
  bool operator!=(const Tuple &rhs) const { return !operator==(rhs); }

  // operations

  Tuple &operator+=(const Tuple &rhs) {
    for (unsigned i = 0; i < m_tuple.size(); i++)
      m_tuple[i] += rhs.m_tuple[i];
    return *this;
  }
  Tuple &operator-=(const Tuple &rhs) {
    for (unsigned i = 0; i < m_tuple.size(); i++)
      m_tuple[i] -= rhs.m_tuple[i];
    return *this;
  }
  Tuple &operator*=(DataType rhs) {
    for (DataType &c : m_tuple)
      c *= rhs;
    return *this;
  }
  Tuple &operator/=(DataType rhs) {
    for (DataType &c : m_tuple)
      c /= rhs;
    return *this;
  }

  Tuple operator-() const {
    return Tuple(-m_tuple[0], -m_tuple[1], -m_tuple[2], -m_tuple[3]);
  }

  // Advanced operations

  DataType dot(const Tuple &rhs) const {
    DataType result = 0;
    for (unsigned i = 0; i < m_tuple.size(); i++) {
      result += m_tuple[i] * rhs.m_tuple[i];
    }
    return result;
  }

  Tuple cross(const Tuple &rhs) const {
    return Vector(m_tuple[1] * rhs.m_tuple[2] - m_tuple[2] * rhs.m_tuple[1],
                  m_tuple[2] * rhs.m_tuple[0] - m_tuple[0] * rhs.m_tuple[2],
                  m_tuple[0] * rhs.m_tuple[1] - m_tuple[1] * rhs.m_tuple[0]);
  }

  Tuple reflect(const Tuple &normal) const {
    Tuple n = normal;
    n *= 2.0 * dot(normal);
    Tuple tmp(*this);
    tmp -= n;
    return tmp;
  }

private:
  /** An array formated as following: Tuple(x, y, z, w). Args type:
   * float/double. */
  std::array<DataType, 4> m_tuple;
};

// Other/External operators
// ========================

// Adding two Tuples/Points/Vector returns repectively a Tuple, a Point or a
// Vector.
/** Adding two Tuples/Vector4.*/
inline Tuple operator+(const Tuple &lhs, const Tuple &rhs) {
  Tuple tmp = lhs;
  tmp += rhs;
  return tmp;
}

/** Substracting two Tuples/Vectors returns respectively a Tuple or a Vector.
 * Subtracting two Points returns a Vector.
 * Subtracting a Vector from a Point returns a Point. */
inline Tuple operator-(const Tuple &lhs, const Tuple &rhs) {
  Tuple tmp = lhs;
  tmp -= rhs;
  return tmp;
}

/** Scalar multiplication. */
inline Tuple operator*(const Tuple &lhs, Tuple::DataType rhs) {
  Tuple tmp = lhs;
  tmp *= rhs;
  return tmp;
}
inline Tuple operator*(Tuple::DataType lhs, const Tuple &rhs) {
  Tuple tmp = rhs;
  tmp *= lhs;
  return tmp;
}

/** Scalar division. */
inline Tuple operator/(const Tuple &lhs, Tuple::DataType rhs) {
  Tuple tmp = lhs;
  tmp /= rhs;
  return tmp;
}

// Function like operators
// =======================

inline Tuple::DataType magnitude(const Tuple &T) { return T.magnitude(); }

inline Tuple normalize(const Tuple &T) {
  Tuple tmp = T;
  return tmp.normalize();
  }

// Advanced operations
// ===================

/** Dot product. */
inline Tuple::DataType dot(const Tuple &lhs, const Tuple &rhs) {
  Tuple tmp = lhs;
  return tmp.dot(rhs);
}

/** Dot product. */
inline Tuple cross(const Tuple &lhs, const Tuple &rhs) {
  Tuple tmp = lhs;
  return tmp.cross(rhs);
}

/** Reflect a vector. */
inline Tuple reflect(const Tuple &in, const Tuple &normal) {
  Tuple tmp = in;
  return tmp.reflect(normal);
}

/** Helper to create a Vector, i.e. a Tuple with w = 0.0 */
template <class XTy, class YTy, class ZTy>
inline Tuple Vector(XTy x, YTy y, ZTy z) {
  return Tuple::Vector(x, y, z);
}

/** Helper to create a Point, i.e. a Tuple with w = 1.0 */
template <class XTy, class YTy, class ZTy>
inline Tuple Point(XTy x, YTy y, ZTy z) {
  return Tuple::Point(x, y, z);
}

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Tuple &T);
