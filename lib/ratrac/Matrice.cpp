#include "ratrac/Matrice.h"

#include <iomanip>

#define FIXED_FLOAT(x) std::fixed << std::setprecision(1) << (x)

std::ostream &operator<<(std::ostream &os, const ratrac::Matrice &M) {
  os << "Matrice { ";
  for (unsigned x = 0; x < M.getNumLines(); x++) {
    for (unsigned y = 0; y < M.getNumColumns(); y++) {
      os << std::setw(6) << std::setfill(' ');
      os << FIXED_FLOAT(M.at(x, y));
      if (y != M.getNumColumns() - 1)
        os << ", ";
    }
    if (x != M.getNumLines() - 1)
      os << "},\n\t{ ";
  }
  os << "}}\n";
  return os;
}
