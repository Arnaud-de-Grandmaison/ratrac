#include "ratrac/World.h"

#include "ratrac/Color.h"
#include "ratrac/Intersections.h"
#include "ratrac/Light.h"
#include "ratrac/Material.h"
#include "ratrac/Shapes.h"
#include "ratrac/Tuple.h"

#include <string>

using std::ostream;
using std::string;
using std::unique_ptr;

std::ostream &operator<<(ostream &os, const ratrac::World &world) {
  os << "World {";
  os << "  lights: [";

  string sep = "";
  for (const auto &l : world.lights()) {
    os << sep << l;
    sep = ", ";
  }

  os << "  ],";
  os << "  objects: [";

  sep = "";
  for (const auto &o : world.objects()) {
    os << sep << *o.get();
    sep = ", ";
  }

  os << "  ]";
  os << "}";
  return os;
}

namespace ratrac {
  Intersections World::intersect(const Ray &r) const {
  Intersections xs;

  for (const auto &o : objects())
    xs.add(o->intersect(r));

  return xs;
}

World World::get_default() {
  World w;
  w.lights().push_back(LightPoint(Point(-10, 10, -10), Color::WHITE()));

  Sphere *s1 = new Sphere();
  s1->material(Material(Color(0.8, 1.0, 0.6), /* ambient: */ 0.1,
                        /* diffuse: */ 0.7, /* specular: */ 0.2,
                        /* shininess: */ 200.0));
  w.objects().push_back(unique_ptr<Shape>(s1));

  Sphere *s2 = new Sphere();
  s2->transform(Matrice::scaling(0.5, 0.5, 0.5));
  w.objects().push_back(unique_ptr<Shape>(s2));

  return w;
}

} // namespace ratrac
