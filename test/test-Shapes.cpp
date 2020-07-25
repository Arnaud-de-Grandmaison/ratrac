#include "ratrac/Matrice.h"
#include "ratrac/Shapes.h"

#include "gtest/gtest.h"

using namespace std;
using namespace ratrac;
using namespace testing;

TEST(Shapes, base) {

  // Basic sphere creation
  Sphere s;
  EXPECT_EQ(s.center(), Point(0, 0, 0));
  EXPECT_EQ(s.radius(), 1.0);
  EXPECT_EQ(s.transform(), Matrice::identity());

  // Change a sphere's transformation
  s = Sphere();
  Matrice t = translation(2, 3, 4);
  s.transform(t);
  EXPECT_EQ(s.transform(), t);
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
