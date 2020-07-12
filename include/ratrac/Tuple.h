#pragma once

#include <array>
#include <cmath>

namespace ratrac {
// Classes
// =======

/** RayTraceTuple represent the Tuples ratrac uses to manipulate point and
 * vectors. */
template <class DataTy> class RayTracerTuple {
public:
  /** As shown here, Type must not be error or 0. It is a Point if w == 1 and a
   * Vector if w == 0. */
  enum class Type {
    error = 0,
    point = 1,
    vector = 2,
  };

  /** Initialise the Tuple/Vector4.*/
  RayTracerTuple(DataTy x, DataTy y, DataTy z, DataTy w)
      : m_tuple{x, y, z, w} {}

  // Accessors
  // =========

  const DataTy &x() const { return m_tuple[0]; }
  const DataTy &y() const { return m_tuple[1]; }
  const DataTy &z() const { return m_tuple[2]; }
  const DataTy &w() const { return m_tuple[3]; }
  Type type() const {
    if (m_tuple[3] == 1.0) {
      return Type::point;
    } else if (m_tuple[3] == 0.0) {
      return Type::vector;
    } else {
      return Type::error;
    }
  }

  // Advanced vectors properties
  // ===========================

  // #help: DataTy sould be rather a double or a float ?
  const double magnitude() const {
    return std::sqrt(m_tuple[0] * m_tuple[0] + m_tuple[1] * m_tuple[1] +
                     m_tuple[2] * m_tuple[2] + m_tuple[3] * m_tuple[3]);
    // #help: Which is better ?
    /*
    return std::sqrt(std::pow(m_tuple[0], 2) + std::pow(m_tuple[1], 2) +
                     std::pow(m_tuple[2], 2) + std::pow(m_tuple[3], 2));*/
  }

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

private:
  /** An array formated as following: Tuple(x, y, z, w). Args type:
   * float/double. */
  std::array<DataTy, 4> m_tuple;

  /** Tests if two coordinates are approximalty equal. The difference must be
   * less than EPSILON. Currently, EPSILON = 0.00001. */
  static bool close_to_equal(const DataTy &a, const DataTy &b) {
    const DataTy EPSILON = 0.00001;
    return std::abs(a - b) < EPSILON;
  }
};

/** Helper to create a Vectore, i.e. a Tuple with w = 0.0 */
template <class DataTy>
RayTracerTuple<DataTy> Vector(const DataTy &x, const DataTy &y,
                              const DataTy &z) {
  return RayTracerTuple<DataTy>(x, y, z, 0.0);
}

/** Helper to create a Point, i.e. a Tuple with w = 1.0 */
template <class DataTy>
RayTracerTuple<DataTy> Point(const DataTy &x, const DataTy &y,
                             const DataTy &z) {
  return RayTracerTuple<DataTy>(x, y, z, 1.0);
}

// Other/External operators
// ========================

// Adding two Tuples/Points/Vector returns repectively a Tuple, a Point or a
// Vector.
/** Adding two Tuples/Vector4.*/
template <class DataTy>
RayTracerTuple<DataTy> operator+(const RayTracerTuple<DataTy> &lhs,
                                 const RayTracerTuple<DataTy> &rhs) {
  RayTracerTuple<DataTy> tmp = lhs;
  tmp += rhs;
  return tmp;
}

/** Substracting two Tuples/Vectors returns respectively a Tuple or a Vector.
 * Subtracting two Points returns a Vector.
 * Subtracting a Vector from a Point returns a Point. */
template <class DataTy>
RayTracerTuple<DataTy> operator-(const RayTracerTuple<DataTy> &lhs,
                                 const RayTracerTuple<DataTy> &rhs) {
  RayTracerTuple<DataTy> tmp = lhs;
  tmp -= rhs;
  return tmp;
}

/** Scalar multiplication. */
template <class DataTy>
RayTracerTuple<DataTy> operator*(const RayTracerTuple<DataTy> &lhs,
                                 const DataTy &rhs) {
  RayTracerTuple<DataTy> tmp = lhs;
  tmp *= rhs;
  return tmp;
}
template <class DataTy>
RayTracerTuple<DataTy> operator*(const DataTy &lhs,
                                 const RayTracerTuple<DataTy> &rhs) {
  RayTracerTuple<DataTy> tmp = rhs;
  tmp *= lhs;
  return tmp;
}

/** Scalar division. */
template <class DataTy>
RayTracerTuple<DataTy> operator/(const RayTracerTuple<DataTy> &lhs,
                                 const DataTy &rhs) {
  RayTracerTuple<DataTy> tmp = lhs;
  tmp /= rhs;
  return tmp;
}

// Renaming classes for external usage

/** Current supported types: double, float.
 * Note: Integers are not supported because of EPSILON. */
using RayTracerDataType = double;
// using Point = RayTracerPoint<RayTracerDataType>;
// using Vector = RayTracerVector<RayTracerDataType>;
using Tuple = RayTracerTuple<RayTracerDataType>;

} // namespace ratrac