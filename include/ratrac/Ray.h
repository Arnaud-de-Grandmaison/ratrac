#pragma once

#include "ratrac/Matrice.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

#include <ostream>

namespace ratrac {
class Ray {
public:
  Ray() : m_origin(), m_direction() {}
  Ray(const Tuple &origin, const Tuple &direction)
      : m_origin(origin), m_direction(direction) {}
  Ray(const Ray &other)
      : m_origin(other.m_origin), m_direction(other.m_direction) {}

  const Tuple &origin() const { return m_origin; }
  const Tuple &direction() const { return m_direction; }

private:
  Tuple m_origin;
  Tuple m_direction;
};

template <class DataTy> Tuple position(const Ray &ray, DataTy t) {
  return ray.origin() + ray.direction() * Tuple::DataType(t);
}

inline Ray transform(const Ray &ray, const Matrice &mat) {
  return Ray(mat * ray.origin(), mat * ray.direction());
}

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Ray &ray);
