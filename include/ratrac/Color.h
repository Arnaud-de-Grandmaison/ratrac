#pragma once

#include <array>
#include <cmath>
#include <ostream>

#include "ratrac/ratrac.h"

namespace ratrac {
// Classes
// =======

/** Color represent a Color which is used by ratrac to manipulate
 * Points and Vectors. */
template <class ColorTy> class RayTracerColor {
  static_assert(std::is_floating_point<ColorTy>::value,
                "Color ColorTy must be a floating point type.");

public:
  /** Initialise the Color/Tuple/Vector4.*/
  RayTracerColor(ColorTy red, ColorTy green, ColorTy blue)
      : m_color{red, green, blue, 1.0f} {}
  RayTracerColor(ColorTy red, ColorTy green, ColorTy blue, ColorTy alpha)
      : m_color{red, green, blue, alpha} {}
  RayTracerColor() : m_color{0.0f, 0.0f, 0.0f, 1.0f} {}

  typedef ColorTy ColorType;

  // Accessors
  // =========

  ColorTy &red() { return m_color[0]; }
  ColorTy &green() { return m_color[1]; }
  ColorTy &blue() { return m_color[2]; }
  ColorTy &alpha() { return m_color[3]; }

  const ColorTy &red() const { return m_color[0]; }
  const ColorTy &green() const { return m_color[1]; }
  const ColorTy &blue() const { return m_color[2]; }
  const ColorTy &alpha() const { return m_color[3]; }

  // Operators
  // =========

  // equal/not equal

  bool operator==(const RayTracerColor &rhs) const {
    return close_to_equal(red(), rhs.red()) &&
           close_to_equal(green(), rhs.green()) &&
           close_to_equal(blue(), rhs.blue()) &&
           close_to_equal(alpha(), rhs.alpha());
  }
  bool operator!=(const RayTracerColor &rhs) const { return !operator==(rhs); }

  // operations

  RayTracerColor &operator+=(const RayTracerColor &rhs) {
    for (unsigned i = 0; i < m_color.size() - 1; i++)
      m_color[i] += rhs.m_color[i];
    return *this;
  }
  RayTracerColor &operator-=(const RayTracerColor &rhs) {
    for (unsigned i = 0; i < m_color.size() - 1; i++)
      m_color[i] -= rhs.m_color[i];
    return *this;
  }
  RayTracerColor &operator*=(RayTracerColorType rhs) {
    for (unsigned i = 0; i < m_color.size() - 1; i++)
      m_color[i] *= rhs;
    return *this;
  }
  RayTracerColor &operator*=(const RayTracerColor &rhs) {
    for (unsigned i = 0; i < m_color.size(); i++)
      m_color[i] *= rhs.m_color[i];
    return *this;
  }
  RayTracerColor &operator/=(RayTracerColorType rhs) {
    for (unsigned i = 0; i < m_color.size() - 1; i++)
      m_color[i] /= rhs;
    return *this;
  }

  // Color helpers.
  static RayTracerColor BLACK() { return RayTracerColor(); }
  static RayTracerColor WHITE() { return RayTracerColor(1, 1, 1); }
  static RayTracerColor RED() { return RayTracerColor(1, 0, 0); }
  static RayTracerColor GREEN() { return RayTracerColor(0, 1, 0); }
  static RayTracerColor BLUE() { return RayTracerColor(0, 0, 1); }

private:
  // An array formated as following: RayTracerColor(red, green, blue, alpha).
  std::array<ColorTy, 4> m_color;
};

using Color = RayTracerColor<RayTracerColorType>;

// Other/External operators
// ========================

/** Adding two Colors. */
inline Color operator+(const Color &lhs, const Color &rhs) {
  Color tmp(lhs);
  tmp += rhs;
  return tmp;
}
/** Subtracting two Colors. */
inline Color operator-(const Color &lhs, const Color &rhs) {
  Color tmp(lhs);
  tmp -= rhs;
  return tmp;
}
/** Scalar multiplication. */
inline Color operator*(const Color &lhs, Color::ColorType rhs) {
  Color tmp(lhs);
  tmp *= rhs;
  return tmp;
}
/** Scalar multiplication reversed. */
inline Color operator*(Color::ColorType lhs, const Color &rhs) {
  Color tmp(rhs);
  tmp *= lhs;
  return tmp;
}
/** Colors multiplication. */
inline Color operator*(const Color &lhs, const Color &rhs) {
  Color tmp(rhs);
  tmp *= lhs;
  return tmp;
}
/** Scalar division. */
inline Color operator/(const Color &lhs, Color::ColorType rhs) {
  Color tmp(lhs);
  tmp /= rhs;
  return tmp;
}

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Color &C);
