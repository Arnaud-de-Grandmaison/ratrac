#include "gtest/gtest.h"

#include "ratrac/Ray.h"

#include <sstream>

using namespace ratrac;
using namespace testing;

using std::ostringstream;

TEST(Ray, base) {
  // Testing Ray
  // ===========

  // Create a default Ray.
  Ray r;

  // Creating and querying a Ray
  Tuple origin = Point(1, 2, 3);
  Tuple direction = Vector(4, 5, 6);
  Ray ray(origin, direction);
  EXPECT_EQ(ray.origin(), origin);
  EXPECT_EQ(ray.direction(), direction);

  // Assign a Ray.
  r = ray;
  EXPECT_EQ(ray.origin(), origin);
  EXPECT_EQ(ray.direction(), direction);

  // Computing a point from a distance
  ray = Ray(Point(2, 3, 4), Vector(1, 0, 0));
  EXPECT_EQ(position(ray, 0), Point(2, 3, 4));
  EXPECT_EQ(position(ray, 1), Point(3, 3, 4));
  EXPECT_EQ(position(ray, -1), Point(1, 3, 4));
  EXPECT_EQ(position(ray, 2.5), Point(4.5, 3, 4));
}

TEST(Ray, output) {
  Ray ray(Point(1, 2, 3), Vector(4, 5, 6));
  ostringstream oss;
  oss << ray;
  EXPECT_EQ(
      oss.str(),
      "Ray { origin: Tuple { 1, 2, 3, 1}, direction: Tuple { 4, 5, 6, 0}}");
}

TEST(Ray, transform) {
  // Translating a ray
  Ray r(Point(1, 2, 3), Vector(0, 1, 0));
  Matrix m = Matrix::translation(3, 4, 5);
  Ray r2 = transform(r, m);
  EXPECT_EQ(r2.origin(), Point(4, 6, 8));
  EXPECT_EQ(r2.direction(), Vector(0, 1, 0));

  // Scaling a ray
  r = Ray(Point(1, 2, 3), Vector(0, 1, 0));
  m = Matrix::scaling(2, 3, 4);
  r2 = transform(r, m);
  EXPECT_EQ(r2.origin(), Point(2, 6, 12));
  EXPECT_EQ(r2.direction(), Vector(0, 3, 0));
}
