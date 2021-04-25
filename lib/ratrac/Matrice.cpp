#include "ratrac/Matrice.h"

#include <iomanip>

std::ostream &operator<<(std::ostream &os, const ratrac::Matrice &M) {
  std::ios_base::fmtflags f(os.flags());
  os << std::fixed << std::setprecision(1); // Use fixed precision.

  os << "Matrice { ";
  for (unsigned x = 0; x < M.getNumLines(); x++) {
    for (unsigned y = 0; y < M.getNumColumns(); y++) {
      os << std::setw(6) << std::setfill(' ');
      os << M.at(x, y);
      if (y != M.getNumColumns() - 1)
        os << ", ";
    }
    if (x != M.getNumLines() - 1)
      os << "},\n\t{ ";
  }
  os << "}}\n";

  os.flags(f); // Restore flags.

  return os;
}
