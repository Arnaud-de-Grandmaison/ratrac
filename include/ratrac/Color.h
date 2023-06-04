#pragma once

#include "ratrac/ratrac.h"

#include <cmath>
#include <ostream>
#include <string>

namespace ratrac {
// Classes
// =======

/** Color represent a Color which is used by ratrac to manipulate
 * Points and Vectors. */
class Color {
  static const unsigned NC = 4; // Number of color components.
  static const unsigned RED_idx = 0;
  static const unsigned GREEN_idx = 1;
  static const unsigned BLUE_idx = 2;
  static const unsigned ALPHA_idx = 3;
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

  ColorType &red() noexcept { return m_color[RED_idx]; }
  ColorType &green() noexcept { return m_color[GREEN_idx]; }
  ColorType &blue() noexcept { return m_color[BLUE_idx]; }
  ColorType &alpha() noexcept { return m_color[ALPHA_idx]; }

  const constexpr ColorType &red() const noexcept { return m_color[RED_idx]; }
  const constexpr ColorType &green() const noexcept {
    return m_color[GREEN_idx];
  }
  const constexpr ColorType &blue() const noexcept { return m_color[BLUE_idx]; }
  const constexpr ColorType &alpha() const noexcept {
    return m_color[ALPHA_idx];
  }

  // Operators
  // =========

  // equal/not equal

  constexpr bool operator==(const Color &rhs) const noexcept {
    return close_to_equal(red(), rhs.red()) &&
           close_to_equal(green(), rhs.green()) &&
           close_to_equal(blue(), rhs.blue()) &&
           close_to_equal(alpha(), rhs.alpha());
  }
  constexpr bool operator!=(const Color &rhs) const noexcept {
    return !operator==(rhs);
  }

  // operations

  Color &operator+=(const Color &rhs) {
    for (unsigned i = 0; i < NC - 1; i++)
      m_color[i] += rhs.m_color[i];
    return *this;
  }
  Color &operator-=(const Color &rhs) {
    for (unsigned i = 0; i < NC - 1; i++)
      m_color[i] -= rhs.m_color[i];
    return *this;
  }
  Color &operator*=(ColorType rhs) {
    for (unsigned i = 0; i < NC - 1; i++)
      m_color[i] *= rhs;
    return *this;
  }
  Color &operator*=(const Color &rhs) {
    for (unsigned i = 0; i < NC; i++)
      m_color[i] *= rhs.m_color[i];
    return *this;
  }
  Color &operator/=(ColorType rhs) {
    for (unsigned i = 0; i < NC - 1; i++)
      m_color[i] /= rhs;
    return *this;
  }

  // Color helpers.
  constexpr static Color BLACK() noexcept { return Color(0, 0, 0); }
  constexpr static Color WHITE() noexcept { return Color(1, 1, 1); }
  constexpr static Color RED() noexcept { return Color(1, 0, 0); }
  constexpr static Color GREEN() noexcept { return Color(0, 1, 0); }
  constexpr static Color BLUE() noexcept { return Color(0, 0, 1); }

  explicit operator std::string() const;

private:
  // An array formatted as following: RayTracerColor(red, green, blue, alpha).
  ColorType m_color[NC];
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
