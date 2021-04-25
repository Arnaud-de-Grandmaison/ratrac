#include "ratrac/Material.h"

#include <iomanip>

std::ostream &operator<<(std::ostream &os, const ratrac::Material &M) {
  std::ios_base::fmtflags f(os.flags());
  os << "Material {";
  os << " color: " << M.color();
  os << std::fixed << std::setprecision(1); // Use fixed precision.
  os << ", ambient: " << M.ambient();
  os << ", diffuse: " << M.diffuse();
  os << ", specular: " << M.specular();
  os << ", shininess: " << M.shininess();
  os << '}';
  os.flags(f); // Restore flags.
  return os;
}
