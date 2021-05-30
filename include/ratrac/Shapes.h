#pragma once

#include "ratrac/Color.h"
#include "ratrac/Intersections.h"
#include "ratrac/Material.h"
#include "ratrac/Ray.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

#include <ostream>
#include <string>

namespace ratrac {
class Shape {
public:
  Shape() : m_transform(Matrice::identity()), m_material() {}
  virtual ~Shape();

  bool operator==(const Shape &rhs) const {
    return m_transform == rhs.m_transform && m_material == rhs.m_material;
  }
  bool operator!=(const Shape &rhs) const { return !(*this == rhs); }

  const Matrice &transform() const { return m_transform; }
  const Material &material() const { return m_material; }
  Material &material() { return m_material; }

  Shape &material(const Material &m) {
    m_material = m;
    return *this;
  }

  Shape &transform(const Matrice &M) {
    m_transform = M;
    return *this;
  }

  Shape &transform(Matrice &&M) {
    m_transform = std::move(M);
    return *this;
  }

  Intersections intersect(const Ray &world_ray) const {
    Ray local_ray = ratrac::transform(world_ray, inverse(m_transform));
    return local_intersect(local_ray);
  }

  Color at(const Tuple &world_point) const {
    Tuple object_point = inverse(m_transform) * world_point;
    return m_material.at(object_point);
  }

  Tuple normal_at(const Tuple &world_point) const {
    Tuple local_point = inverse(m_transform) * world_point;
    Tuple local_normal = local_normal_at(local_point);
    Tuple world_normal = transpose(inverse(m_transform)) * local_normal;
    world_normal[3] = 0;
    return world_normal.normalize();
  }

  virtual Intersections local_intersect(const Ray &ray) const = 0;
  virtual Tuple local_normal_at(const Tuple &point) const = 0;

  virtual explicit operator std::string() const { return std::string(); }

private:
  Matrice m_transform;
  Material m_material;
};

class Sphere : public Shape {
public:
  Sphere() : Shape(), m_center(Point(0, 0, 0)), m_radius(1.0) {}

  Sphere(const Sphere &) = default;
  Sphere(Sphere &&) = default;

  Sphere &operator=(const Sphere &) = default;
  Sphere &operator=(Sphere &&) = default;

  const Tuple &center() const { return m_center; }
  const RayTracerDataType &radius() const { return m_radius; }

  bool operator==(const Sphere &rhs) const {
    return Shape::operator==(rhs) && m_center == rhs.m_center &&
           m_radius == rhs.m_radius;
  }
  bool operator!=(const Sphere &rhs) const { return !(*this == rhs); }

  virtual Intersections local_intersect(const Ray &r) const override;

  virtual Tuple local_normal_at(const Tuple &local_point) const override {
    return Vector(local_point.x() - m_center.x(),
                  local_point.y() - m_center.y(),
                  local_point.z() - m_center.z());
  }

  virtual explicit operator std::string() const override;

private:
  Tuple m_center;
  RayTracerDataType m_radius;
};

// An XZ plane.
class Plane : public Shape {
public:
  Plane() : Shape() {}

  Plane(const Plane &) = default;
  Plane(Plane &&) = default;

  Plane &operator=(const Plane &) = default;
  Plane &operator=(Plane &&) = default;

  bool operator==(const Plane &rhs) const { return true; }
  bool operator!=(const Plane &rhs) const { return false; }

  virtual Intersections local_intersect(const Ray &r) const override;

  virtual Tuple local_normal_at(const Tuple &local_point) const override {
    return Vector(0, 1, 0);
  }

  virtual explicit operator std::string() const override;
};

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Shape &s);
