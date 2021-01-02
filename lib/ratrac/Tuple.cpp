#include "ratrac/Tuple.h"

std::ostream &operator<<(std::ostream &os, const ratrac::Tuple &T) {
  os << "Tuple { " << T.x() << ", " << T.y() << ", " << T.z() << ", " << T.w()
     << "}";
  return os;
}
