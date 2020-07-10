#include "ratrac/ratrac.h"

#include "gtest/gtest.h"

#include <sstream>

using namespace ratrac;
using namespace testing;

TEST(World, basic) {
  World w;

  std::ostringstream sstr;
  w.say(sstr,"howdy");
  EXPECT_EQ(sstr.str(), "howdy\n");
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
