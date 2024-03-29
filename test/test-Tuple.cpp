#include "gtest/gtest.h"

#include "ratrac/Tuple.h"

#include <sstream>

using namespace ratrac;
using namespace testing;

using std::ostringstream;

TEST(Tuple, init) {
  // Tuple initialisation.
  Tuple rtt(4.3, -4.2, 3.1, 1.0);
  EXPECT_EQ(rtt.x(), 4.3);
  EXPECT_EQ(rtt.y(), -4.2);
  EXPECT_EQ(rtt.z(), 3.1);
  EXPECT_EQ(rtt.w(), 1.0);
  EXPECT_TRUE(rtt.isPoint());
  EXPECT_FALSE(rtt.isVector());

  rtt = Tuple(4.3, -4.2, 3.1, 0.0);
  EXPECT_EQ(rtt.x(), 4.3);
  EXPECT_EQ(rtt.y(), -4.2);
  EXPECT_EQ(rtt.z(), 3.1);
  EXPECT_EQ(rtt.w(), 0.0);
  EXPECT_TRUE(rtt.isVector());
  EXPECT_FALSE(rtt.isPoint());

  // Point initialisation.
  Tuple p = Point(4.0, -4.0, 3.0);
  EXPECT_TRUE(p == Tuple(4.0, -4.0, 3.0, 1.0));
  EXPECT_FALSE(p != Tuple(4.0, -4.0, 3.0, 1.0));

  // Vector initialisation.
  Tuple v = Vector(4, -4, 3);
  EXPECT_TRUE(v == Tuple(4.0, -4.0, 3.0, 0.0));
  EXPECT_FALSE(v != Tuple(4.0, -4.0, 3.0, 0.0));
}

TEST(Tuple, output) {
  Tuple t(4.3, -4.2, 3.1, 1.0);
  ostringstream oss;
  oss << t;
  EXPECT_EQ(oss.str(), "Tuple { 4.3, -4.2, 3.1, 1}");
}

TEST(Tuple, operations) {
  // Sum.
  // ====

  // Adding two Tuples.
  Tuple t1(3.0, 2.0, 1.0, 1.0);
  Tuple t2(5.0, 6.0, 7.0, 1.0);
  EXPECT_EQ(t1 + t2, Tuple(8.0, 8.0, 8.0, 2.0));
  t1 += t2;
  EXPECT_EQ(t1, Tuple(8.0, 8.0, 8.0, 2.0));

  // Adding two Points.
  Tuple p1 = Point(3, 2, 1);
  Tuple p2 = Point(5, 6, 7);
  EXPECT_EQ(p1 + p2, Tuple(8.0, 8.0, 8.0, 2.0));

  // Adding two Vectors.
  Tuple v1 = Vector(3, 2, 1);
  Tuple v2 = Vector(5, 6, 7);
  EXPECT_EQ(v1 + v2, Vector(8, 8, 8));

  // Sum of a Point and a Vector => Point.
  Tuple p = Point(3, -2, 5);
  Tuple v = Vector(-2, 3, 1);
  EXPECT_EQ(p + v, Tuple(1.0, 1.0, 6.0, 1.0));
  p += v;
  EXPECT_EQ(p, Tuple(1.0, 1.0, 6.0, 1.0));

  // Substraction.
  // =============

  // Subtracting two Tuples.
  t1 = Tuple(3.0, 2, 1.0, 0.0);
  t2 = Tuple(5.0, 6, 7.0, 1.0);
  EXPECT_EQ(t1 - t2, Tuple(-2.0, -4.0, -6.0, -1.0));
  t1 -= t2;
  EXPECT_EQ(t1, Tuple(-2.0, -4.0, -6.0, -1.0));

  // Subtracting two Points.
  p1 = Point(3, 2, 1);
  p2 = Point(5, 6, 7);
  EXPECT_EQ(p1 - p2, Vector(-2.0, -4, -6.0));

  // Subtracting two Vectors.
  v1 = Vector(3, 2, 1);
  v2 = Vector(5, 6, 7);
  EXPECT_EQ(v1 - v2, Vector(-2.0, -4.0, -6));

  // Subtracting a Vector from a Point.
  p = Point(3.0, 2.0, 1.0);
  v = Vector(5.0, 6.0, 7.0);
  EXPECT_EQ(p - v, Point(-2, -4.0, -6.0));
  p -= v;
  EXPECT_EQ(p, Point(-2.0, -4, -6));

  // Minus operator/Negating a tuple.

  Tuple t(1.0, -2.0, 3.0, -4.0);
  EXPECT_EQ(-t, Tuple(-1.0, 2.0, -3.0, 4.0));

  // Not required by the book
  // v = Vector(1.0, -2.0, 3.0);
  // EXPECT_EQ(-t, Vector(1.0, 2.0, -3.0));

  // Multiplication.
  // ===============

  // Multiplying a tuple by a scalar (3.5).
  t = Tuple(1.0, -2.0, 3.0, -4.0);
  EXPECT_EQ(t * 3.5, Tuple(3.5, -7.0, 10.5, -14.0));
  EXPECT_EQ(3.5 * t, Tuple(3.5, -7.0, 10.5, -14.0));

  // Multiplying a tuple by a fraction (0.5).
  EXPECT_EQ(t * 0.5, Tuple(0.5, -1, 1.5, -2));
  EXPECT_EQ(0.5 * t, Tuple(0.5, -1, 1.5, -2));

  // Division.
  // =========

  // Dividing a tuple by a scalar (2).
  t = Tuple(1.0, -2.0, 3.0, -4.0);
  EXPECT_EQ(t / 2.0, Tuple(0.5, -1.0, 1.5, -2.0));
  // Not required by the book => not defined in the Tuple class
  // EXPECT_EQ(2.0 / t, Tuple(?, ?, ?, ?));

  // Not required by the book
  // Dividing a tuple by a fraction (0.5)
  // EXPECT_EQ(t / 0.5, Tuple(0.5, -1, 1.5, -2));
  // EXPECT_EQ(0.5 / t, Tuple(0.5, -1, 1.5, -2));

  // Getting magnitude.
  // ==================

  // Simple tests.
  v = Vector(1.0, 0.0, 0.0);
  EXPECT_EQ(v.magnitude(), 1.0);
  v = Vector(0.0, 1.0, 0.0);
  EXPECT_EQ(v.magnitude(), 1.0);
  v = Vector(0.0, 0.0, 1.0);
  EXPECT_EQ(v.magnitude(), 1.0);

  // Advanced tests.
  v = Vector(1.0, 2.0, 3.0);
  EXPECT_EQ(v.magnitude(), std::sqrt(14));
  v = Vector(-1.0, -2.0, -3.0);
  EXPECT_EQ(v.magnitude(), std::sqrt(14));
  EXPECT_EQ(magnitude(v), std::sqrt(14));

  // Normalizing.
  // ============

  v = Vector(4.0, 0.0, 0.0);
  EXPECT_EQ(normalize(v), Vector(1.0, 0.0, 0.0));
  v.normalize();
  EXPECT_EQ(v, Vector(1.0, 0.0, 0.0));
  v = Vector(1.0, 2.0, 3.0);
  EXPECT_EQ(v.normalize(),
            Vector(1 / std::sqrt(14), 2 / std::sqrt(14), 3 / std::sqrt(14)));

  // Dot product (/scalar product).
  // ==============================

  v1 = Vector(1.0, 2.0, 3.0);
  v2 = Vector(2.0, 3.0, 4.0);
  EXPECT_EQ(dot(v1, v2), 20);

  // Cross product.
  // ==============

  v1 = Vector(1.0, 2.0, 3.0);
  v2 = Vector(2.0, 3.0, 4.0);
  EXPECT_EQ(cross(v1, v2), Vector(-1.0, 2.0, -1.0));
  EXPECT_EQ(cross(v2, v1), Vector(1.0, -2.0, 1.0));
}

TEST(Tuple, reflection) {
  // Reflecting a vector approaching at 45°.
  // =======================================
  Tuple v = Vector(1, -1, 0);
  Tuple n = Vector(0, 1, 0);
  Tuple r = v.reflect(n);
  EXPECT_EQ(r, Vector(1, 1, 0));

  // Reflecting a vector off a slanted surface.
  // ==========================================
  v = Vector(0, -1, 0);
  n = Vector(sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0);
  r = v.reflect(n);
  EXPECT_EQ(r, Vector(1, 0, 0));
}
