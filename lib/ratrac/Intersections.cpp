#include "ratrac/Shapes.h"

#include "ratrac/Intersections.h"
#include "ratrac/Ray.h"
#include "ratrac/ratrac.h"

#include <cmath>

using std::sqrt;

namespace ratrac {

Intersections intersect(const Sphere &s, const Ray &r) {
  Ray ray = transform(r, inverse(s.transform()));
  Tuple sphere_to_ray = ray.origin() - s.center();
  RayTracerDataType a = dot(ray.direction(), ray.direction());
  RayTracerDataType b = 2.0 * dot(ray.direction(), sphere_to_ray);
  RayTracerDataType c = dot(sphere_to_ray, sphere_to_ray) - 1.0;
  RayTracerDataType discriminant = b * b - 4.0 * a * c;
  if (discriminant < 0.0)
    return Intersections();

  RayTracerDataType t1 = (-b - sqrt(discriminant)) / (2.0 * a);
  RayTracerDataType t2 = (-b + sqrt(discriminant)) / (2.0 * a);
  return Intersections(Intersection(t1, &s), Intersection(t2, &s));
}

Intersections intersect(const World &w, const Ray &r) {
  Intersections xs;

  for (const Sphere &o : w.objects())
    xs.add(intersect(o, r));

  return xs;
}

} // namespace ratrac
