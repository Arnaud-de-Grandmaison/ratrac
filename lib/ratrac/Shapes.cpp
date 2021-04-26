#include "ratrac/Shapes.h"
#include "ratrac/Intersections.h"
#include "ratrac/ratrac.h"

//#include <iomanip>
#include <sstream>

namespace ratrac {
Shape::~Shape() {}

Intersections Sphere::local_intersect(const Ray &r) const {
  Tuple sphere_to_ray = r.origin() - center();
  Tuple::DataType a = dot(r.direction(), r.direction());
  Tuple::DataType b = 2.0 * dot(r.direction(), sphere_to_ray);
  Tuple::DataType c = dot(sphere_to_ray, sphere_to_ray) - 1.0;
  Tuple::DataType discriminant = b * b - 4.0 * a * c;
  if (discriminant < 0.0)
    return Intersections();

  Tuple::DataType t1 = (-b - sqrt(discriminant)) / (2.0 * a);
  Tuple::DataType t2 = (-b + sqrt(discriminant)) / (2.0 * a);
  return Intersections(Intersection(t1, this), Intersection(t2, this));
}

Sphere::operator std::string() const {
  std::ostringstream os;
  os << "Sphere {";
  os << " center: " << center();
  os << ", radius: " << radius();
  os << ", transform: " << transform();
  os << ", material: " << material();
  os << "}";
  return os.str();
}

Intersections Plane::local_intersect(const Ray &r) const {
  // If the ray is parallel to the plane, then there is no intersection.
  if (std::fabs(r.direction().y()) < EPSILON<Tuple::DataType>())
    return Intersections();

  Tuple::DataType t = -r.origin().y() / r.direction().y();
  return Intersections(Intersection(t, this));
}

Plane::operator std::string() const {
  std::ostringstream os;
  os << "Plane {";
  os << " transform: " << transform();
  os << ", material: " << material();
  os << "}";
  return os.str();
}

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Shape &s) {
  os << s.operator std::string();
  return os;
}
