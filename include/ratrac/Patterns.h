#pragma once

#include <cmath>
#include <memory>
#include <ostream>
#include <string>

#include "ratrac/Color.h"
#include "ratrac/Matrice.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

namespace ratrac {

class Pattern {
public:
  Pattern() : m_transform(Matrice::identity()) {}
  Pattern(const Matrice &t) : m_transform(t) {}
  Pattern(Matrice &&t) : m_transform(std::move(t)) {}
  virtual ~Pattern();

  virtual std::unique_ptr<Pattern> clone() const = 0;

  Color at(const Tuple &point) const {
    Tuple local_point = inverse(transform()) * point;
    return local_at(local_point);
  }

  virtual Color local_at(const Tuple &point) const = 0;

  virtual explicit operator std::string() const { return "Pattern {}"; }

  const Matrice &transform() const { return m_transform; }

  Pattern &transform(const Matrice &M) {
    m_transform = M;
    return *this;
  }

  Pattern &transform(Matrice &&M) {
    m_transform = std::move(M);
    return *this;
  }

private:
  Matrice m_transform;
};

class Stripes : public Pattern {
public:
  Stripes(const Stripes &) = default;
  Stripes(const Color &a, const Color &b) : Pattern(), a(a), b(b) {}
  Stripes(const Color &a, const Color &b, const Matrice &t)
      : Pattern(t), a(a), b(b) {}
  Stripes(const Color &a, const Color &b, Matrice &&t)
      : Pattern(t), a(a), b(b) {}

  virtual std::unique_ptr<Pattern> clone() const override {
    return std::unique_ptr<Stripes>(new Stripes(*this));
  }

  const Color &color1() const { return a; }
  const Color &color2() const { return b; }

  virtual Color local_at(const Tuple &point) const override {
    return long(std::floor(point.x())) % 2 == 0 ? a : b;
  }

  virtual explicit operator std::string() const override;

private:
  Color a;
  Color b;
};

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Pattern &P);
