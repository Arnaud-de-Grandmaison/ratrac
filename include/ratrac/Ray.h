#pragma once

#include "ratrac/Matrix.h"
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

  constexpr const Tuple &origin() const noexcept { return m_origin; }
  constexpr const Tuple &direction() const noexcept { return m_direction; }

private:
  Tuple m_origin;
  Tuple m_direction;
};

template <class DataTy>
constexpr Tuple position(const Ray &ray, DataTy t) noexcept {
  return ray.origin() + ray.direction() * Tuple::DataType(t);
}

inline Ray transform(const Ray &ray, const Matrix &mat) {
  return Ray(mat * ray.origin(), mat * ray.direction());
}

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Ray &ray);
