TEST(Canvas, canvas) {
  // Testing canvas
  // ==============

  // Creating a canvas
  Canvas C(10, 20);
  EXPECT_EQ(C.width(), 10);
  EXPECT_EQ(C.height(), 20);

  // Writing pixels to a canvas
  Color red(1.0, 0.0, 0.0);
  C.at(2, 3) = red;
  EXPECT_EQ(C.at(2, 3), red);

  // Saving canvas
  // =============

  // Constructing the PPM header
  std::ostringstream sstr;
  C = Canvas(5, 3);
  C.to_ppm(sstr);
  EXPECT_EQ(sstr.str(),
            "P3\n5 3\n255\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 "
            "0 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");

  // Constructing the PPM pixel data
  sstr.str("");
  C = Canvas(5, 3);
  Color c1(1.5, 0.0, 0.0);
  Color c2(0.0, 0.5, 0.0);
  Color c3(-0.5, 0.0, 1.0);
  C.at(0, 0) = c1;
  C.at(2, 1) = c2;
  C.at(4, 2) = c3;
  C.to_ppm(sstr);
  EXPECT_EQ(sstr.str(),
            "P3\n5 3\n255\n255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 128 "
            "0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n");

  // Splitting long lines in PPM files
  sstr.str("");
  C = Canvas(10, 2, Color(1., 0.8, 0.6));
  C.to_ppm(sstr);
  EXPECT_EQ(
      sstr.str(),
      "P3\n10 2\n255\n255 204 153 255 204 153 255 204 153 255 204 153 255 204 "
      "153 255 204\n153 255 204 153 255 204 153 255 204 153 255 204 153\n255 "
      "204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n153 "
      "255 204 153 255 204 153 255 204 153 255 204 153\n");

  // PPM files are terminated by a newline character
  sstr.str("");
  C = Canvas(5, 3);
  C.to_ppm(sstr);
  std::string s = sstr.str();
  EXPECT_EQ(s[s.size() - 1], '\n');
}
