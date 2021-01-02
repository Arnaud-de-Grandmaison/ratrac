#include "gtest/gtest.h"

using namespace testing;

#include "ratrac/ratrac.h"
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

using namespace ratrac;

#include "test-Canvas.inc.cpp"
#include "test-Color.inc.cpp"
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
