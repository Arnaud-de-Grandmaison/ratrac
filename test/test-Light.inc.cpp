TEST(Light, base) {
  // A point of light has a position and intensity.
  Color intensity(1, 1, 1);
  Tuple position = Point(0, 0, 0);
  LightPoint lp(position, intensity);

  EXPECT_EQ(lp.position(), position);
  EXPECT_EQ(lp.intensity(), intensity);

  // Equality.
  EXPECT_TRUE(lp == LightPoint(position, intensity));
  EXPECT_FALSE(lp == LightPoint(Point(1, 1, 1), intensity));
  EXPECT_FALSE(lp == LightPoint(position, Color(0.5, 0.5, 0.5)));

  // Inequality.
  EXPECT_FALSE(lp != LightPoint(position, intensity));
  EXPECT_TRUE(lp != LightPoint(Point(1, 1, 1), intensity));
  EXPECT_TRUE(lp != LightPoint(position, Color(0.5, 0.5, 0.5)));
}

TEST(Light, output) {
  LightPoint lp(Point(0, 0, 0), Color::WHITE());
  std::ostringstream string_stream;
  string_stream << lp;
  EXPECT_EQ(string_stream.str(),
            "LightPoint { intensity: Color { red:1, green:1, blue:1, alpha:1}, "
            "position: Tuple { 0, 0, 0, 1}}");
}
