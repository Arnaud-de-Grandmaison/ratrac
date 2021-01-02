TEST(Color, base) {
  // Testing same operations on colors.
  // ==================================

  // Colors are red, green, blue tuples.
  Color c(-0.5, 0.4, 1.7);
  EXPECT_EQ(c.red(), -0.5f);
  EXPECT_EQ(c.green(), 0.4f);
  EXPECT_EQ(c.blue(), 1.7f);

  // Adding colors.
  Color c1(0.9, 0.6, 0.75);
  Color c2(0.7, 0.1, 0.25);
  EXPECT_EQ(c1 + c2, Color(1.6, 0.7, 1.0));

  // Subtracting colors.
  c1 = Color(0.9, 0.6, 0.75);
  c2 = Color(0.7, 0.1, 0.25);
  EXPECT_EQ(c1 - c2, Color(0.2, 0.5, 0.5));

  // Multiplying a color by a scalar.
  c = Color(0.2, 0.3, 0.4);
  EXPECT_EQ(c * 2.0, Color(0.4, 0.6, 0.8));
  EXPECT_EQ(2.0 * c, Color(0.4, 0.6, 0.8));

  // Dividing a color by a scalar.
  c = Color(0.4, 0.6, 0.8);
  EXPECT_EQ(c / 2.0, Color(0.2, 0.3, 0.4));

  // Multiplying colors.
  c1 = Color(1.0, 0.2, 0.4);
  c2 = Color(0.9, 1.0, 0.1);
  EXPECT_EQ(c1 * c2, Color(0.9, 0.2, 0.04));

  // Multiplying colors/Hadamard product/Schur product
}

TEST(Color, output) {
  Color c1(0.9, 0.6, 0.75);
  std::ostringstream string_stream;
  string_stream << c1;
  EXPECT_EQ(string_stream.str(),
            "Color { red:0.9, green:0.6, blue:0.75, alpha:1}");

  string_stream.str("");
  Color c2(0.7, 0.1, 0.25, 0.5);
  string_stream << c2;
  EXPECT_EQ(string_stream.str(),
            "Color { red:0.7, green:0.1, blue:0.25, alpha:0.5}");
}

TEST(Color, helpers) {
  EXPECT_EQ(Color::BLACK(), Color(0, 0, 0));
  EXPECT_EQ(Color::WHITE(), Color(1, 1, 1));
  EXPECT_EQ(Color::RED(), Color(1, 0, 0));
  EXPECT_EQ(Color::GREEN(), Color(0, 1, 0));
  EXPECT_EQ(Color::BLUE(), Color(0, 0, 1));
}
