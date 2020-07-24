#include "ratrac/Shapes.h"
#include "ratrac/Ray.h"
#include "ratrac/ratrac.h"

#include <cmath>

using std::sqrt;

namespace ratrac {
Intersection intersect(const Sphere &s, const Ray &r) {
  Tuple sphere_to_ray = r.origin() - s.center();
  RayTracerDataType a = dot(r.direction(), r.direction());
  RayTracerDataType b = 2.0 * dot(r.direction(), sphere_to_ray);
  RayTracerDataType c = dot(sphere_to_ray, sphere_to_ray) - 1.0;
  RayTracerDataType discriminant = b * b - 4.0 * a * c;
  if (discriminant < 0.0)
    return Intersection();

  RayTracerDataType t1 = (-b - sqrt(discriminant)) / (2.0 * a);
  RayTracerDataType t2 = (-b + sqrt(discriminant)) / (2.0 * a);
  return Intersection(t1, t2);
}
} // namespace ratrac
