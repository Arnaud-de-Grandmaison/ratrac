#include "gtest/gtest.h"

#include "ratrac/ProgressBar.h"

#include <sstream>
#include <string>

using namespace ratrac;
using namespace testing;

using std::ostringstream;
using std::string;

TEST(ProgressBar, ProgressBar) {
    ostringstream oss;
    ProgressBar PB1("caption", 100, oss);
    EXPECT_EQ(oss.str(), "\rcaption: 0%");

    oss.str("");
    PB1.incr();
    EXPECT_EQ(oss.str(), "\rcaption: 1%");

    oss.str("");
    PB1.incr();
    EXPECT_EQ(oss.str(), "\rcaption: 2%");

    oss.str("");
    PB1.incr(10);
    EXPECT_EQ(oss.str(), "\rcaption: 12%");

    oss.str("");
    ProgressBar PB2("caption", 100, oss, false);
    EXPECT_EQ(oss.str(), "\rcaption: 0%");

    oss.str("");
    PB2.incr();
    EXPECT_EQ(oss.str(), "\rcaption: 1%");

    oss.str("");
    PB2.incr();
    EXPECT_EQ(oss.str(), "\rcaption: 2%");

    oss.str("");
    PB2.incr(20);
    EXPECT_EQ(oss.str(), "\rcaption: 22%");

    oss.str("");
    ProgressBar PB3("caption", 100, oss, true);
    EXPECT_EQ(oss.str(), "");
    PB3.incr(10);
    EXPECT_EQ(oss.str(), "");
}

TEST(ProgressBar, TimedProgressBar) {
    ostringstream oss;
    {
        TimedProgressBar TPB1("caption", 100, oss);
        EXPECT_EQ(oss.str(), "\rcaption: 0%");

        oss.str("");
        TPB1.incr();
        EXPECT_EQ(oss.str(), "\rcaption: 1%");

        oss.str("");
        TPB1.incr();
        EXPECT_EQ(oss.str(), "\rcaption: 2%");

        oss.str("");
        TPB1.incr(10);
        EXPECT_EQ(oss.str(), "\rcaption: 12%");
    }
    string s = oss.str();
    EXPECT_EQ(s.substr(0, 15), "\rcaption: 12% (");
    EXPECT_EQ(s.substr(s.size()-3, 2), "s)");

    oss.str("");
    {
        TimedProgressBar TPB2("caption", 100, oss, false);
        EXPECT_EQ(oss.str(), "\rcaption: 0%");

        oss.str("");
        TPB2.incr();
        EXPECT_EQ(oss.str(), "\rcaption: 1%");

        oss.str("");
        TPB2.incr(10);
        EXPECT_EQ(oss.str(), "\rcaption: 11%");

        oss.str("");
        TPB2.incr(10);
        EXPECT_EQ(oss.str(), "\rcaption: 21%");
    }
    s = oss.str();
    EXPECT_EQ(s.substr(0, 15), "\rcaption: 21% (");
    EXPECT_EQ(s.substr(s.size()-3, 2), "s)");

    oss.str("");
    {
        TimedProgressBar TPB3("caption", 100, oss, true);
        EXPECT_TRUE(oss.str().empty());

        TPB3.incr();
        EXPECT_TRUE(oss.str().empty());

        TPB3.incr(10);
        EXPECT_TRUE(oss.str().empty());
    }
    EXPECT_TRUE(oss.str().empty());
}
