#include "ratrac/Light.h"

#include "gtest/gtest.h"

using namespace ratrac;
using namespace testing;

TEST(Light, base) {
  // A point of light has a position and intensity.
  Color intensity(1, 1, 1);
  Tuple position = Point(0, 0, 0);
  LightPoint lp(position, intensity);

  EXPECT_EQ(lp.position(), position);
  EXPECT_EQ(lp.intensity(), intensity);

  // Equality.
  EXPECT_TRUE(lp == LightPoint(position, intensity));
  EXPECT_FALSE(lp == LightPoint(Point(1, 1, 1), intensity));
  EXPECT_FALSE(lp == LightPoint(position, Color(0.5, 0.5, 0.5)));

  // Inequality.
  EXPECT_FALSE(lp != LightPoint(position, intensity));
  EXPECT_TRUE(lp != LightPoint(Point(1, 1, 1), intensity));
  EXPECT_TRUE(lp != LightPoint(position, Color(0.5, 0.5, 0.5)));
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
