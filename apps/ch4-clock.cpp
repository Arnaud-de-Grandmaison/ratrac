#include "ratrac/App.h"
#include "ratrac/Canvas.h"
#include "ratrac/Color.h"
#include "ratrac/Matrix.h"
#include "ratrac/ProgressBar.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

#include <fstream>
#include <iostream>

using namespace ratrac;
using std::cout;
using std::ofstream;

void clock_generator(Canvas &C) {
  // Generate a centered point
  TimedProgressBar PB("Generation", 12, cout);
  for (unsigned i = 0; i < 12; i++) {
    Matrix temp_transform = Matrix::identity();
    temp_transform *= Matrix::rotation_y(double(i) * 2.0 * M_PI / double(12));
    Tuple temp_point = Matrix::translation(100.0, 0.0, 100.0) * temp_transform *
                       Point(50, 0, 0);
    C.at(temp_point.x(), temp_point.z()) = Color(.5, .75, .5);
    PB.incr();
  }
}

int main(int argc, char *argv[]) {
  App app("clock", "simple clock test", 200, 200);
  if (!app.parse(argc - 1, (const char **)argv + 1))
    app.error("command line arguments parsing failed.");
  if (app.verbose())
    cout << app.parameters() << '\n';

  Canvas C(app.width(), app.height());

  clock_generator(C);

  app.save(C);

  return 0;
}
