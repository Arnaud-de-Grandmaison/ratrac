#pragma once

#include <array>
#include <cmath>
#include <ostream>
#include <string>

#include "ratrac/ratrac.h"

namespace ratrac {
// Classes
// =======

/** Color represent a Color which is used by ratrac to manipulate
 * Points and Vectors. */
class Color {
  static_assert(std::is_floating_point<ratrac::RayTracerColorType>::value,
                "Color ColorTy must be a floating point type.");

public:
  typedef ratrac::RayTracerColorType ColorType;

  /** Initialise the Color/Tuple/Vector4.*/
  constexpr Color(ColorType red, ColorType green, ColorType blue)
      : m_color{red, green, blue, 1.0f} {}
  constexpr Color(ColorType red, ColorType green, ColorType blue,
                  ColorType alpha)
      : m_color{red, green, blue, alpha} {}
  constexpr Color() : m_color{0.0f, 0.0f, 0.0f, 1.0f} {}

  // Accessors
  // =========

  ColorType &red() { return m_color[0]; }
  ColorType &green() { return m_color[1]; }
  ColorType &blue() { return m_color[2]; }
  ColorType &alpha() { return m_color[3]; }

  const ColorType &red() const { return m_color[0]; }
  const ColorType &green() const { return m_color[1]; }
  const ColorType &blue() const { return m_color[2]; }
  const ColorType &alpha() const { return m_color[3]; }

  // Operators
  // =========

  // equal/not equal

  bool operator==(const Color &rhs) const {
    return close_to_equal(red(), rhs.red()) &&
           close_to_equal(green(), rhs.green()) &&
           close_to_equal(blue(), rhs.blue()) &&
           close_to_equal(alpha(), rhs.alpha());
  }
  bool operator!=(const Color &rhs) const { return !operator==(rhs); }

  // operations

  Color &operator+=(const Color &rhs) {
    for (unsigned i = 0; i < m_color.size() - 1; i++)
      m_color[i] += rhs.m_color[i];
    return *this;
  }
  Color &operator-=(const Color &rhs) {
    for (unsigned i = 0; i < m_color.size() - 1; i++)
      m_color[i] -= rhs.m_color[i];
    return *this;
  }
  Color &operator*=(ColorType rhs) {
    for (unsigned i = 0; i < m_color.size() - 1; i++)
      m_color[i] *= rhs;
    return *this;
  }
  Color &operator*=(const Color &rhs) {
    for (unsigned i = 0; i < m_color.size(); i++)
      m_color[i] *= rhs.m_color[i];
    return *this;
  }
  Color &operator/=(ColorType rhs) {
    for (unsigned i = 0; i < m_color.size() - 1; i++)
      m_color[i] /= rhs;
    return *this;
  }

  // Color helpers.
  constexpr static Color BLACK() { return Color(0, 0, 0); }
  constexpr static Color WHITE() { return Color(1, 1, 1); }
  constexpr static Color RED() { return Color(1, 0, 0); }
  constexpr static Color GREEN() { return Color(0, 1, 0); }
  constexpr static Color BLUE() { return Color(0, 0, 1); }

  explicit operator std::string() const;

private:
  // An array formated as following: RayTracerColor(red, green, blue, alpha).
  std::array<ColorType, 4> m_color;
};

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
