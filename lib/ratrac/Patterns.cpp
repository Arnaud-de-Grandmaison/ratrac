#include "ratrac/Patterns.h"

namespace ratrac {
Pattern::~Pattern() {}

Stripes::operator std::string() const {
  return "Stripes { a: " + std::string(a) + ", b: " + std::string(b) +
         ", transform: " + std::string(transform()) + "}";
}

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Pattern &P) {
  os << std::string(P);
  return os;
}
