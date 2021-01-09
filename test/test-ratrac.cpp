#include "gtest/gtest.h"

#include "ratrac/ratrac.h"
#include "ratrac/Camera.h"
#include "ratrac/Canvas.h"
#include "ratrac/Color.h"
#include "ratrac/Intersections.h"
#include "ratrac/Light.h"
#include "ratrac/Material.h"
#include "ratrac/Matrice.h"
#include "ratrac/Ray.h"
#include "ratrac/Shapes.h"
#include "ratrac/Tuple.h"
#include "ratrac/World.h"

#include <cmath>
#include <sstream>
#include <tuple>
#include <vector>

using namespace ratrac;
using namespace testing;

#include "test-Camera.inc.cpp"
#include "test-Canvas.inc.cpp"
#include "test-Color.inc.cpp"
#include "test-Intersections.inc.cpp"
#include "test-Light.inc.cpp"
#include "test-Material.inc.cpp"
#include "test-Matrice.inc.cpp"
#include "test-Ray.inc.cpp"
#include "test-Shapes.inc.cpp"
#include "test-Tuple.inc.cpp"
#include "test-World.inc.cpp"

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
