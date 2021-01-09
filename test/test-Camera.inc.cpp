#include <gtest/gtest.h>

TEST(Camera, view_transform) {
  // The transformation matrix for the default orientation.
  Tuple from = Point(0, 0, 0);
  Tuple to = Point(0, 0, -1);
  Tuple up = Vector(0, 1, 0);
  Matrice t = view_transform(from, to, up);
  EXPECT_EQ(t, Matrice::identity());

  // View transformation matrix looking in positive z direction.
  from = Point(0, 0, 0);
  to = Point(0, 0, 1);
  up = Vector(0, 1, 0);
  t = view_transform(from, to, up);
  EXPECT_EQ(t, scaling(-1, 1, -1));

  // The view transformation moves the world.
  from = Point(0, 0, 8);
  to = Point(0, 0, 0);
  up = Vector(0, 1, 0);
  t = view_transform(from, to, up);
  EXPECT_EQ(t, translation(0, 0, -8));

  // An arbitrary view transformation.
  from = Point(1, 3, 2);
  to = Point(4, -2, 8);
  up = Vector(1, 1, 0);
  t = view_transform(from, to, up);
  EXPECT_TRUE(t.approximatly_equal(Matrice({
    {-0.50709,  0.50709,  0.67612, -2.36643},
    { 0.76772,  0.60609,  0.12122, -2.82843},
    {-0.35857,  0.59761, -0.71714,  0.00000},
    { 0.00000,  0.00000,  0.00000,  1.00000}
  })));
}
