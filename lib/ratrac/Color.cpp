#include "ratrac/Color.h"

#include <sstream>

namespace ratrac {
Color::operator std::string() const {
  std::ostringstream os;
  os << "Color { red:" << red();
  os << ", green:" << green();
  os << ", blue:" << blue();
  os << ", alpha:" << alpha() << "}";
  return os.str();
}
} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Color &C) {
  os << std::string(C);
  return os;
}
