#include "gtest/gtest.h"

#include "ratrac/Patterns.h"
#include "ratrac/Shapes.h"

#include <memory>
#include <string>
#include <sstream>

using namespace ratrac;
using namespace testing;

using std::ostringstream;
using std::string;
using std::unique_ptr;

class TestPattern : public Pattern {
public:
  TestPattern() : Pattern() {}
  TestPattern(const TestPattern &) = default;
  virtual unique_ptr<Pattern> clone() const override {
    return unique_ptr<TestPattern>(new TestPattern(*this));
  }

  virtual Color local_at(const Tuple &point) const override {
    return Color(point.x(), point.y(), point.z());
  }

  virtual explicit operator string() const override {
    return "TestPattern {}";
  }
};

TEST(Patterns, base) {
  // Default pattern transformation
  TestPattern p;
  EXPECT_EQ(p.transform(), Matrix::identity());

  // Assigning a transformation
  p = TestPattern();
  p.transform(Matrix::translation(1, 2, 3));
  EXPECT_EQ(p.transform(), Matrix::translation(1, 2, 3));

  // Creating a stripe pattern
  Stripes s = Stripes(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(s.color1(), Color::WHITE());
  EXPECT_EQ(s.color2(), Color::BLACK());
}

TEST(Patterns, output) {
  Stripes s(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(
      string(s),
      "Stripes { a: Color { red:1, green:1, blue:1, alpha:1}, b: Color { "
      "red:0, green:0, blue:0, alpha:1}, transform: Matrix {    1.0,    0.0,  "
      "  0.0,    0.0},\n\t{    0.0,    1.0,    0.0,    0.0},\n\t{    0.0,    "
      "0.0,    1.0,    0.0},\n\t{    0.0,    0.0,    0.0,    1.0}}\n}");

  ostringstream oss;
  oss << s;
  EXPECT_EQ(
      oss.str(),
      "Stripes { a: Color { red:1, green:1, blue:1, alpha:1}, b: Color { "
      "red:0, green:0, blue:0, alpha:1}, transform: Matrix {    1.0,    0.0,  "
      "  0.0,    0.0},\n\t{    0.0,    1.0,    0.0,    0.0},\n\t{    0.0,    "
      "0.0,    1.0,    0.0},\n\t{    0.0,    0.0,    0.0,    1.0}}\n}");

  Gradient g(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(
      string(g),
      "Gradient { a: Color { red:1, green:1, blue:1, alpha:1}, b: Color { "
      "red:0, green:0, blue:0, alpha:1}, transform: Matrix {    1.0,    0.0,  "
      "  0.0,    0.0},\n\t{    0.0,    1.0,    0.0,    0.0},\n\t{    0.0,    "
      "0.0,    1.0,    0.0},\n\t{    0.0,    0.0,    0.0,    1.0}}\n}");

  oss.str("");
  oss << g;
  EXPECT_EQ(
      oss.str(),
      "Gradient { a: Color { red:1, green:1, blue:1, alpha:1}, b: Color { "
      "red:0, green:0, blue:0, alpha:1}, transform: Matrix {    1.0,    0.0,  "
      "  0.0,    0.0},\n\t{    0.0,    1.0,    0.0,    0.0},\n\t{    0.0,    "
      "0.0,    1.0,    0.0},\n\t{    0.0,    0.0,    0.0,    1.0}}\n}");

  Ring r(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(
      string(r),
      "Ring { a: Color { red:1, green:1, blue:1, alpha:1}, b: Color { "
      "red:0, green:0, blue:0, alpha:1}, transform: Matrix {    1.0,    0.0,  "
      "  0.0,    0.0},\n\t{    0.0,    1.0,    0.0,    0.0},\n\t{    0.0,    "
      "0.0,    1.0,    0.0},\n\t{    0.0,    0.0,    0.0,    1.0}}\n}");

  oss.str("");
  oss << r;
  EXPECT_EQ(
      oss.str(),
      "Ring { a: Color { red:1, green:1, blue:1, alpha:1}, b: Color { "
      "red:0, green:0, blue:0, alpha:1}, transform: Matrix {    1.0,    0.0,  "
      "  0.0,    0.0},\n\t{    0.0,    1.0,    0.0,    0.0},\n\t{    0.0,    "
      "0.0,    1.0,    0.0},\n\t{    0.0,    0.0,    0.0,    1.0}}\n}");

  Checkers c(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(
      string(c),
      "Checkers { a: Color { red:1, green:1, blue:1, alpha:1}, b: Color { "
      "red:0, green:0, blue:0, alpha:1}, transform: Matrix {    1.0,    0.0,  "
      "  0.0,    0.0},\n\t{    0.0,    1.0,    0.0,    0.0},\n\t{    0.0,    "
      "0.0,    1.0,    0.0},\n\t{    0.0,    0.0,    0.0,    1.0}}\n}");

  oss.str("");
  oss << c;
  EXPECT_EQ(
      oss.str(),
      "Checkers { a: Color { red:1, green:1, blue:1, alpha:1}, b: Color { "
      "red:0, green:0, blue:0, alpha:1}, transform: Matrix {    1.0,    0.0,  "
      "  0.0,    0.0},\n\t{    0.0,    1.0,    0.0,    0.0},\n\t{    0.0,    "
      "0.0,    1.0,    0.0},\n\t{    0.0,    0.0,    0.0,    1.0}}\n}");
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

  // A gradient linearly interpolates between colors
  Gradient g(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(g.at(Point(0, 0, 0)), Color::WHITE());
  EXPECT_EQ(g.at(Point(0.25, 0, 0)), Color(0.75, 0.75, 0.75));
  EXPECT_EQ(g.at(Point(0.5, 0, 0)), Color(0.5, 0.5, 0.5));
  EXPECT_EQ(g.at(Point(0.75, 0, 0)), Color(0.25, 0.25, 0.25));

  // A ring should extend in both x and z
  Ring r(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(r.at(Point(0, 0, 0)), Color::WHITE());
  EXPECT_EQ(r.at(Point(1, 0, 0)), Color::BLACK());
  EXPECT_EQ(r.at(Point(0, 0, 1)), Color::BLACK());
  EXPECT_EQ(r.at(Point(0.708, 0, 0.708)), Color::BLACK());

  // Checkers should repeat in x
  Checkers c(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(c.at(Point(0, 0, 0)), Color::WHITE());
  EXPECT_EQ(c.at(Point(0.99, 0, 0)), Color::WHITE());
  EXPECT_EQ(c.at(Point(1.01, 0, 0)), Color::BLACK());

  // Checkers should repeat in y
  c = Checkers(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(c.at(Point(0, 0, 0)), Color::WHITE());
  EXPECT_EQ(c.at(Point(0, 0.99, 0)), Color::WHITE());
  EXPECT_EQ(c.at(Point(0, 1.01, 0)), Color::BLACK());

  // Checkers should repeat in z
  c = Checkers(Color::WHITE(), Color::BLACK());
  EXPECT_EQ(c.at(Point(0, 0, 0)), Color::WHITE());
  EXPECT_EQ(c.at(Point(0, 0, 0.99)), Color::WHITE());
  EXPECT_EQ(c.at(Point(0, 0, 1.01)), Color::BLACK());
}

TEST(Patterns, transform) {
  Sphere s;
  Color c;

  // A pattern with an object transformation
  s = Sphere();
  s.transform(Matrix::scaling(2, 2, 2));
  s.material().pattern(TestPattern());
  c = s.at(Point(2, 3, 4));
  EXPECT_EQ(c, Color(1, 1.5, 2));

  // A pattern with a pattern transformation
  s = Sphere();
  TestPattern tp;
  tp.transform(Matrix::scaling(2, 2, 2));
  s.material().pattern(tp);
  c = s.at(Point(2, 3, 4));
  EXPECT_EQ(c, Color(1, 1.5, 2));

  // A pattern with both an object and a pattern transformation
  s = Sphere();
  s.transform(Matrix::scaling(2, 2, 2));
  tp = TestPattern();
  tp.transform(Matrix::translation(0.5, 1, 1.5));
  s.material().pattern(tp);
  c = s.at(Point(2.5, 3, 3.5));
  EXPECT_EQ(c, Color(0.75, 0.5, 0.25));

  // Stripes with an object transformation
  s = Sphere();
  s.transform(Matrix::scaling(2, 2, 2));
  s.material().pattern(Stripes(Color::WHITE(), Color::BLACK()));
  c = s.at(Point(1.5, 0, 0));
  EXPECT_EQ(c, Color::WHITE());

  // Stripes with a pattern transformation
  s = Sphere();
  Stripes p(Color::WHITE(), Color::BLACK());
  p.transform(Matrix::scaling(2, 2, 2));
  s.material().pattern(p);
  c = s.at(Point(1.5, 0, 0));
  EXPECT_EQ(c, Color::WHITE());

  // Stripes with both an object and a pattern transformation
  s = Sphere();
  s.transform(Matrix::scaling(2, 2, 2));
  p = Stripes(Color::WHITE(), Color::BLACK());
  p.transform(Matrix::translation(0.5, 0, 0));
  s.material().pattern(p);
  c = s.at(Point(2.5, 0, 0));
  EXPECT_EQ(c, Color::WHITE());
}
