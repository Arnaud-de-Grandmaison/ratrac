#include "ratrac/Ray.h"
#include "ratrac/Shapes.h"
#include "ratrac/Tuple.h"

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

TEST(Ray, intersections) {
  // A ray intersects a sphere at two points
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  Sphere s;
  Intersection xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0], 4.0);
  EXPECT_EQ(xs[1], 6.0);

  // A ray intersects a sphere at a tangent
  r = Ray(Point(0, 1, -5), Vector(0, 0, 1));
  xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0], 5.0);
  EXPECT_EQ(xs[1], 5.0);

  // A ray misses a sphere
  r = Ray(Point(0, 2, -5), Vector(0, 0, 1));
  xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 0);

  // A ray intersects a sphere at a tangent
  r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
  xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0], -1.0);
  EXPECT_EQ(xs[1], 1.0);

  // A sphere is behind a ray
  r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
  xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0], -6.0);
  EXPECT_EQ(xs[1], -4.0);
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
