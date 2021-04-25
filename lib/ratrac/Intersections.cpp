#include "ratrac/Intersections.h"
#include "ratrac/Ray.h"
#include "ratrac/Shapes.h"
#include "ratrac/World.h"
#include "ratrac/ratrac.h"

#include <cmath>

using std::sqrt;

namespace ratrac {

Computations::Computations(const Intersection &x, const Ray &ray)
    : t(x.t), object(x.object), point(position(ray, x.t)), over_point(),
      eyev(-ray.direction()), normalv(object->normal_at(point)), inside(false) {
  if (dot(normalv, eyev) < 0.0) {
    inside = true;
    normalv = -normalv;
  }
  over_point = point + normalv * EPSILON<Tuple::DataType>();
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
  Intersections xs = world.intersect(ray);
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
  Intersections intersections = world.intersect(r);

  Intersections::const_iterator h = intersections.hit();
  if (h != intersections.end() && h->t < distance)
    return true;

  return false;
}
} // namespace ratrac
