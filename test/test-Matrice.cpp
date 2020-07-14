#include "ratrac/Matrice.h"

#include "gtest/gtest.h"

#include <sstream>
#include <tuple>
#include <vector>

using namespace ratrac;
using namespace testing;

TEST(RayTracerMatrice, init) {
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

  // init Tests
  // ==========

  // 4*4 matrice

  matrice = {{1, 2, 3, 4},
             {5.5, 6.5, 7.5, 8.5},
             {9, 10, 11, 12},
             {13.5, 14.5, 15.5, 16.5}};
  M = matrice;
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

  matrice = {{1, 2, 3}, {5.5, 6.5, 7.5}, {9, 10, 11}};
  M = matrice;
  EXPECT_EQ(M.at(0, 0), 1);
  EXPECT_EQ(M.at(1, 0), 5.5);
  EXPECT_EQ(M.at(1, 2), 7.5);
  EXPECT_EQ(M.at(2, 2), 11);
  t = std::tuple<int, int>(3, 3);
  EXPECT_EQ(M.size(), t);

  // 2*2 Matrice

  matrice = {{1, 2}, {5.5, 6.5}};
  M = matrice;
  EXPECT_EQ(M.at(0, 0), 1);
  EXPECT_EQ(M.at(1, 0), 5.5);
  t = std::tuple<int, int>(2, 2);
  EXPECT_EQ(M.size(), t);
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
