#pragma once

#include <iostream>
#include <string>

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
  bool operator==(const RayTracerTuple &rhs) const {
    return x() == rhs.x() && y() == rhs.y() && z() == rhs.z() && w() == rhs.w();
  }
  bool operator!=(const RayTracerTuple &rhs) const {
    return ! operator==(rhs);
  }

private:
  DataTy m_tuple[4];
};

template <class DataTy = RayTracerDataType>
class RayTracerPoint : public RayTracerTuple<DataTy> {
public:
  RayTracerPoint(DataTy x, DataTy y, DataTy z) : RayTracerTuple(x, y, z, 1.0) {}


};

using Point = RayTracerPoint<RayTracerDataType>;
using Tuple = RayTracerTuple<RayTracerDataType>;

} // namespace ratrac
