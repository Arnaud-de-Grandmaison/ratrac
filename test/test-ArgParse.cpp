#include <gtest/gtest.h>

#include "ratrac/ArgParse.h"

#include <array>
#include <string>

using namespace ratrac;
using namespace testing;

using std::array;
using std::string;

TEST(ArgParse, construct) {
  ArgParse A("myprogram", "does wonderful things.");
  EXPECT_EQ(A.programName(), "myprogram");
  EXPECT_EQ(A.help(), "myprogram does wonderful things.");
}

TEST(ArgParse, addOptionWithNoValue) {
  string option = "Not set";
  ArgParse A1("myprogram", "does wonderful things.");
  A1.addOption({"-o", "--option"}, "a great option", [&]() {
    option = "Has been set";
    return true;
  });

  EXPECT_EQ(A1.help(), "myprogram does wonderful things.\n\nOptions:\n  "
                       "--option, -o: a great option");

  array<const char *, 1> args1 = {"-o"};
  EXPECT_TRUE(A1.parse(args1.size(), args1.data()));
  EXPECT_EQ(A1.programName(), "myprogram");
  EXPECT_EQ(option, "Has been set");

  option = "Not set";
  array<const char *, 1> args2 = {"--option"};
  EXPECT_TRUE(A1.parse(args2.size(), args2.data()));
  EXPECT_EQ(A1.programName(), "myprogram");
  EXPECT_EQ(option, "Has been set");

  option = "Not set";
  array<const char *, 0> args3 = {};
  EXPECT_TRUE(A1.parse(args3.size(), args3.data()));
  EXPECT_EQ(A1.programName(), "myprogram");
  EXPECT_EQ(option, "Not set");
}

TEST(ArgParse, addOptionWithValue) {
  string option = "Not set";
  ArgParse A1("myprogram", "does wonderful things.");
  A1.addOptionWithValue({"-o", "--option"}, "F", "a great option",
                        [&](const string &s) {
                          option = s;
                          return true;
                        });

  EXPECT_EQ(A1.help(), "myprogram does wonderful things.\n\nOptions:\n  "
                       "--option=F, -o F: a great option");

  array<const char *, 2> args1 = {"-o", "file"};
  EXPECT_TRUE(A1.parse(args1.size(), args1.data()));
  EXPECT_EQ(A1.programName(), "myprogram");
  EXPECT_EQ(option, "file");

  array<const char *, 2> args2 = {"--option", "another file"};
  EXPECT_TRUE(A1.parse(args2.size(), args2.data()));
  EXPECT_EQ(A1.programName(), "myprogram");
  EXPECT_EQ(option, "another file");

  array<const char *, 1> args3 = {"--option=yet another file"};
  EXPECT_TRUE(A1.parse(args3.size(), args3.data()));
  EXPECT_EQ(A1.programName(), "myprogram");
  EXPECT_EQ(option, "yet another file");

  option = "Not set";
  array<const char *, 0> args4 = {};
  EXPECT_TRUE(A1.parse(args4.size(), args4.data()));
  EXPECT_EQ(A1.programName(), "myprogram");
  EXPECT_EQ(option, "Not set");
}