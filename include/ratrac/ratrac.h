#pragma once

#include <iostream>
#include <string>
#include <cmath>

namespace ratrac {
class World {
public:
  void say(std::ostream &os, const std::string &msg) const;
};

using RayTracerDataType = double;

template <class DataTy = RayTracerDataType> class RayTracerTuple {
public:
  enum class Type {
    error = 0,
    point = 1,
    vector = 2,
  };

  RayTracerTuple(DataTy x, DataTy y, DataTy z, DataTy w)
      : m_tuple{x, y, z, w} {}

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

  bool operator==(const RayTracerTuple<DataTy> &rhs) const {
    return close_to_equal(x(), rhs.x()) && close_to_equal(y(), rhs.y()) &&
           close_to_equal(z(), rhs.z()) && close_to_equal(w(), rhs.w());
  }
  bool operator!=(const RayTracerTuple<DataTy> &rhs) const {
    return !operator==(rhs);
  }

  RayTracerTuple<DataTy> &operator+=(const RayTracerTuple<DataTy> &rhs) {
    m_tuple[0] += rhs.x();
    m_tuple[1] += rhs.y();
    m_tuple[2] += rhs.z();
    m_tuple[3] += rhs.w();
    return *this;
  }

private:
  DataTy m_tuple[4];

  static bool close_to_equal(const RayTracerDataType &a,
                             const RayTracerDataType &b) {
    const RayTracerDataType EPSILON = 0.00001;
    return std::abs(a - b) < EPSILON;
  }
};
template <class DataTy = RayTracerDataType>
RayTracerTuple<DataTy> operator+(const RayTracerTuple<DataTy> &lhs,
                                 const RayTracerTuple<DataTy> &rhs) {
  RayTracerTuple<DataTy> tmp = lhs;
  tmp += rhs;
  return tmp;
}

template <class DataTy = RayTracerDataType>
class RayTracerPoint : public RayTracerTuple<DataTy> {
public:
  RayTracerPoint(DataTy x, DataTy y, DataTy z)
      : RayTracerTuple<DataTy>(x, y, z, 1.0) {}
};

template <class DataTy = RayTracerDataType>
class RayTracerVector : public RayTracerTuple<DataTy> {
public:
  RayTracerVector(DataTy x, DataTy y, DataTy z)
      : RayTracerTuple<DataTy>(x, y, z, 0.0) {}
};

using Point = RayTracerPoint<RayTracerDataType>;
using Vector = RayTracerVector<RayTracerDataType>;
using Tuple = RayTracerTuple<RayTracerDataType>;

} // namespace ratrac
