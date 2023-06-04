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

  void precompute() {
    m_inverted_transform = inverse(m_transform);
  }
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

class Stripes : public BiColorPattern {
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

class Gradient : public BiColorPattern {
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

class Ring : public BiColorPattern {
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

class Checkers : public BiColorPattern {
public:
  Checkers(const Checkers &) = default;
  Checkers(const Color &a, const Color &b) : BiColorPattern(a, b) {}
  Checkers(const Color &a, const Color &b, const Matrix &t)
      : BiColorPattern(a, b, t) {}
  Checkers(const Color &a, const Color &b, Matrix &&t)
      : BiColorPattern(a, b, t) {}

  virtual std::unique_ptr<Pattern> clone() const override {
    return std::unique_ptr<Checkers>(new Checkers(*this));
  }

  virtual Color local_at(const Tuple &point) const override {
    long m = long(std::floor(point.x()) + std::floor(point.y()) +
                  std::floor(point.z()));
    return m % 2 == 0 ? color1() : color2();
  }

  virtual explicit operator std::string() const override;
};

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Pattern &P);
