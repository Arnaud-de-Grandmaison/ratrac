#include "gtest/gtest.h"

#include "ratrac/StopWatch.h"

#include <chrono>
#include <sstream>
#include <string>
#include <thread>

using namespace ratrac;
using namespace testing;

using std::ostringstream;
using std::string;

TEST(StopWatch, StopWatch) {
    StopWatch W;
    EXPECT_FALSE(W.isRunning());

    StopWatch::time_point start = W.start();
    EXPECT_TRUE(W.isRunning());
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    EXPECT_GT(W.now(), start);
    EXPECT_GT(W.elapsed(), 0.0);
    EXPECT_TRUE(W.isRunning());
    EXPECT_GT(W.stop(), start);
    EXPECT_FALSE(W.isRunning());
    EXPECT_GT(W.elapsed(), 0.0);
}

TEST(StopWatch, AutoStopWatch) {
    ostringstream oss;

    {
        AutoStopWatch ASW1("stopwatch1", oss);
    }
    string s = oss.str();
    EXPECT_EQ(s.substr(0, 12), "stopwatch1: ");
    EXPECT_EQ(s.substr(s.size()-2, 1), "s");

    oss.str("");
    {
        AutoStopWatch ASW2("stopwatch2", oss);
    }
    s = oss.str();
    EXPECT_EQ(s.substr(0, 12), "stopwatch2: ");
    EXPECT_EQ(s.substr(s.size()-2, 1), "s");

    oss.str("");
    {
        AutoStopWatch ASW3("stopwatch3", oss, true);
    }
    EXPECT_TRUE(oss.str().empty());
}