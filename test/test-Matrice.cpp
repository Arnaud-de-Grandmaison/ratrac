#include "ratrac/Matrice.h"
#include "ratrac/Tuple.h"

#include "gtest/gtest.h"

#include <sstream>
#include <tuple>
#include <vector>

using namespace ratrac;
using namespace testing;

TEST(RayTracerMatrice, init) {
  // init Tests
  // ==========

  // 4*4 matrice

  std::vector<std::vector<float>> matrice = {{1, 2, 3, 4},
                                             {5.5, 6.5, 7.5, 8.5},
                                             {9, 10, 11, 12},
                                             {13.5, 14.5, 15.5, 16.5}};
  Matrice M = matrice;
  EXPECT_EQ(M.at(0, 0), 1);
  EXPECT_EQ(M.at(0, 3), 4);
  EXPECT_EQ(M.at(1, 0), 5.5);
  EXPECT_EQ(M.at(1, 2), 7.5);
  EXPECT_EQ(M.at(2, 2), 11);
  EXPECT_EQ(M.at(3, 0), 13.5);
  EXPECT_EQ(M.at(3, 2), 15.5);
  std::tuple<int, int> t(4, 4);
  EXPECT_EQ(M.size(), t);

  // 3*3 Matrice

  matrice = {{-3, 5, 0}, {1, -2, -7}, {0, 1, 1}};
  M = matrice;
  EXPECT_EQ(M.at(0, 0), -3);
  EXPECT_EQ(M.at(1, 1), -2);
  EXPECT_EQ(M.at(2, 2), 1);
  t = std::tuple<int, int>(3, 3);
  EXPECT_EQ(M.size(), t);

  // 2*2 Matrice

  matrice = {{-3, 5}, {1, -2}};
  M = matrice;
  EXPECT_EQ(M.at(0, 0), -3);
  EXPECT_EQ(M.at(0, 1), 5);
  EXPECT_EQ(M.at(1, 0), 1);
  EXPECT_EQ(M.at(1, 1), -2);
  t = std::tuple<int, int>(2, 2);
  EXPECT_EQ(M.size(), t);
}

TEST(RayTracerMatrice, operators) {
  // << operator Test
  std::vector<std::vector<float>> matrice = {{1.0f, 1.0f, 1.0f, 1.0f},
                                             {1.0f, 1.0f, 1.0f, 1.0f},
                                             {1.0f, 1.0f, 1.0f, 1.0f},
                                             {1.0f, 1.0f, 1.0f, 1.0f}};
  Matrice M(matrice);
  std::ostringstream string_stream;
  string_stream << M;
  EXPECT_EQ(string_stream.str(), "Matrice {    1.0,    1.0,    1.0,    1.0},\n"
                                 "\t{    1.0,    1.0,    1.0,    1.0},\n"
                                 "\t{    1.0,    1.0,    1.0,    1.0},\n"
                                 "\t{    1.0,    1.0,    1.0,    1.0}}\n");

  // Equity test

  std::vector<std::vector<float>> matrice1 = {
      {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
  Matrice M1 = matrice1;
  std::vector<std::vector<float>> matrice2 = {
      {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
  Matrice M2 = matrice2;
  EXPECT_EQ(M1, M2);
  matrice2 = {{2, 3, 4, 5}, {6, 7, 8, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}};
  M2 = matrice2;
  EXPECT_NE(M1, M2);

  // Mutliply test

  // Multiply Matrices

  matrice1 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
  M1 = matrice1;
  matrice2 = {{-2, 1, 2, 3}, {3, 2, 1, -1}, {4, 3, 6, 5}, {1, 2, 7, 8}};
  M2 = matrice2;
  matrice = {{20, 22, 50, 48},
             {44, 54, 114, 108},
             {40, 58, 110, 102},
             {16, 26, 46, 42}};
  M1 *= M2;
  EXPECT_EQ(M1, Matrice(matrice));
  M1 = matrice1;
  EXPECT_EQ(M1 * M2, Matrice(matrice));

  // Multiply a Matrice by a Tuple

  matrice1 = {{1, 2, 3, 4}, {2, 4, 4, 2}, {8, 6, 4, 1}, {0, 0, 0, 1}};
  M = matrice1;
  Tuple b(1, 2, 3, 1);
  EXPECT_EQ(M * b, Tuple(18, 24, 33, 1));
}

TEST(RayTracerMatrice, other_stuff) {
  // Identity matrice tests

  Matrice M(std::vector<std::vector<float>>(
      {{0, 1, 2, 4}, {1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}}));
  EXPECT_EQ(M * identity_matrix(), M);
  Tuple a(1., 2., 3., 4.);
  EXPECT_EQ(identity_matrix() * a, a);

  M = std::vector<std::vector<float>>(
      {{0, 9, 3, 0}, {9, 8, 0, 8}, {1, 8, 5, 3}, {0, 0, 5, 8}});
  EXPECT_EQ(transpose(M),
            Matrice({{0, 9, 1, 0}, {9, 8, 8, 0}, {3, 0, 5, 5}, {0, 8, 3, 8}}));
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
