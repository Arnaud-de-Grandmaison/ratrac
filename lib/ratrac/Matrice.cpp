#include "ratrac/Matrice.h"

#include <iomanip>
#include <sstream>

namespace ratrac {
Matrice::operator std::string() const {
  std::ostringstream os;
  os << std::fixed << std::setprecision(1); // Use fixed precision.
  os << "Matrice { ";
  for (unsigned x = 0; x < getNumLines(); x++) {
    for (unsigned y = 0; y < getNumColumns(); y++) {
      os << std::setw(6) << std::setfill(' ');
      os << at(x, y);
      if (y != getNumColumns() - 1)
        os << ", ";
    }
    if (x != getNumLines() - 1)
      os << "},\n\t{ ";
  }
  os << "}}\n";

  return os.str();
}
} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Matrice &M) {
  os << std::string(M);

  return os;
}
