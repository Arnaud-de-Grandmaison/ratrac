#include "ratrac/Tuple.h"
#include "ratrac/Ray.h"

#include "gtest/gtest.h"

using namespace std;
using namespace ratrac;
using namespace testing;

TEST(Ray, base) {

  // Testing Ray
  // ===========

  // Creating and querying a Ray
  Tuple origin = Point(1, 2, 3);
  Tuple direction = Vector(4, 5, 6);
  Ray ray(origin, direction);
  EXPECT_EQ(ray.origin(), origin);
  EXPECT_EQ(ray.direction(), direction);

  // Computing a point from a distance
  ray = Ray(Point(2, 3, 4), Vector(1, 0, 0));
  EXPECT_EQ(position(ray, 0), Point(2, 3, 4));
  EXPECT_EQ(position(ray, 1), Point(3, 3, 4));
  EXPECT_EQ(position(ray, -1), Point(1, 3, 4));
  EXPECT_EQ(position(ray, 2.5), Point(4.5, 3, 4));
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
