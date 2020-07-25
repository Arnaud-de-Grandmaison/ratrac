#pragma once

#include "ratrac/Ray.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

namespace ratrac {
class Sphere {
public:
  Sphere() : m_center(Point(0, 0, 0)), m_radius(1.0) {}

  const Tuple &center() const { return m_center; }
  const RayTracerDataType &radius() const { return m_radius; }

private:
  Tuple m_center;
  RayTracerDataType m_radius;
};

struct Intersection {
  Intersection() : t(RayTracerDataType()), object(nullptr) {}
  Intersection(RayTracerDataType t, const Sphere *object)
      : t(t), object(object) {}
  Intersection(RayTracerDataType t, const Sphere &object)
      : t(t), object(&object) {}
  RayTracerDataType t;
  const Sphere *object;
};

class Intersections {
public:
  Intersections() : m_count(0), m_xs{} {}
  explicit Intersections(Intersection xs0) : m_count(1), m_xs{xs0} {}
  Intersections(Intersection xs0, Intersection xs1)
      : m_count(2), m_xs{xs0, xs1} {}

  unsigned count() const { return m_count; }

  const Intersection &operator[](unsigned i) const {
    assert(i < m_count && "Out of bounds access");
    return m_xs[i];
  }
  Intersection &operator[](unsigned i) {
    assert(i < m_count && "Out of bounds access");
    return m_xs[i];
  }

private:
  unsigned m_count;
  Intersection m_xs[2];
};

Intersections intersect(const Sphere &s, const Ray &r);

} // namespace ratrac
