#include "ratrac/Material.h"

std::ostream &operator<<(std::ostream &os, const ratrac::Material &M) {
  os << "Material {";
  os << " color: " << M.color();
  os << ", ambient: " << M.ambient();
  os << ", diffuse: " << M.diffuse();
  os << ", specular: " << M.specular();
  os << ", shininess: " << M.shininess();
  os << '}';
  return os;
}
