#include "ratrac/World.h"

#include "ratrac/Color.h"
#include "ratrac/Light.h"
#include "ratrac/Material.h"
#include "ratrac/Shapes.h"
#include "ratrac/Tuple.h"

std::ostream &operator<<(std::ostream &os, const ratrac::World &world) {
  os << "World {\n";
  os << "  lights: [\n";
  os << "  ],\n";
  os << "  objects: [\n";
  os << "  ]\n";
  os << "}";
  return os;
}

namespace ratrac {
World World::get_default() {
  World w;
  w.lights().push_back(LightPoint(Point(-10, 10, -10), Color::WHITE()));
  w.objects().push_back(
      Sphere().material(Material(Color(0.8, 1.0, 0.6), /* ambient: */ 0.1,
                                 /* diffuse: */ 0.7, /* specular: */ 0.2,
                                 /* shininess: */ 200.0)));
  w.objects().push_back(Sphere().transform(scaling(0.5, 0.5, 0.5)));
  return w;
}
} // namespace ratrac
