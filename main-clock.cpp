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
  Canvas C(100, 100);
  // Generate a centered point
  Tuple temp_point;
  Matrice temp_transform;
  double temp_angle;

  temp_transform = Matrice::identity_matrix();
  temp_angle = M_PI * 2;
  temp_transform *= rotation_z(temp_angle);
  temp_transform *= translation(5., 5., 0.);
  temp_point = temp_transform * Point<double>(0, 0, 0);
  C.at(temp_point.x(), temp_point.y()) = Color(1, 1, 1);

  /*
  // Move the point
  // --------------
  for (unsigned rotation_angle(0); rotation_angle < 4; rotation_angle++) {
    temp_transform = Matrice::identity_matrix();
    temp_transform *= translation<double>(50., 0., 0.);
    temp_angle = 0 + 2 * M_PI / 12 * rotation_angle;
    temp_transform *= rotation_x(temp_angle);
    // #help: Should be working ...
    // temp_transform.rotate_x(0 + 2 * M_PI / 12 * rotation_angle);
    // temp_transform.translate(50., 0., 0.);
  temp_point = temp_transform * Point<double>(500, 500, 0);
  C.at(temp_point.x(), temp_point.y()) = Color(1, 1, 1);
}*/
  ofstream file(filename);
  C.to_ppm(file);
}

int main(int argc, char *argv[]) {
  clock_generator();
  return 0;
}
