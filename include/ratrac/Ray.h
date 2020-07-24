#pragma once

#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

#include <cassert>

namespace ratrac {
class Ray {
public:
  Ray(const Tuple &origin, const Tuple &direction)
      : m_origin(origin), m_direction(direction) {}

  const Tuple &origin() const { return m_origin; }
  const Tuple &direction() const { return m_direction; }

private:
  Tuple m_origin;
  Tuple m_direction;
};

template <class DataTy> Tuple position(const Ray &ray, DataTy t) {
  return ray.origin() + ray.direction() * Tuple::DataType(t);
}

class Intersection {
public:
  Intersection() : m_count(0), m_xs{}  {}
  Intersection(RayTracerDataType xs0) : m_count(1), m_xs{xs0, 0.} {}
  Intersection(RayTracerDataType xs0, RayTracerDataType xs1)
      : m_count(2), m_xs{xs0, xs1} {}

  unsigned count() const { return m_count; }

  const RayTracerDataType &operator[](unsigned i) const {
    assert(i < m_count && "Out of bounds access");
    return m_xs[i];
  }
  RayTracerDataType &operator[](unsigned i) {
    assert(i < m_count && "Out of bounds access");
    return m_xs[i];
  }

private:
  unsigned m_count;
  RayTracerDataType m_xs[2];
};

} // namespace ratrac
