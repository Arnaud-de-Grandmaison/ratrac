#include "ratrac/Matrice.h"

#include <iomanip>
// #help: Should ostream be included here ?

#define FIXED_FLOAT(x) std::fixed << std::setprecision(1) << (x)

std::ostream &operator<<(std::ostream &os, const ratrac::Matrice &M) {
  os << "Matrice { ";
  for (unsigned x{0}; x < std::get<0>(M.size()); x++) {
    for (unsigned y{0}; y < std::get<1>(M.size()); y++) {
      os << std::setw(6) << std::setfill(' ');
      os << FIXED_FLOAT(M.at(x, y));
      if (y != std::get<1>(M.size()) - 1)
        os << ", ";
    }
    if (x != std::get<0>(M.size()) - 1)
      os << "},\n\t{ ";
  }
  os << "}}\n";
  return os;
}
