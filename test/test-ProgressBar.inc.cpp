TEST(ProgressBar, ProgressBar) {
    std::ostringstream oss;
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
