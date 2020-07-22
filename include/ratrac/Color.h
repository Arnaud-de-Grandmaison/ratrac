#pragma once

#include <array>
#include <cmath>
#include <ostream>

namespace ratrac {
// Classes
// =======

/** Color represent a Color which is used by ratrac to manipulate
 * Points and Vectors. */
class Color {
public:
  /** Initialise the Color/Tuple/Vector4.*/
  Color(const float &red, const float &green, const float &blue)
      : m_color{red, green, blue, 1.0f} {} // Should it be Color(x, y, z, 1.0f)
  Color(const float &red, const float &green, const float &blue,
        const float &alpha)
      : m_color{red, green, blue, alpha} {}
  Color() : m_color{0.0f, 0.0f, 0.0f, 1.0f} {}

  // Accessors
  // =========

  const float &red() const { return m_color[0]; }
  const float &green() const { return m_color[1]; }
  const float &blue() const { return m_color[2]; }
  const float &alpha() const { return m_color[3]; }

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
  Color &operator*=(const float &rhs) {
    for (unsigned i = 0; i < m_color.size() - 1; i++)
      m_color[i] *= rhs;
    return *this;
  }
  Color &operator*=(const Color &rhs) {
    for (unsigned i = 0; i < m_color.size(); i++) {
      m_color[i] *= rhs.m_color[i];
    }
    return *this;
  }
  Color &operator/=(const float &rhs) {
    for (float &c : m_color)
      c /= rhs;
    return *this;
  }

private:
  /** An array formated as following: Color(red, green, blue, alpha). Args type:
   * float. */
  std::array<float, 4> m_color;

  /** Tests if two coordinates are approximalty equal. The difference must be
   * less than EPSILON. Currently, EPSILON = 0.00001. */
  static bool close_to_equal(const float &a, const float &b) {
    const float EPSILON = 0.00001;
    return std::fabs(a - b) < EPSILON;
  }
}; // namespace ratrac

// Other/External operators
// ========================

/** Adding two Colors. */
inline Color operator+(const Color &lhs, const Color &rhs) {
  Color tmp = lhs;
  tmp += rhs;
  return tmp;
}
/** Subtracting two Colors. */
inline Color operator-(const Color &lhs, const Color &rhs) {
  Color tmp = lhs;
  tmp -= rhs;
  return tmp;
}
/** Scalar multiplication. */
inline Color operator*(const Color &lhs, const float &rhs) {
  Color tmp = lhs;
  tmp *= rhs;
  return tmp;
}
/** Scalar multiplication reversed. */
inline Color operator*(const float &lhs, const Color &rhs) {
  Color tmp = rhs;
  tmp *= lhs;
  return tmp;
}
/** Colors multiplication. */
inline Color operator*(const Color &lhs, const Color &rhs) {
  Color tmp = rhs;
  tmp *= lhs;
  return tmp;
}
/** Scalar division. */
inline Color operator/(const Color &lhs, const float &rhs) {
  Color tmp = lhs;
  tmp /= rhs;
  return tmp;
}
} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Color &C);
