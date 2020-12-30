#include "ratrac/Color.h"

#include "gtest/gtest.h"

using namespace ratrac;
using namespace testing;

TEST(Color, colors) {
  // Testing same operations on colors.
  // ==================================

  // Colors are red, green, blue tuples.
  Color c(-0.5, 0.4, 1.7);
  EXPECT_EQ(c.red(), -0.5f);
  EXPECT_EQ(c.green(), 0.4f);
  EXPECT_EQ(c.blue(), 1.7f);

  // Adding colors.
  Color c1(0.9, 0.6, 0.75);
  Color c2(0.7, 0.1, 0.25);
  EXPECT_EQ(c1 + c2, Color(1.6, 0.7, 1.0));

  // Subtracting colors.
  c1 = Color(0.9, 0.6, 0.75);
  c2 = Color(0.7, 0.1, 0.25);
  EXPECT_EQ(c1 - c2, Color(0.2, 0.5, 0.5));

  // Multiplying a color by a scalar.
  c = Color(0.2, 0.3, 0.4);
  EXPECT_EQ(c * 2.0, Color(0.4, 0.6, 0.8));

  // Multiplying colors.
  c1 = Color(1.0, 0.2, 0.4);
  c2 = Color(0.9, 1.0, 0.1);
  EXPECT_EQ(c1 * c2, Color(0.9, 0.2, 0.04));

  // Color helpers.
  EXPECT_EQ(Color::black(), Color(0, 0, 0));

  // Multiplying colors/Hadamard product/Schur product
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
