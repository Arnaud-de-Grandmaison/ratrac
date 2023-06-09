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

ColorCheckers::operator std::string() const {
  return "ColorCheckers { a: " + std::string(color1()) +
         ", b: " + std::string(color2()) +
         ", transform: " + std::string(transform()) + "}";
}

RadialGradient::operator std::string() const {
  return "RadialGradient { a: " + std::string(color1()) + ", b: " + std::string(color2()) +
         ", transform: " + std::string(transform()) + "}";
}

PatternCheckers::operator std::string() const {
  return "PatternCheckers { a: " + std::string(*pattern1()) +
         ", b: " + std::string(*pattern2()) +
         ", transform: " + std::string(transform()) + "}";
}

PatternBlender::operator std::string() const {
  return "PatternBlender { a: " + std::string(*pattern1()) +
         ", b: " + std::string(*pattern2()) +
         ", transform: " + std::string(transform()) + "}";
}

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Pattern &P) {
  os << std::string(P);
  return os;
}
