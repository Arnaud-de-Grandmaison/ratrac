TEST(Matrice, init) {
  // init Tests
  // ==========

  // 4*4 matrice

  Matrice M({{1., 2., 3., 4.},
             {5.5, 6.5, 7.5, 8.5},
             {9., 10., 11., 12.},
             {13.5, 14.5, 15.5, 16.5}});
  EXPECT_EQ(M.at(0, 0), 1);
  EXPECT_EQ(M.at(0, 3), 4);
  EXPECT_EQ(M.at(1, 0), 5.5);
  EXPECT_EQ(M.at(1, 2), 7.5);
  EXPECT_EQ(M.at(2, 2), 11);
  EXPECT_EQ(M.at(3, 0), 13.5);
  EXPECT_EQ(M.at(3, 2), 15.5);
  std::tuple<int, int> t(4, 4);
  EXPECT_EQ(M.shape(), t);
  EXPECT_EQ(M.getNumColumns(), 4);
  EXPECT_EQ(M.getNumLines(), 4);

  // 3*3 Matrice

  M = {{-3, 5, 0}, {1, -2, -7}, {0, 1, 1}};
  EXPECT_EQ(M.at(0, 0), -3);
  EXPECT_EQ(M.at(1, 1), -2);
  EXPECT_EQ(M.at(2, 2), 1);
  t = std::tuple<int, int>(3, 3);
  EXPECT_EQ(M.shape(), t);
  EXPECT_EQ(M.getNumColumns(), 3);
  EXPECT_EQ(M.getNumLines(), 3);

  // 2*2 Matrice

  M = {{-3, 5}, {1, -2}};
  EXPECT_EQ(M.at(0, 0), -3);
  EXPECT_EQ(M.at(0, 1), 5);
  EXPECT_EQ(M.at(1, 0), 1);
  EXPECT_EQ(M.at(1, 1), -2);
  t = std::tuple<int, int>(2, 2);
  EXPECT_EQ(M.shape(), t);
  EXPECT_EQ(M.getNumColumns(), 2);
  EXPECT_EQ(M.getNumLines(), 2);
}

TEST(Matrice, output) {
  Matrice M({{1.0f, 1.0f, 1.0f, 1.0f},
             {1.0f, 1.0f, 1.0f, 1.0f},
             {1.0f, 1.0f, 1.0f, 1.0f},
             {1.0f, 1.0f, 1.0f, 1.0f}});

  // String conversion
  EXPECT_EQ(std::string(M), "Matrice {    1.0,    1.0,    1.0,    1.0},\n"
                            "\t{    1.0,    1.0,    1.0,    1.0},\n"
                            "\t{    1.0,    1.0,    1.0,    1.0},\n"
                            "\t{    1.0,    1.0,    1.0,    1.0}}\n");

  // << operator Test
  std::ostringstream string_stream;
  string_stream << M;
  EXPECT_EQ(string_stream.str(), "Matrice {    1.0,    1.0,    1.0,    1.0},\n"
                                 "\t{    1.0,    1.0,    1.0,    1.0},\n"
                                 "\t{    1.0,    1.0,    1.0,    1.0},\n"
                                 "\t{    1.0,    1.0,    1.0,    1.0}}\n");
}

TEST(Matrice, operators) {
  // Equality test

  Matrice M1({{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}});
  Matrice M2({{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}});
  EXPECT_EQ(M1, M2);
  M2 = {{2, 3, 4, 5}, {6, 7, 8, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}};
  EXPECT_NE(M1, M2);

  // Mutliply test

  // Multiply Matrices

  Matrice matrice1 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
  M1 = matrice1;
  M2 = {{-2, 1, 2, 3}, {3, 2, 1, -1}, {4, 3, 6, 5}, {1, 2, 7, 8}};
  const Matrice result({{20, 22, 50, 48},
                        {44, 54, 114, 108},
                        {40, 58, 110, 102},
                        {16, 26, 46, 42}});
  M1 *= M2;
  EXPECT_EQ(M1, result);
  M1 = matrice1;
  EXPECT_EQ(M1 * M2, result);

  // Multiply a Matrice by a Tuple

  Matrice M = {{1, 2, 3, 4}, {2, 4, 4, 2}, {8, 6, 4, 1}, {0, 0, 0, 1}};
  Tuple b(1., 2., 3., 1.);
  EXPECT_EQ(M * b, Tuple(18., 24., 33., 1.));
}

TEST(Matrice, other_stuff) {
  // Identity matrice tests

  Matrice M({{0, 1, 2, 4}, {1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}});
  EXPECT_EQ(M * Matrice::identity(), M);
  Tuple a(1., 2., 3., 4.);
  EXPECT_EQ(Matrice::identity() * a, a);

  M = {{0, 9, 3, 0}, {9, 8, 0, 8}, {1, 8, 5, 3}, {0, 0, 5, 8}};
  EXPECT_EQ(transpose(M),
            Matrice({{0, 9, 1, 0}, {9, 8, 8, 0}, {3, 0, 5, 5}, {0, 8, 3, 8}}));
  M.transpose();
  EXPECT_EQ(M,
            Matrice({{0, 9, 1, 0}, {9, 8, 8, 0}, {3, 0, 5, 5}, {0, 8, 3, 8}}));

  EXPECT_EQ(transpose(Matrice::identity()), Matrice::identity());
  EXPECT_EQ(Matrice::identity().transpose(), Matrice::identity());

  // Inverting Matrices
  // ==================

  // Calculating the determinant of a 2*2 matrix
  M = {{1, 5}, {-3, 2}};
  EXPECT_EQ(determinant(M), 17);
  EXPECT_EQ(M.determinant(), 17);

  // A submatrix of a 3*3 matrix is a 2*2 matrix
  M = {{1, 5, 0}, {-3, 2, 7}, {0, 6, -3}};
  EXPECT_EQ(submatrix(M, 0, 2), Matrice({{-3, 2}, {0, 6}}));
  EXPECT_EQ(M.submatrix(0, 2), Matrice({{-3, 2}, {0, 6}}));

  // A submatrix of a 4*4 matrix is a 3*3 matrix
  M = {{-6, 1, 1, 6}, {-8, 5, 8, 6}, {-1, 0, 8, 2}, {-7, 1, -1, 1}};
  EXPECT_EQ(submatrix(M, 2, 1), Matrice({{-6, 1, 6}, {-8, 8, 6}, {-7, -1, 1}}));
  EXPECT_EQ(M.submatrix(2, 1), Matrice({{-6, 1, 6}, {-8, 8, 6}, {-7, -1, 1}}));

  // Calculating a minor of a 3*3 matrix
  M = {{3, 5, 0}, {2, -1, -7}, {6, -1, 5}};
  Matrice M2 = submatrix(M, 1, 0);
  EXPECT_EQ(determinant(M2), 25);
  EXPECT_EQ(matrixminor(M, 1, 0), 25);

  EXPECT_EQ(M2.determinant(), 25);
  EXPECT_EQ(M.matrixminor(1, 0), 25);

  // Calculating the cofactor of a 3*3 matrix
  M = {{3, 5, 0}, {2, -1, -7}, {6, -1, 5}};
  EXPECT_EQ(matrixminor(M, 0, 0), -12);
  EXPECT_EQ(cofactor(M, 0, 0), -12);
  EXPECT_EQ(matrixminor(M, 1, 0), 25);
  EXPECT_EQ(cofactor(M, 1, 0), -25);

  EXPECT_EQ(M.matrixminor(0, 0), -12);
  EXPECT_EQ(M.cofactor(0, 0), -12);
  EXPECT_EQ(M.matrixminor(1, 0), 25);
  EXPECT_EQ(M.cofactor(1, 0), -25);

  // Calculating the determinant of a 3*3 matrix
  M = {{1, 2, 6}, {-5, 8, -4}, {2, 6, 4}};
  EXPECT_EQ(cofactor(M, 0, 0), 56);
  EXPECT_EQ(cofactor(M, 0, 1), 12);
  EXPECT_EQ(cofactor(M, 0, 2), -46);
  EXPECT_EQ(determinant(M), -196);

  EXPECT_EQ(M.cofactor(0, 0), 56);
  EXPECT_EQ(M.cofactor(0, 1), 12);
  EXPECT_EQ(M.cofactor(0, 2), -46);
  EXPECT_EQ(M.determinant(), -196);

  // Calculating the determinant of a 4*4 matrix
  M = {{-2, -8, 3, 5}, {-3, 1, 7, 3}, {1, 2, -9, 6}, {-6, 7, 7, -9}};
  EXPECT_EQ(cofactor(M, 0, 0), 690);
  EXPECT_EQ(cofactor(M, 0, 1), 447);
  EXPECT_EQ(cofactor(M, 0, 2), 210);
  EXPECT_EQ(cofactor(M, 0, 3), 51);
  EXPECT_EQ(determinant(M), -4071);

  EXPECT_EQ(M.cofactor(0, 0), 690);
  EXPECT_EQ(M.cofactor(0, 1), 447);
  EXPECT_EQ(M.cofactor(0, 2), 210);
  EXPECT_EQ(M.cofactor(0, 3), 51);
  EXPECT_EQ(M.determinant(), -4071);

  // Testing an invertible matrix for invertibility
  M = {{6, 4, 4, 4}, {5, 5, 7, 6}, {4, -9, 3, -7}, {9, 1, 7, -6}};
  EXPECT_EQ(determinant(M), -2120);
  EXPECT_TRUE(is_invertible(M));

  EXPECT_EQ(M.determinant(), -2120);
  EXPECT_TRUE(M.is_invertible());

  // Testing a noninvertible matrix for invertibility
  M = {{-4, 2, -2, -3}, {9, 6, 2, 6}, {0, -5, 1, -5}, {0, 0, 0, 0}};
  EXPECT_EQ(determinant(M), 0);
  EXPECT_FALSE(is_invertible(M));

  EXPECT_EQ(M.determinant(), 0);
  EXPECT_FALSE(M.is_invertible());

  // Calculating the inverse of a matrix
  M = {{-5, 2, 6, -8}, {1, -5, 1, 8}, {7, 7, -6, -7}, {1, -3, 7, 4}};
  M2 = inverse(M);
  EXPECT_EQ(determinant(M), 532);
  EXPECT_EQ(cofactor(M, 2, 3), -160);
  Matrice::DataType value_f = -160;
  value_f /= 532;
  EXPECT_TRUE(close_to_equal(M2.at(3, 2), value_f)); // float(-160 / 532));
  EXPECT_EQ(cofactor(M, 3, 2), 105);
  value_f = 105;
  value_f /= 532;
  EXPECT_TRUE(close_to_equal(M2.at(2, 3), value_f)); // float(105 / 532));
  EXPECT_TRUE(M2.approximatly_equal(
      Matrice({{0.21805, 0.45113, 0.24060, -0.04511},
               {-0.80827, -1.45677, -0.44361, 0.52068},
               {-0.07895, -0.22368, -0.05263, 0.19737},
               {-0.52256, -0.81391, -0.30075, 0.30639}})));

  // Calculating the invert of another matrix
  M = {{8, -5, 9, 2}, {7, 5, 6, 1}, {-6, 0, 9, 6}, {-3, 0, -9, -4}};
  EXPECT_TRUE(inverse(M).approximatly_equal(
      Matrice({{-0.15385, -0.15385, -0.28205, -0.53846},
               {-0.07692, 0.12308, 0.02564, 0.03077},
               {0.35897, 0.35897, 0.43590, 0.92308},
               {-0.69231, -0.69231, -0.76923, -1.92308}})));

  EXPECT_TRUE(M.inverse().approximatly_equal(
      Matrice({{-0.15385, -0.15385, -0.28205, -0.53846},
               {-0.07692, 0.12308, 0.02564, 0.03077},
               {0.35897, 0.35897, 0.43590, 0.92308},
               {-0.69231, -0.69231, -0.76923, -1.92308}})));

  // Calculating the invert of a third matrix
  M = {{9, 3, 0, 9}, {-5, -2, -6, -3}, {-4, 9, 6, 4}, {-7, 6, 6, 2}};
  EXPECT_TRUE(inverse(M).approximatly_equal(
      Matrice({{-0.04074, -0.07778, 0.14444, -0.22222},
               {-0.07778, 0.03333, 0.36667, -0.33333},
               {-0.02901, -0.14630, -0.10926, 0.12963},
               {0.17778, 0.06667, -0.26667, 0.33333}})));

  EXPECT_TRUE(M.inverse().approximatly_equal(
      Matrice({{-0.04074, -0.07778, 0.14444, -0.22222},
               {-0.07778, 0.03333, 0.36667, -0.33333},
               {-0.02901, -0.14630, -0.10926, 0.12963},
               {0.17778, 0.06667, -0.26667, 0.33333}})));

  // Multiplying a product by its inverse
  M = {{3, -9, 7, 3}, {3, -8, 2, -9}, {-4, 4, 4, 1}, {-6, 5, -1, 1}};
  M2 = {{8, 2, 2, 2}, {3, -1, 7, 0}, {7, 0, 5, 4}, {6, -2, 0, 5}};
  Matrice M3 = M * M2;
  EXPECT_TRUE((M3 * inverse(M2)).approximatly_equal(M));
  EXPECT_TRUE((M3 * M2.inverse()).approximatly_equal(M));
}

TEST(Matrice, transformations) {
  // Translation matrix
  // ==================

  // Mutiplying by a translating matrix
  Matrice transform = Matrice::translation(5., -3., 2.);
  Tuple p = Point(-3., 4., 5.);
  EXPECT_EQ(transform * p, Point(2., 1., 7.));

  // Multiplying by the inverse of a translation matrix
  transform = Matrice::translation(5., -3., 2.);
  Matrice inv = inverse(transform);
  p = Point(-3., 4., 5.);
  EXPECT_EQ(inv * p, Point(-8., 7., 3.));

  // Translation does not affect vectors
  transform = Matrice::translation(5., -3., 2.);
  Tuple v = Vector(-3., 4., 5.);
  EXPECT_EQ(transform * v, v);

  // Scaling matrix
  // ==============

  // A scaling matrix applied to a point
  transform = Matrice::scaling(2., 3., 4.);
  p = Point(-4., 6., 8.);
  EXPECT_EQ(transform * p, Point(-8., 18., 32.));

  // A scaling matrix applied to a vector
  // transform = scaling(2, 3, 4);
  v = Vector(-4., 6., 8.);
  EXPECT_EQ(transform * v, Vector(-8., 18., 32.));

  // Multiplying by the inverse of a scaling matrix
  transform = Matrice::scaling(2., 3., 4.);
  inv = inverse(transform);
  v = Vector(-4., 6., 8.);
  EXPECT_EQ(inv * v, Vector(-2., 2., 2.));

  // Relfection is scaling by a negative value
  transform = Matrice::scaling(-1., 1., 1.);
  p = Point(2., 3., 4.);
  EXPECT_EQ(transform * p, Point(-2., 3., 4.));

  // Rotating
  // ========

  // X axis

  // Rotating a point around the x axis
  p = Point(0., 1., 0.);
  Matrice half_quarter = Matrice::rotation_x(M_PI / 4);
  Matrice full_quarter = Matrice::rotation_x(M_PI / 2);
  EXPECT_EQ(half_quarter * p, Point(0., std::sqrt(2) / 2, std::sqrt(2) / 2));

  /// The inverse of an x-rotation rotates in the opposite direction
  p = Point(0., 1., 0.);
  half_quarter = Matrice::rotation_x(M_PI / 4);
  inv = inverse(half_quarter);
  EXPECT_EQ(inv * p, Point(0., std::sqrt(2) / 2, -std::sqrt(2) / 2));

  // Y axis

  // Rotating a point around the y axis
  p = Point(0, 0, 1);
  half_quarter = Matrice::rotation_y(M_PI / 4);
  full_quarter = Matrice::rotation_y(M_PI / 2);
  EXPECT_EQ(half_quarter * p, Point(std::sqrt(2) / 2, 0, std::sqrt(2) / 2));
  EXPECT_EQ(full_quarter * p, Point(1, 0, 0));
  // Z axis

  // Rotating a point arround the z axis
  p = Point(0, 1, 0);
  half_quarter = Matrice::rotation_z(M_PI / 4);
  full_quarter = Matrice::rotation_z(M_PI / 2);
  EXPECT_EQ(half_quarter * p, Point(-std::sqrt(2) / 2, std::sqrt(2) / 2, 0));
  EXPECT_EQ(full_quarter * p, Point(-1, 0, 0));

  // Shearing
  // ========

  // A shearing transformation moves x in proportion to y
  transform = Matrice::shearing(1, 0, 0, 0, 0, 0);
  p = Point(2, 3, 4);
  EXPECT_EQ(transform * p, Point(5, 3, 4));

  // A shearing transformation moves x in proportion to z
  transform = Matrice::shearing(0, 1, 0, 0, 0, 0);
  p = Point(2, 3, 4);
  EXPECT_EQ(transform * p, Point(6, 3, 4));

  // A shearing transformation moves y in proportion to x
  transform = Matrice::shearing(0, 0, 1, 0, 0, 0);
  p = Point(2, 3, 4);
  EXPECT_EQ(transform * p, Point(2, 5, 4));

  // A shearing transformation moves y in proportion to z
  transform = Matrice::shearing(0, 0, 0, 1, 0, 0);
  p = Point(2, 3, 4);
  EXPECT_EQ(transform * p, Point(2, 7, 4));

  // A shearing transformation moves z in proportion to x
  transform = Matrice::shearing(0, 0, 0, 0, 1, 0);
  p = Point(2, 3, 4);
  EXPECT_EQ(transform * p, Point(2, 3, 6));

  // A shearing transformation moves x in proportion to z
  transform = Matrice::shearing(0, 0, 0, 0, 0, 1);
  p = Point(2, 3, 4);
  EXPECT_EQ(transform * p, Point(2, 3, 7));

  // Chaining transformations
  // ========================

  // Individual transformations are applied in sequence
  // --------------------------------------------------

  p = Point(1, 0, 1);
  Matrice A = Matrice::rotation_x(M_PI / 2);
  Matrice B = Matrice::scaling(5, 5, 5);
  Matrice C = Matrice::translation(10, 5, 7);
  // Apply rotation first
  Tuple p2 = A * p;
  EXPECT_EQ(p2, Point(1, -1, 0));
  // Then apply scaling
  Tuple p3 = B * p2;
  EXPECT_EQ(p3, Point(5, -5, 0));
  // Then apply translation
  Tuple p4 = C * p3;
  EXPECT_EQ(p4, Point(15, 0, 7));

  // Chained transformations must be applied in reverse order
  // --------------------------------------------------------

  p = Point(1, 0, 1);
  A = Matrice::rotation_x(M_PI / 2);
  B = Matrice::scaling(5, 5, 5);
  C = Matrice::translation(10, 5, 7);
  Matrice T = C * B * A;
  EXPECT_EQ(T * p, Point(15, 0, 7));
}
