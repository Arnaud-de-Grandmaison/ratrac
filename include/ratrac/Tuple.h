#pragma once

#include "ratrac/ratrac.h"

#include <cassert>
#include <cmath>
#include <ostream>

namespace ratrac {
// Classes
// =======

/** RayTracerTuple represent the Tuples ratrac uses to manipulate point and
 * vectors. */
class Tuple {
  static const unsigned NC = 4; // Number of components in the Tuple.
  static const unsigned X_idx = 0;
  static const unsigned Y_idx = 1;
  static const unsigned Z_idx = 2;
  static const unsigned W_idx = 3;
  static_assert(std::is_floating_point<ratrac::RayTracerDataType>::value,
                "Tuple DataTy must be a floating point type.");

public:
  typedef ratrac::RayTracerDataType DataType;

  /** Initialise the Tuple/Vector4.*/
  Tuple() {}

  template <class XTy, class YTy, class ZTy, class WTy>
  constexpr Tuple(XTy x, YTy y, ZTy z, WTy w)
      : m_tuple{DataType(x), DataType(y), DataType(z), DataType(w)} {}

  /** Helpers to create points and vectors. */
  template <class XTy, class YTy, class ZTy>
  static constexpr Tuple Vector(XTy x, YTy y, ZTy z) {
    return Tuple(x, y, z, DataType(0.0));
  }

  template <class XTy, class YTy, class ZTy>
  static constexpr Tuple Point(XTy x, YTy y, ZTy z) {
    return Tuple(x, y, z, DataType(1.0));
  }

  // Accessors
  // =========

  constexpr const DataType &x() const noexcept { return m_tuple[X_idx]; }
  constexpr const DataType &y() const noexcept { return m_tuple[Y_idx]; }
  constexpr const DataType &z() const noexcept { return m_tuple[Z_idx]; }
  constexpr const DataType &w() const noexcept { return m_tuple[W_idx]; }

  constexpr DataType &operator[](size_t index) noexcept {
    assert(index < NC && "out of bounds access in Tuple");
    return m_tuple[index];
  }
  constexpr const DataType &operator[](size_t index) const noexcept {
    assert(index < NC && "out of bounds access in Tuple");
    return m_tuple[index];
  }

  constexpr bool isPoint() const noexcept { return m_tuple[W_idx] == 1.0; }
  constexpr bool isVector() const noexcept { return m_tuple[W_idx] == 0.0; }

  constexpr size_t size() const noexcept { return 4; }

  // Advanced vectors properties
  // ===========================

  DataType magnitude() const {
    DataType result = DataType();
    for (size_t i = 0; i < NC; i++)
      result += m_tuple[i] * m_tuple[i];
    return std::sqrt(result);
  }

  Tuple &normalize() {
    *this /= magnitude();
    return *this;
  }

  // Operators
  // =========

  // equal/not equal

  constexpr bool operator==(const Tuple &rhs) const {
    return close_to_equal(x(), rhs.x()) && close_to_equal(y(), rhs.y()) &&
           close_to_equal(z(), rhs.z()) && close_to_equal(w(), rhs.w());
  }
  constexpr bool operator!=(const Tuple &rhs) const { return !operator==(rhs); }

  // operations

  constexpr Tuple &operator+=(const Tuple &rhs) noexcept {
    for (size_t i = 0; i < NC; i++)
      m_tuple[i] += rhs.m_tuple[i];
    return *this;
  }
  constexpr Tuple &operator-=(const Tuple &rhs) noexcept {
    for (size_t i = 0; i < NC; i++)
      m_tuple[i] -= rhs.m_tuple[i];
    return *this;
  }
  constexpr Tuple &operator*=(DataType rhs) noexcept {
    for (size_t i = 0; i < NC; i++)
      m_tuple[i] *= rhs;
    return *this;
  }
  constexpr Tuple &operator/=(DataType rhs) noexcept {
    for (size_t i = 0; i < NC; i++)
      m_tuple[i] /= rhs;
    return *this;
  }

  constexpr Tuple operator-() const noexcept {
    return Tuple(-m_tuple[0], -m_tuple[1], -m_tuple[2], -m_tuple[3]);
  }

  // Advanced operations

  constexpr DataType dot(const Tuple &rhs) const noexcept {
    DataType result = DataType();
    for (size_t i = 0; i < NC; i++)
      result += m_tuple[i] * rhs.m_tuple[i];
    return result;
  }

  constexpr Tuple cross(const Tuple &rhs) const noexcept {
    return Vector(m_tuple[Y_idx] * rhs.m_tuple[Z_idx] -
                      m_tuple[Z_idx] * rhs.m_tuple[Y_idx],
                  m_tuple[Z_idx] * rhs.m_tuple[X_idx] -
                      m_tuple[X_idx] * rhs.m_tuple[Z_idx],
                  m_tuple[X_idx] * rhs.m_tuple[Y_idx] -
                      m_tuple[Y_idx] * rhs.m_tuple[X_idx]);
  }

  constexpr Tuple reflect(const Tuple &normal) const noexcept {
    Tuple n = normal;
    n *= 2.0 * dot(normal);
    Tuple tmp(*this);
    tmp -= n;
    return tmp;
  }

private:
  DataType m_tuple[4];
};

// Other/External operators
// ========================

// Adding two Tuples/Points/Vector returns respectively a Tuple, a Point or a
// Vector.
/** Adding two Tuples/Vector4.*/
inline constexpr Tuple operator+(const Tuple &lhs, const Tuple &rhs) noexcept {
  Tuple tmp = lhs;
  tmp += rhs;
  return tmp;
}

/** Substracting two Tuples/Vectors returns respectively a Tuple or a Vector.
 * Subtracting two Points returns a Vector.
 * Subtracting a Vector from a Point returns a Point. */
inline constexpr Tuple operator-(const Tuple &lhs, const Tuple &rhs) noexcept {
  Tuple tmp = lhs;
  tmp -= rhs;
  return tmp;
}

/** Scalar multiplication. */
inline constexpr Tuple operator*(const Tuple &lhs,
                                 Tuple::DataType rhs) noexcept {
  Tuple tmp = lhs;
  tmp *= rhs;
  return tmp;
}
inline constexpr Tuple operator*(Tuple::DataType lhs,
                                 const Tuple &rhs) noexcept {
  Tuple tmp = rhs;
  tmp *= lhs;
  return tmp;
}

/** Scalar division. */
inline constexpr Tuple operator/(const Tuple &lhs,
                                 Tuple::DataType rhs) noexcept {
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
inline constexpr Tuple::DataType dot(const Tuple &lhs,
                                     const Tuple &rhs) noexcept {
  Tuple tmp = lhs;
  return tmp.dot(rhs);
}

/** Dot product. */
inline constexpr Tuple cross(const Tuple &lhs, const Tuple &rhs) noexcept {
  Tuple tmp = lhs;
  return tmp.cross(rhs);
}

/** Reflect a vector. */
inline constexpr Tuple reflect(const Tuple &in, const Tuple &normal) noexcept {
  Tuple tmp = in;
  return tmp.reflect(normal);
}

/** Helper to create a Vector, i.e. a Tuple with w = 0.0 */
template <class XTy, class YTy, class ZTy>
inline constexpr Tuple Vector(XTy x, YTy y, ZTy z) noexcept {
  return Tuple::Vector(x, y, z);
}

/** Helper to create a Point, i.e. a Tuple with w = 1.0 */
template <class XTy, class YTy, class ZTy>
inline constexpr Tuple Point(XTy x, YTy y, ZTy z) noexcept {
  return Tuple::Point(x, y, z);
}

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Tuple &T);
