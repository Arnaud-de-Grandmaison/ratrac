#include "ratrac/Shapes.h"

std::ostream &operator<<(std::ostream &os, const ratrac::Sphere &s) {
  os << "Sphere {";
  os << " center: " << s.center();
  os << ", radius: " << s.radius();
  os << ", transform: " << s.transform();
  os << ", material: " << s.material();
  os << "}";
  return os;
}
