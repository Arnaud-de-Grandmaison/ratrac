#pragma once

#include "ratrac/Color.h"
#include "ratrac/Matrix.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

#include <cmath>
#include <memory>
#include <ostream>
#include <string>

namespace ratrac {

class Pattern {
public:
  Pattern()
      : m_transform(Matrix::identity()),
        m_inverted_transform(Matrix::identity()) {}
  Pattern(const Matrix &t)
      : m_transform(t), m_inverted_transform(inverse(m_transform)) {}
  Pattern(Matrix &&t)
      : m_transform(std::move(t)), m_inverted_transform(inverse(m_transform)) {}
  virtual ~Pattern();

  virtual std::unique_ptr<Pattern> clone() const = 0;

  Color at(const Tuple &point) const {
    Tuple local_point = m_inverted_transform * point;
    return local_at(local_point);
  }

  virtual Color local_at(const Tuple &point) const = 0;

  virtual explicit operator std::string() const { return "Pattern {}"; }

  const Matrix &transform() const { return m_transform; }

  Pattern &transform(const Matrix &M) {
    m_transform = M;
    precompute();
    return *this;
  }

  Pattern &transform(Matrix &&M) {
    m_transform = std::move(M);
    precompute();
    return *this;
  }

private:
  Matrix m_transform;
  Matrix m_inverted_transform;

  void precompute() { m_inverted_transform = inverse(m_transform); }
};

class BiColorPattern : public Pattern {
public:
  BiColorPattern(const BiColorPattern &) = default;
  BiColorPattern(const Color &a, const Color &b) : Pattern(), a(a), b(b) {}
  BiColorPattern(const Color &a, const Color &b, const Matrix &t)
      : Pattern(t), a(a), b(b) {}
  BiColorPattern(const Color &a, const Color &b, Matrix &&t)
      : Pattern(t), a(a), b(b) {}

  const Color &color1() const { return a; }
  const Color &color2() const { return b; }

private:
  Color a;
  Color b;
};

class Stripes final : public BiColorPattern {
public:
  Stripes(const Stripes &) = default;
  Stripes(const Color &a, const Color &b) : BiColorPattern(a, b) {}
  Stripes(const Color &a, const Color &b, const Matrix &t)
      : BiColorPattern(a, b, t) {}
  Stripes(const Color &a, const Color &b, Matrix &&t)
      : BiColorPattern(a, b, t) {}

  virtual std::unique_ptr<Pattern> clone() const override {
    return std::unique_ptr<Stripes>(new Stripes(*this));
  }

  virtual Color local_at(const Tuple &point) const override {
    return long(std::floor(point.x())) % 2 == 0 ? color1() : color2();
  }

  virtual explicit operator std::string() const override;
};

class Gradient final : public BiColorPattern {
public:
  Gradient(const Gradient &) = default;
  Gradient(const Color &a, const Color &b) : BiColorPattern(a, b) {}
  Gradient(const Color &a, const Color &b, const Matrix &t)
      : BiColorPattern(a, b, t) {}
  Gradient(const Color &a, const Color &b, Matrix &&t)
      : BiColorPattern(a, b, t) {}

  virtual std::unique_ptr<Pattern> clone() const override {
    return std::unique_ptr<Gradient>(new Gradient(*this));
  }

  virtual Color local_at(const Tuple &point) const override {
    Color distance = color2() - color1();
    return color1() + distance * (point.x() - std::floor(point.x()));
  }

  virtual explicit operator std::string() const override;
};

class Ring final : public BiColorPattern {
public:
  Ring(const Ring &) = default;
  Ring(const Color &a, const Color &b) : BiColorPattern(a, b) {}
  Ring(const Color &a, const Color &b, const Matrix &t)
      : BiColorPattern(a, b, t) {}
  Ring(const Color &a, const Color &b, Matrix &&t) : BiColorPattern(a, b, t) {}

  virtual std::unique_ptr<Pattern> clone() const override {
    return std::unique_ptr<Ring>(new Ring(*this));
  }

  virtual Color local_at(const Tuple &point) const override {
    long m = long(
        std::floor(std::sqrt(point.x() * point.x() + point.z() * point.z())));
    return m % 2 == 0 ? color1() : color2();
  }

  virtual explicit operator std::string() const override;
};

class ColorCheckers final : public BiColorPattern {
public:
  ColorCheckers(const ColorCheckers &) = default;
  ColorCheckers(const Color &a, const Color &b) : BiColorPattern(a, b) {}
  ColorCheckers(const Color &a, const Color &b, const Matrix &t)
      : BiColorPattern(a, b, t) {}
  ColorCheckers(const Color &a, const Color &b, Matrix &&t)
      : BiColorPattern(a, b, t) {}

  virtual std::unique_ptr<Pattern> clone() const override {
    return std::unique_ptr<ColorCheckers>(new ColorCheckers(*this));
  }

  virtual Color local_at(const Tuple &point) const override {
    long m = long(std::floor(point.x()) + std::floor(point.y()) +
                  std::floor(point.z()));
    return m % 2 == 0 ? color1() : color2();
  }

  virtual explicit operator std::string() const override;
};

class RadialGradient final : public BiColorPattern {
public:
  RadialGradient(const RadialGradient &) = default;
  RadialGradient(const Color &a, const Color &b) : BiColorPattern(a, b) {}
  RadialGradient(const Color &a, const Color &b, const Matrix &t)
      : BiColorPattern(a, b, t) {}
  RadialGradient(const Color &a, const Color &b, Matrix &&t)
      : BiColorPattern(a, b, t) {}

  virtual std::unique_ptr<Pattern> clone() const override {
    return std::unique_ptr<RadialGradient>(new RadialGradient(*this));
  }

  virtual Color local_at(const Tuple &point) const override {
    Color distance = color2() - color1();
    auto m = magnitude(point - Point(0, 0, 0));
    return color1() + distance * (m - std::floor(m));
  }

  virtual explicit operator std::string() const override;
};

class BiPattern : public Pattern {
public:
  BiPattern(const BiPattern &) = delete;
  BiPattern(Pattern *a, Pattern *b) : Pattern(), a(a), b(b) {}
  BiPattern(Pattern *a, Pattern *b, const Matrix &t) : Pattern(t), a(a), b(b) {}
  BiPattern(std::unique_ptr<Pattern> a, std::unique_ptr<Pattern> b)
      : Pattern(), a(std::move(a)), b(std::move(b)) {}
  BiPattern(std::unique_ptr<Pattern> a, std::unique_ptr<Pattern> b,
            const Matrix &t)
      : Pattern(t), a(std::move(a)), b(std::move(b)) {}
  BiPattern(Pattern *a, Pattern *b, Matrix &&t) : Pattern(t), a(a), b(b) {}
  BiPattern(BiPattern &&other)
      : Pattern(std::move(other)), a(std::move(other.a)),
        b(std::move(other.b)) {}

  BiPattern &operator=(const BiPattern &) = delete;
  BiPattern &operator=(BiPattern &&rhs) {
    a.reset(rhs.a.release());
    b.reset(rhs.b.release());
    this->Pattern::operator=(std::move(rhs));
    return *this;
  }

  const Pattern *pattern1() const { return a.get(); }
  const Pattern *pattern2() const { return b.get(); }

private:
  std::unique_ptr<Pattern> a;
  std::unique_ptr<Pattern> b;
};

class PatternCheckers final : public BiPattern {
public:
  PatternCheckers(const PatternCheckers &other)
      : BiPattern(other.pattern1()->clone(), other.pattern2()->clone(),
                  other.transform()) {}
  PatternCheckers(PatternCheckers &&other) : BiPattern(std::move(other)) {}
  PatternCheckers(Pattern *a, Pattern *b) : BiPattern(a, b) {}
  PatternCheckers(Pattern *a, Pattern *b, const Matrix &t)
      : BiPattern(a, b, t) {}
  PatternCheckers(Pattern *a, Pattern *b, Matrix &&t) : BiPattern(a, b, t) {}

  virtual std::unique_ptr<Pattern> clone() const override {
    return std::unique_ptr<PatternCheckers>(new PatternCheckers(*this));
  }

  virtual Color local_at(const Tuple &point) const override {
    long m = long(std::floor(point.x()) + std::floor(point.y()) +
                  std::floor(point.z()));
    return m % 2 == 0 ? pattern1()->at(point) : pattern2()->at(point);
  }

  virtual explicit operator std::string() const override;
};

class PatternBlender final : public BiPattern {
public:
  PatternBlender(const PatternBlender &other)
      : BiPattern(other.pattern1()->clone(), other.pattern2()->clone(),
                  other.transform()) {}
  PatternBlender(PatternBlender &&other) : BiPattern(std::move(other)) {}
  PatternBlender(Pattern *a, Pattern *b) : BiPattern(a, b) {}
  PatternBlender(Pattern *a, Pattern *b, const Matrix &t)
      : BiPattern(a, b, t) {}
  PatternBlender(Pattern *a, Pattern *b, Matrix &&t) : BiPattern(a, b, t) {}

  virtual std::unique_ptr<Pattern> clone() const override {
    return std::unique_ptr<PatternBlender>(new PatternBlender(*this));
  }

  virtual Color local_at(const Tuple &point) const override {
    return (pattern1()->at(point) + pattern2()->at(point)) / 2.0;
  }

  virtual explicit operator std::string() const override;
};

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Pattern &P);
