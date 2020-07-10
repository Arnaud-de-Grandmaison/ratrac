#pragma once

#include <iostream>
#include <string>
#include <cmath>

namespace ratrac {
// Classes
// =======

/** Useless class, mainly used for testing on project initialisation. */
class World {
public:
  void say(std::ostream &os, const std::string &msg) const;
};

/** Current supported types: double, float. (Integers are not supported because EPSILON.) */
using RayTracerDataType = double;

/** Parent class of RayTracerVector(x, y, z, 0) and RayTracerPoint(x, y, z, 1).*/
template <class DataTy = RayTracerDataType> class RayTracerTuple {
public:

  /** As shown here, Type must not be error or 0. It is a Point if w == 1 and a Vector if w == 0. */
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
    m_tuple[0] += rhs.x();
    m_tuple[1] += rhs.y();
    m_tuple[2] += rhs.z();
    m_tuple[3] += rhs.w();
    return *this;
  }
  RayTracerTuple<DataTy> &operator-=(const RayTracerTuple<DataTy> &rhs) {
    m_tuple[0] -= rhs.x();
    m_tuple[1] -= rhs.y();
    m_tuple[2] -= rhs.z();
    m_tuple[3] -= rhs.w();
    return *this;
  };

protected:
  /** An array formated as following: Tuple(x, y, z, w). Args type: float/double. */
  DataTy m_tuple[4];

private:
  /** Tests if two coordinates are approximalty equal. The difference must be less than EPSILON. Currently, EPSILON = 0.00001. */
  static bool close_to_equal(const RayTracerDataType &a,
                             const RayTracerDataType &b) {
    const RayTracerDataType EPSILON = 0.00001;
    return std::abs(a - b) < EPSILON;
  }
};

/** A class representing a Vector3 or a Vector4 whre w = 0. */
template <class DataTy = RayTracerDataType>
class RayTracerVector : public RayTracerTuple<DataTy> {
public:
  /** RayTracerVector = {x, y, z, 0} */
  RayTracerVector(DataTy x, DataTy y, DataTy z)
      : RayTracerTuple<DataTy>(x, y, z, 0.0) {}
  
  /** Adding two Vectors returns a Vector. */
  RayTracerVector<DataTy> &operator+=(const RayTracerVector<DataTy> &rhs) {
    m_tuple[0] += rhs.x();
    m_tuple[1] += rhs.y();
    m_tuple[2] += rhs.z();
    return *this;
  }
  /** A subtraction of two Vectors returns a Vector. */
  RayTracerVector<DataTy> &operator-=(const RayTracerVector<DataTy> &rhs) {
    m_tuple[0] -= rhs.x();
    m_tuple[1] -= rhs.y();
    m_tuple[2] -= rhs.z();
    m_tuple[3] -= rhs.w();
    return *this;
  }
};

/** A class representing a Point or a Vector4 where w = 1. */
template <class DataTy = RayTracerDataType>
class RayTracerPoint : public RayTracerTuple<DataTy> {
public:
  /** RayTracerPoint = {x, y, z, 1} */
  RayTracerPoint(DataTy x, DataTy y, DataTy z)
      : RayTracerTuple<DataTy>(x, y, z, 1.0) {}

  // /* Adding two Points does not really make sense. However, it will returns the point which is in middle of both. */
  //RayTracerVector<DataTy> &operator+=(const RayTracerVector<DataTy> &rhs) {
  //  m_tuple[0] += rhs.x();
  //  m_tuple[1] += rhs.y();
  //  m_tuple[2] += rhs.z();
  //  return *this;
  //}

  /** Subtracting a Vector from a Point returns a Point. */
  RayTracerPoint<DataTy> &operator-=(const RayTracerVector<DataTy> &rhs) {
    m_tuple[0] -= rhs.x();
    m_tuple[1] -= rhs.y();
    m_tuple[2] -= rhs.z();
    m_tuple[3] -= rhs.w();
    return *this;
  }
};

// Other/External operators
// ========================

// Adding two Tuples/Points/Vector returns repectively a Tuple, a Point or a
// Vector.
// However adding two points does not really make sense, actually, it will return the middle between both.
/** Adding two Tuples/Points/Vector returns a Tuple (because the Tests of the
book does not required any other kind of return Type).*/
template <class DataTy = RayTracerDataType,
          class Obj1Kind = RayTracerTuple<DataTy>,
          class Obj2Kind = RayTracerTuple<DataTy>>
RayTracerTuple<DataTy>  operator+(const Obj1Kind &lhs,
                                  const Obj2Kind &rhs) {
  RayTracerTuple<DataTy> tmp = lhs;
  tmp += rhs;
  return tmp;
}

/** Subtracting two Points returns a Vector. */
template <class DataTy = RayTracerDataType>
RayTracerVector<DataTy> operator-(const RayTracerPoint<DataTy> &lhs,
                                  const RayTracerPoint<DataTy> &rhs) {
  return RayTracerVector<DataTy>(lhs.x() - rhs.x(), lhs.y() - rhs.y(),
                                 lhs.z() - rhs.z());
}

/** Subtracting a Vector from a Point returns a Point. */
template <class DataTy = RayTracerDataType>
RayTracerPoint<DataTy> operator-(const RayTracerPoint<DataTy> &lhs,
                                 const RayTracerVector<DataTy> &rhs) {
  RayTracerPoint<DataTy> tmp = lhs;
  tmp -= rhs;
  return tmp;
}



/** Substracting two Tuples/Vectors returns respectively a Tuple or a Vector./n
WARNING: May makes trouble with RayTracerVector<DataTy>
operator-(const RayTracerPoint<DataTy> &lhs, const RayTracerPoint<DataTy> &rhs)*/
template <class DataTy = RayTracerDataType, class ObjKind = RayTracerTuple<DataTy>>
ObjKind operator-(const ObjKind &lhs,
                  const ObjKind &rhs) {
  ObjKind tmp = lhs;
  tmp -= rhs;
  return tmp;
}

// Renaming classes for external usage
using Point = RayTracerPoint<RayTracerDataType>;
using Vector = RayTracerVector<RayTracerDataType>;
using Tuple = RayTracerTuple<RayTracerDataType>;

} // namespace ratrac
