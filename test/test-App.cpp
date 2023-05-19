#include <gtest/gtest.h>

#include "ratrac/App.h"

#include <array>

using namespace ratrac;
using namespace testing;

using std::array;

TEST(App, defaults) {
  App A("myapp", "is wonderful.");
  EXPECT_EQ(A.programName(), "myapp");
  EXPECT_EQ(
      A.help(),
      "myapp is wonderful.\n\nOptions:\n  --help, -?: Display this help "
      "message.\n  --verbose, -v: Increase program verbosity.\n  --width=W, -w "
      "W: Set canvas width to W\n  --height=H, -h H: Set canvas height to H\n  "
      "--output=F, -o F: Save output to filename F\n  --format=T, -f T: Save "
      "output in image format T (PPM ony for now)");

  array<const char *, 0> args = {};
  EXPECT_TRUE(A.parse(args.size(), args.data()));
  EXPECT_EQ(A.width(), 320);
  EXPECT_EQ(A.height(), 240);
  EXPECT_FALSE(A.verbose());
  EXPECT_EQ(A.verbosity(), 0);
  EXPECT_EQ(A.outputFormat(), App::PPM);
  EXPECT_EQ(A.outputFilename(), "myapp.ppm");
}

TEST(App, overrideDefaultCanvas) {
  App A("myapp", "is wonderful.", 640, 480);

  array<const char *, 0> args = {};
  EXPECT_TRUE(A.parse(args.size(), args.data()));
  EXPECT_EQ(A.width(), 640);
  EXPECT_EQ(A.height(), 480);
  EXPECT_FALSE(A.verbose());
  EXPECT_EQ(A.verbosity(), 0);
}

TEST(App, configureCanvas) {
  App A("myapp", "is wonderful.");

  array<const char *, 2> argsW = {"-w", "640"};
  EXPECT_TRUE(A.parse(argsW.size(), argsW.data()));
  EXPECT_EQ(A.width(), 640);
  EXPECT_EQ(A.height(), 240);
  EXPECT_FALSE(A.verbose());
  EXPECT_EQ(A.verbosity(), 0);

  array<const char *, 2> argsWL1 = {"--width", "123"};
  EXPECT_TRUE(A.parse(argsWL1.size(), argsWL1.data()));
  EXPECT_EQ(A.width(), 123);
  EXPECT_EQ(A.height(), 240);
  EXPECT_FALSE(A.verbose());
  EXPECT_EQ(A.verbosity(), 0);

  array<const char *, 1> argsWL2 = {"--width=456"};
  EXPECT_TRUE(A.parse(argsWL2.size(), argsWL2.data()));
  EXPECT_EQ(A.width(), 456);
  EXPECT_EQ(A.height(), 240);
  EXPECT_FALSE(A.verbose());
  EXPECT_EQ(A.verbosity(), 0);

  array<const char *, 2> argsH = {"-h", "640"};
  EXPECT_TRUE(A.parse(argsH.size(), argsH.data()));
  EXPECT_EQ(A.width(), 456);
  EXPECT_EQ(A.height(), 640);
  EXPECT_FALSE(A.verbose());
  EXPECT_EQ(A.verbosity(), 0);

  array<const char *, 2> argsHL1 = {"--height", "123"};
  EXPECT_TRUE(A.parse(argsHL1.size(), argsHL1.data()));
  EXPECT_EQ(A.width(), 456);
  EXPECT_EQ(A.height(), 123);
  EXPECT_FALSE(A.verbose());
  EXPECT_EQ(A.verbosity(), 0);

  array<const char *, 1> argsHL2 = {"--height=456"};
  EXPECT_TRUE(A.parse(argsHL2.size(), argsHL2.data()));
  EXPECT_EQ(A.width(), 456);
  EXPECT_EQ(A.height(), 456);
  EXPECT_FALSE(A.verbose());
  EXPECT_EQ(A.verbosity(), 0);

  array<const char *, 3> argsHWL = {"--height=123", "-w", "512"};
  EXPECT_TRUE(A.parse(argsHWL.size(), argsHWL.data()));
  EXPECT_EQ(A.width(), 512);
  EXPECT_EQ(A.height(), 123);
  EXPECT_FALSE(A.verbose());
  EXPECT_EQ(A.verbosity(), 0);

  array<const char *, 3> argsWHL = {"--width=123", "-h", "456"};
  EXPECT_TRUE(A.parse(argsWHL.size(), argsWHL.data()));
  EXPECT_EQ(A.width(), 123);
  EXPECT_EQ(A.height(), 456);
  EXPECT_FALSE(A.verbose());
  EXPECT_EQ(A.verbosity(), 0);
}

TEST(App, configureVerbosity) {
  {
    App A("myapp", "is wonderful.");
    array<const char *, 1> args1 = {"-v"};
    EXPECT_TRUE(A.parse(args1.size(), args1.data()));
    EXPECT_TRUE(A.verbose());
    EXPECT_EQ(A.verbosity(), 1);
  }

  {
    App A("myapp", "is wonderful.");
    array<const char *, 1> args1 = {"--verbose"};
    EXPECT_TRUE(A.parse(args1.size(), args1.data()));
    EXPECT_TRUE(A.verbose());
    EXPECT_EQ(A.verbosity(), 1);
  }

  {
    App A("myapp", "is wonderful.");
    array<const char *, 3> args1 = {"-v", "--verbose", "-v"};
    EXPECT_TRUE(A.parse(args1.size(), args1.data()));
    EXPECT_TRUE(A.verbose());
    EXPECT_EQ(A.verbosity(), 3);
  }
}

TEST(App, configureOutputFile) {
  {
    App A("myapp", "is wonderful.");
    array<const char *, 2> args1 = {"-o", "file.txt"};
    EXPECT_TRUE(A.parse(args1.size(), args1.data()));
    EXPECT_EQ(A.outputFormat(), App::PPM);
    EXPECT_EQ(A.outputFilename(), "file.txt");
  }
  {
    App A("myapp", "is wonderful.");
    array<const char *, 2> args1 = {"--output", "file2.txt"};
    EXPECT_TRUE(A.parse(args1.size(), args1.data()));
    EXPECT_EQ(A.outputFormat(), App::PPM);
    EXPECT_EQ(A.outputFilename(), "file2.txt");
  }
  {
    App A("myapp", "is wonderful.");
    array<const char *, 1> args1 = {"--output=file3.txt"};
    EXPECT_TRUE(A.parse(args1.size(), args1.data()));
    EXPECT_EQ(A.outputFormat(), App::PPM);
    EXPECT_EQ(A.outputFilename(), "file3.txt");
  }
}

TEST(App, configureOutputFormat) {
  {
    App A("myapp", "is wonderful.");
    array<const char *, 2> args1 = {"-f", "PPM"};
    EXPECT_TRUE(A.parse(args1.size(), args1.data()));
    EXPECT_EQ(A.outputFormat(), App::PPM);
  }
  {
    App A("myapp", "is wonderful.");
    array<const char *, 2> args1 = {"--format", "PPM"};
    EXPECT_TRUE(A.parse(args1.size(), args1.data()));
    EXPECT_EQ(A.outputFormat(), App::PPM);
  }
  {
    App A("myapp", "is wonderful.");
    array<const char *, 1> args1 = {"--format=PPM"};
    EXPECT_TRUE(A.parse(args1.size(), args1.data()));
    EXPECT_EQ(A.outputFormat(), App::PPM);
  }
  {
    App A("myapp", "is wonderful.");
    array<const char *, 2> args1 = {"--format", "PPM"};
    EXPECT_TRUE(A.parse(args1.size(), args1.data()));
    EXPECT_EQ(A.outputFormat(), App::PPM);
  }
}
