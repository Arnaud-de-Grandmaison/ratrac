#pragma once

#include <array>
#include <cmath>
#include <ostream>

#include "ratrac/ratrac.h"

namespace ratrac {
// Classes
// =======

/** RayTraceTuple represent the Tuples ratrac uses to manipulate point and
 * vectors. */
template <class DataTy> class RayTracerTuple {
  static_assert(std::is_floating_point<DataTy>::value, "Tuple DataTy must be a floating point type.");

public:
  /** Initialise the Tuple/Vector4.*/
  RayTracerTuple() {}
  template <class XTy, class YTy, class ZTy, class WTy>
  RayTracerTuple(XTy x, YTy y, ZTy z, WTy w)
      : m_tuple{DataTy(x), DataTy(y), DataTy(z), DataTy(w)} {}

  /** Helpers to create points and vectors. */
  template <class XTy, class YTy, class ZTy>
  static RayTracerTuple Vector(XTy x, YTy y, ZTy z) {
    return RayTracerTuple(x, y, z, RayTracerTuple::DataType(0.0));
  }

  template <class XTy, class YTy, class ZTy>
  static RayTracerTuple Point(XTy x, YTy y, ZTy z) {
    return RayTracerTuple(x, y, z, RayTracerTuple::DataType(1.0));
  }

  typedef DataTy DataType;

  // Accessors
  // =========

  const DataTy &x() const { return m_tuple[0]; }
  const DataTy &y() const { return m_tuple[1]; }
  const DataTy &z() const { return m_tuple[2]; }
  const DataTy &w() const { return m_tuple[3]; }

  DataTy &operator[](size_t index) { return m_tuple[index]; }
  const DataTy &operator[](size_t index) const { return m_tuple[index]; }

  bool isPoint() const { return m_tuple[3] == 1.0; }
  bool isVector() const { return m_tuple[3] == 0.0; }

  size_t size() const { return m_tuple.size(); }

  // Advanced vectors properties
  // ===========================

  DataTy magnitude() const {
    DataTy result = 0.0;
    for (const DataTy &c : m_tuple)
      result += c * c;
    return std::sqrt(result);
  }

  RayTracerTuple<DataTy> normalize() const { return *this / magnitude(); }

  // Operators
  // =========

  // equal/not equal

  bool operator==(const RayTracerTuple<DataTy> &rhs) const {
    return close_to_equal(x(), rhs.x()) && close_to_equal(y(), rhs.y()) &&
           close_to_equal(z(), rhs.z()) && close_to_equal(w(), rhs.w());
  }
  bool operator!=(const RayTracerTuple<DataTy> &rhs) const {
    return !operator==(rhs);
  }

  // operations

  RayTracerTuple<DataTy> &operator+=(const RayTracerTuple<DataTy> &rhs) {
    for (unsigned i = 0; i < m_tuple.size(); i++)
      m_tuple[i] += rhs.m_tuple[i];
    return *this;
  }
  RayTracerTuple<DataTy> &operator-=(const RayTracerTuple<DataTy> &rhs) {
    for (unsigned i = 0; i < m_tuple.size(); i++)
      m_tuple[i] -= rhs.m_tuple[i];
    return *this;
  }
  RayTracerTuple<DataTy> &operator*=(const DataTy &rhs) {
    for (DataTy &c : m_tuple)
      c *= rhs;
    return *this;
  }
  RayTracerTuple<DataTy> &operator/=(const DataTy &rhs) {
    for (DataTy &c : m_tuple)
      c /= rhs;
    return *this;
  }

  RayTracerTuple<DataTy> operator-() const {
    return RayTracerTuple<DataTy>(-m_tuple[0], -m_tuple[1], -m_tuple[2],
                                  -m_tuple[3]);
  }

  // Advanced operations

  DataTy dot(const RayTracerTuple<DataTy> &rhs) const {
    DataTy result = 0;
    for (unsigned i = 0; i < m_tuple.size(); i++) {
      result += m_tuple[i] * rhs.m_tuple[i];
    }
    return result;
  }
  RayTracerTuple<DataTy> cross(const RayTracerTuple<DataTy> &rhs) const {
    return Vector(m_tuple[1] * rhs.m_tuple[2] - m_tuple[2] * rhs.m_tuple[1],
                  m_tuple[2] * rhs.m_tuple[0] - m_tuple[0] * rhs.m_tuple[2],
                  m_tuple[0] * rhs.m_tuple[1] - m_tuple[1] * rhs.m_tuple[0]);
  }

private:
  /** An array formated as following: Tuple(x, y, z, w). Args type:
   * float/double. */
  std::array<DataTy, 4> m_tuple;
};

// Other/External operators
// ========================

// Adding two Tuples/Points/Vector returns repectively a Tuple, a Point or a
// Vector.
/** Adding two Tuples/Vector4.*/
template <class DataTy>
inline RayTracerTuple<DataTy> operator+(const RayTracerTuple<DataTy> &lhs,
                                        const RayTracerTuple<DataTy> &rhs) {
  RayTracerTuple<DataTy> tmp = lhs;
  tmp += rhs;
  return tmp;
}

/** Substracting two Tuples/Vectors returns respectively a Tuple or a Vector.
 * Subtracting two Points returns a Vector.
 * Subtracting a Vector from a Point returns a Point. */
template <class DataTy>
inline RayTracerTuple<DataTy> operator-(const RayTracerTuple<DataTy> &lhs,
                                        const RayTracerTuple<DataTy> &rhs) {
  RayTracerTuple<DataTy> tmp = lhs;
  tmp -= rhs;
  return tmp;
}

/** Scalar multiplication. */
template <class DataTy>
inline RayTracerTuple<DataTy> operator*(const RayTracerTuple<DataTy> &lhs,
                                        const DataTy &rhs) {
  RayTracerTuple<DataTy> tmp = lhs;
  tmp *= rhs;
  return tmp;
}
template <class DataTy>
inline RayTracerTuple<DataTy> operator*(const DataTy &lhs,
                                        const RayTracerTuple<DataTy> &rhs) {
  RayTracerTuple<DataTy> tmp = rhs;
  tmp *= lhs;
  return tmp;
}

/** Scalar division. */
template <class DataTy>
inline RayTracerTuple<DataTy> operator/(const RayTracerTuple<DataTy> &lhs,
                                        const DataTy &rhs) {
  RayTracerTuple<DataTy> tmp = lhs;
  tmp /= rhs;
  return tmp;
}

// Function like operators
// =======================

template <class DataTy> DataTy magnitude(const RayTracerTuple<DataTy> &T) {
  return T.magnitude();
}

template <class DataTy>
RayTracerTuple<DataTy> normalize(const RayTracerTuple<DataTy> &T) {
  return T.normalize();
}

// Advanced operations
// ===================

/** Dot product. */
template <class DataTy>
inline DataTy dot(const RayTracerTuple<DataTy> &lhs,
                  const RayTracerTuple<DataTy> &rhs) {
  RayTracerTuple<DataTy> tmp = lhs;
  return tmp.dot(rhs);
}
template <class DataTy>
inline RayTracerTuple<DataTy> cross(const RayTracerTuple<DataTy> &lhs,
                                    const RayTracerTuple<DataTy> &rhs) {
  RayTracerTuple<DataTy> tmp = lhs;
  return tmp.cross(rhs);
}
// Renaming classes for external usage

using Tuple = RayTracerTuple<RayTracerDataType>;

/** Helper to create a Vectore, i.e. a Tuple with w = 0.0 */
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
