#pragma once

#include <cmath>
#include <memory>
#include <ostream>
#include <string>

#include "ratrac/Color.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

namespace ratrac {

class Pattern {
public:
  virtual ~Pattern();

  virtual std::unique_ptr<Pattern> clone() const = 0;

  virtual Color at(const Tuple &point) const = 0;

  virtual explicit operator std::string() const { return "Pattern {}"; }
};

class Stripes : public Pattern {
public:
  Stripes(const Stripes &) =default;
  Stripes(const Color &a, const Color &b) : Pattern(), a(a), b(b) {}

  virtual std::unique_ptr<Pattern> clone() const override {
    return std::unique_ptr<Stripes>(new Stripes(*this));
  }

  const Color &color1() const { return a; }
  const Color &color2() const { return b; }

  virtual Color at(const Tuple &point) const override {
    return long(std::floor(point.x())) % 2 == 0 ? a : b;
  }

  virtual explicit operator std::string() const override;

private:
  Color a;
  Color b;
};

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Pattern &P);
