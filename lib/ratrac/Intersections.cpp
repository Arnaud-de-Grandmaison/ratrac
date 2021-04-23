#include "ratrac/Intersections.h"
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

Color shade_hit(const World &world, const Computations &comps) {
  Color col;
  for (unsigned i = 0; i < world.lights().size(); i++) {
    bool in_shadow = is_shadowed(world, comps.over_point, i);
    col += lighting(comps.object->material(), *world.light(i), comps.over_point,
                    comps.eyev, comps.normalv, in_shadow);
  }
  return col;
}

Color color_at(const World &world, const Ray &ray) {
  Intersections xs = intersect(world, ray);
  if (xs.empty())
    return Color::BLACK();

  Computations comps(*xs.hit(), ray);

  return shade_hit(world, comps);
}

bool is_shadowed(const World &world, const Tuple &point, unsigned i) {
  Tuple v = world.light(i)->position() - point;
  Tuple::DataType distance = magnitude(v);
  Tuple direction = normalize(v);

  Ray r = Ray(point, direction);
  Intersections intersections = intersect(world, r);

  Intersections::const_iterator h = intersections.hit();
  if (h != intersections.end() && h->t < distance)
    return true;

  return false;
}
} // namespace ratrac
