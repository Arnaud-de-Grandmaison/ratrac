#include "ratrac/Patterns.h"

namespace ratrac {
Pattern::~Pattern() {}

Stripes::operator std::string() const {
  return "Stripes { a: " + std::string(color1()) + ", b: " + std::string(color2()) +
         ", transform: " + std::string(transform()) + "}";
}

Gradient::operator std::string() const {
  return "Gradient { a: " + std::string(color1()) + ", b: " + std::string(color2()) +
         ", transform: " + std::string(transform()) + "}";
}

Ring::operator std::string() const {
  return "Ring { a: " + std::string(color1()) + ", b: " + std::string(color2()) +
         ", transform: " + std::string(transform()) + "}";
}

Checkers::operator std::string() const {
  return "Checkers { a: " + std::string(color1()) + ", b: " + std::string(color2()) +
         ", transform: " + std::string(transform()) + "}";
}
} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Pattern &P) {
  os << std::string(P);
  return os;
}
