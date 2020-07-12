#include <string>

#include "ratrac/Canvas.h"

#include "gtest/gtest.h"

using namespace ratrac;
using namespace testing;

TEST(RayTracerTuple, canvas) {
  // Testing canvas
  // ==============

  // Creating a canvas
  Canvas C(10, 20);
  EXPECT_EQ(C.width(), 10);
  EXPECT_EQ(C.height(), 20);

  // Writing pixels to a canvas
  Color red(1.0, 0.0, 0.0);
  C.at(2, 3) = red;
  EXPECT_EQ(C.at(2, 3), red);

  // Saving canvas
  // =============

  // Constructing the PPM header
  C = Canvas(5, 3);
  std::string ppm = canvas_to_ppm(C);
  // #ToBeContinued: test each line.
  EXPECT_EQ(std::getline(ppm, "P3\n"), "P3\n");
  EXPECT_EQ(std::getline(ppm, 1), "5 3\n");
  EXPECT_EQ(std::getline(ppm, 2), "255\n");
  // Constructing the PPM pixel data

  // Splitting long lines in PPM files

  // PPM files are terminated by a newline character
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
