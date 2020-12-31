#include "ratrac/Ray.h"

std::ostream &operator<<(std::ostream &os, const ratrac::Ray &ray) {
  os << "Ray {";
  os << " origin: " << ray.origin();
  os << ", direction: " << ray.direction();
  os << "}";
  return os;
}
