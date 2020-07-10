#include "ratrac/ratrac.h"

#include "gtest/gtest.h"

#include <sstream>

using namespace ratrac;
using namespace testing;

TEST(World, basic) {
  World w;

  std::ostringstream sstr;
  w.say(sstr, "howdy");
  EXPECT_EQ(sstr.str(), "howdy\n");
}

TEST(RayTracerTuple, init) {
  // Tuple initialisation
  Tuple rtt(4.3, -4.2, 3.1, 1.0);
  EXPECT_EQ(rtt.x(), 4.3);
  EXPECT_EQ(rtt.y(), -4.2);
  EXPECT_EQ(rtt.z(), 3.1);
  EXPECT_EQ(rtt.w(), 1.0);
  EXPECT_EQ(rtt.type(), Tuple::Type::point);
  EXPECT_NE(rtt.type(), Tuple::Type::vector);

  rtt = Tuple(4.3, -4.2, 3.1, 0.0);
  EXPECT_EQ(rtt.x(), 4.3);
  EXPECT_EQ(rtt.y(), -4.2);
  EXPECT_EQ(rtt.z(), 3.1);
  EXPECT_EQ(rtt.w(), 0.0);
  EXPECT_EQ(rtt.type(), Tuple::Type::vector);
  EXPECT_NE(rtt.type(), Tuple::Type::point);

  //Point initialisation
  Point p(4.0, -4.0, 3.0);
  EXPECT_TRUE(p == Tuple(4.0, -4.0, 3.0, 1.0));
  EXPECT_FALSE(p != Tuple(4.0, -4.0, 3.0, 1.0));
  
  //Vector initialisation
  Vector v(4.0, -4.0, 3.0);
  EXPECT_TRUE(v == Tuple(4.0, -4.0, 3.0, 0.0));
  EXPECT_FALSE(v != Tuple(4.0, -4.0, 3.0, 0.0));
}

TEST(RayTracerTuple, operations) {
  // Sum
  // ===

  // Sum of a Point and a Vector
  Point p(3.0, -2.0, 5.0);
  Vector v(-2.0, 3.0, 1.0);
  EXPECT_EQ(p + v, Tuple(1.0, 1.0, 6.0, 1.0));
  p += v;
  EXPECT_EQ(p, Tuple(1.0, 1.0, 6.0, 1.0));

  // Substraction
  // ============

  // Subtracting two Tuples
  Tuple t1(3.0, 2.0, 1.0, 0.0);
  Tuple t2(5.0, 6.0, 7.0, 1.0);
  EXPECT_EQ(t1 - t2, Tuple(-2.0, -4.0, -6.0, -1.0));
  t1 -= t2;
  EXPECT_EQ(t1, Tuple(-2.0, -4.0, -6.0, -1.0));

  // Subtracting two Points
  Point p1(3.0, 2.0, 1.0);
  Point p2(5.0, 6.0, 7.0);
  EXPECT_EQ(p1 - p2, Vector(-2.0, -4.0, -6.0));

  // Subtracting two Vectors
    Vector v1(3.0, 2.0, 1.0);
  Vector v2(5.0, 6.0, 7.0);
  EXPECT_EQ(v1 - v2, Vector(-2.0, -4.0, -6.0));

  // Subtracting a Vector from a Point
  p = Point(3, 2, 1);
  v = Vector(5, 6, 7);
  EXPECT_EQ(p - v, Point(-2.0, -4.0, -6.0));
  p -= v;
  EXPECT_EQ(p, Point(-2.0, -4.0, -6.0));
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
