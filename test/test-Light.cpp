#include "ratrac/Light.h"

#include "gtest/gtest.h"

using namespace ratrac;
using namespace testing;

TEST(Light, light) {
  // A point of light has a position and intensity.
  Color intensity(1, 1, 1);
  Tuple position = Point(0, 0, 0);
  LightPoint lp(position, intensity);

  EXPECT_EQ(lp.position(), position);
  EXPECT_EQ(lp.intensity(), intensity);
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
