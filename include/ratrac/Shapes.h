#pragma once

#include "ratrac/Material.h"
#include "ratrac/Ray.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"


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

  bool operator==(const Sphere &rhs) const {
    return m_transform == rhs.m_transform && m_material == rhs.m_material &&
           m_center == rhs.m_center && m_radius == rhs.m_radius;
  }
  bool operator!=(const Sphere &rhs) const {
    return !(*this == rhs);
  }

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

} // namespace ratrac
