#include "ratrac/Color.h"

std::ostream &operator<<(std::ostream &os, const ratrac::Color &C) {
  os << "Color { reg:" << C.red() << ", green:" << C.green()
     << ", blue:" << C.blue() << ", alpha:" << C.alpha() << "}";
  return os;
}
