#include "ratrac/Canvas.h"
#include "ratrac/Color.h"
#include "ratrac/Matrice.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

#include <fstream>
#include <iostream>
#include <string>

#define _USE_MATH_DEFINES
#ifndef M_PI
const double M_PI = 3.14159265;
#endif // ! M_PI

using namespace ratrac;
using namespace std;

void clock_generator(const char *filename = "clock.ppm") {
  Canvas C(200, 200);

  // Generate a centered point
  for (unsigned i = 0; i < 12; i++) {
    Matrice temp_transform = Matrice::identity_matrix();
    temp_transform *= rotation_y(double(i) * 2.0 * M_PI / double(12));
    Tuple temp_point = translation(100.0, 0.0, 100.0) * temp_transform *
                       Point<double>(50, 0, 0);
    C.at(temp_point.x(), temp_point.z()) = Color(.5, .75, .5);
  }

  ofstream file(filename);
  C.to_ppm(file);
}

int main(int argc, char *argv[]) {
  clock_generator();
  return 0;
}
