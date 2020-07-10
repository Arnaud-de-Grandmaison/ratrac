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

  Point p(4.0, -4.0, 3.0);
  EXPECT_TRUE(p == Tuple(4.0, -4.0, 3.0, 1.0));
  EXPECT_FALSE(p != Tuple(4.0, -4.0, 3.0, 1.0));
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
