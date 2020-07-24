#pragma once

#include "ratrac/Tuple.h"
#include "ratrac/Ray.h"
#include "ratrac/ratrac.h"

namespace ratrac {
class Sphere {
public:
  Sphere() : m_center(Point(0,0,0)), m_radius(1.0) {}

  const Tuple &center() const { return m_center; }
  const RayTracerDataType &radius() const { return m_radius; }

private:
  Tuple m_center;
  RayTracerDataType m_radius;
};

Intersection intersect(const Sphere &s, const Ray &r);

} // namespace ratrac
