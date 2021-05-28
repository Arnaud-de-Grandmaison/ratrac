TEST(Patterns, base) {
  // Creating a stripe pattern
  Stripes s = Stripes(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(s.color1(), Color::WHITE());
  EXPECT_EQ(s.color2(), Color::BLACK());
}

TEST(Patterns, output) {
  Stripes s = Stripes(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(std::string(s),
            "Stripes { a: Color { red:1, green:1, blue:1, alpha:1}, b: Color { "
            "red:0, green:0, blue:0, alpha:1}}");
  std::ostringstream string_stream;
  string_stream << s;
  EXPECT_EQ(string_stream.str(),
            "Stripes { a: Color { red:1, green:1, blue:1, alpha:1}, b: Color { "
            "red:0, green:0, blue:0, alpha:1}}");
}

TEST(Patterns, at) {
  // A stripe pattern is constant in y
  Stripes s = Stripes(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(s.at(Point(0, 0, 0)), Color::WHITE());
  EXPECT_EQ(s.at(Point(0, 1, 0)), Color::WHITE());
  EXPECT_EQ(s.at(Point(0, 2, 0)), Color::WHITE());

  // A stripe pattern is constant in z
  s = Stripes(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(s.at(Point(0, 0, 0)), Color::WHITE());
  EXPECT_EQ(s.at(Point(0, 0, 1)), Color::WHITE());
  EXPECT_EQ(s.at(Point(0, 0, 2)), Color::WHITE());

  // A stripe pattern alternates in x
  s = Stripes(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(s.at(Point(0, 0, 0)), Color::WHITE());
  EXPECT_EQ(s.at(Point(0.9, 0, 0)), Color::WHITE());
  EXPECT_EQ(s.at(Point(1, 0, 0)), Color::BLACK());
  EXPECT_EQ(s.at(Point(-0.1, 0, 0)), Color::BLACK());
  EXPECT_EQ(s.at(Point(-1, 0, 0)), Color::BLACK());
  EXPECT_EQ(s.at(Point(-1.1, 0, 0)), Color::WHITE());
}
