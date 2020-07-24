#pragma once

#include "ratrac/Tuple.h"

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

template <class DataTy>
Tuple position(const Ray &ray, DataTy t) {
  return ray.origin() + ray.direction() * Tuple::DataType(t);
}
} // namespace ratrac
