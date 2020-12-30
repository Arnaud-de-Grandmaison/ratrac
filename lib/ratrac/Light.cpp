#include "ratrac/Light.h"

std::ostream &operator<<(std::ostream &os, const ratrac::LightPoint &LP) {
  os << "LightPoint { intensity: " << LP.intensity()
     << ", position: " << LP.position() << "}";
  return os;
}
