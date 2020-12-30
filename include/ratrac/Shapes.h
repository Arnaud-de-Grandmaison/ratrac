#pragma once

#include "ratrac/Material.h"
#include "ratrac/Ray.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

#include <algorithm>
#include <vector>

namespace ratrac {
class Sphere {
public:
  Sphere()
      : m_transform(Matrice::identity()), m_material(),
        m_center(Point(0, 0, 0)), m_radius(1.0) {}

  const Matrice &transform() const { return m_transform; }
  const Material &material() const { return m_material; }
  const Tuple &center() const { return m_center; }
  const RayTracerDataType &radius() const { return m_radius; }

  Sphere &material(const Material &m) {
    m_material = m;
    return *this;
  }

  Sphere &transform(const Matrice &M) {
    m_transform = M;
    return *this;
  }
  Sphere &transform(Matrice &&M) {
    m_transform = std::move(M);
    return *this;
  }

  Tuple normal_at(const Tuple &world_point) const {
    Tuple object_point = inverse(m_transform) * world_point;
    Tuple object_normal =
        Vector(object_point.x() - m_center.x(), object_point.y() - m_center.y(),
               object_point.z() - m_center.z());
    Tuple world_normal = transpose(inverse(m_transform)) * object_normal;
    world_normal[3] = 0;
    return world_normal.normalize();
  }

private:
  Matrice m_transform;
  Material m_material;
  Tuple m_center;
  RayTracerDataType m_radius;
};

struct Intersection {
  Intersection() : t(RayTracerDataType()), object(nullptr) {}
  Intersection(const Intersection &) = default;
  Intersection(RayTracerDataType t, const Sphere *object)
      : t(t), object(object) {}
  Intersection(RayTracerDataType t, const Sphere &object)
      : t(t), object(&object) {}

  Intersection &operator=(const Intersection &) = default;

  bool operator<(const Intersection &rhs) const { return t < rhs.t; }

  bool operator==(const Intersection &Other) const {
    return t == Other.t && object == Other.object;
  }
  bool operator!=(const Intersection &Other) const {
    return t != Other.t || object != Other.object;
  }

  RayTracerDataType t;
  const Sphere *object;
};

class Intersections {
public:
  Intersections() : m_xs() {}
  explicit Intersections(const Intersection &x0) : m_xs{x0} {}
  Intersections(const Intersection &x0, const Intersection &x1) : m_xs{x0, x1} {
    // Ensure our Intersection list is sorted.
    if (m_xs[0].t > m_xs[1].t)
      std::swap(m_xs[0], m_xs[1]);
  }

  typedef std::vector<Intersection>::iterator iterator;
  typedef std::vector<Intersection>::const_iterator const_iterator;

  iterator begin() { return m_xs.begin(); }
  iterator end() { return m_xs.end(); }
  const_iterator begin() const { return m_xs.begin(); }
  const_iterator end() const { return m_xs.end(); }

  Intersections &add(Intersection &x) {
    // Keep our list sorted upon insertion of a new Intersection.
    iterator it = std::lower_bound(begin(), end(), x);
    m_xs.insert(it, x);
    return *this;
  }

  unsigned count() const { return m_xs.size(); }
  bool empty() const { return m_xs.empty(); }

  const Intersection &operator[](unsigned i) const {
    assert(i < m_xs.size() && "Out of bounds access");
    return m_xs[i];
  }
  Intersection &operator[](unsigned i) {
    assert(i < m_xs.size() && "Out of bounds access");
    return m_xs[i];
  }

  const_iterator hit() const {
    return std::find_if(begin(), end(), [&](const Intersection &x) -> bool {
      return x.t >= 0.0;
    });
  }

private:
  std::vector<Intersection> m_xs;
};

Intersections intersect(const Sphere &s, const Ray &r);

} // namespace ratrac
