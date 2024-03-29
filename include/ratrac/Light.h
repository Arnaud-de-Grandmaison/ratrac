#pragma once

#include "ratrac/Color.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

#include <ostream>

namespace ratrac {
/** A lightPoint represents a point of light, i.e. a light source with no size,
 * existing at a single point in space. */
class LightPoint {

public:
  LightPoint() : m_intensity(), m_position() {}
  LightPoint(const LightPoint &) = default;
  LightPoint(const Tuple &position, const Color &intensity)
      : m_intensity(intensity), m_position(position) {}

  LightPoint &operator=(const LightPoint &) = default;

  constexpr bool operator==(const LightPoint &rhs) const noexcept {
    return m_intensity == rhs.m_intensity && m_position == rhs.m_position;
  }
  constexpr bool operator!=(const LightPoint &rhs) const noexcept {
    return !(*this == rhs);
  }

  const constexpr Color &intensity() const noexcept { return m_intensity; }
  const constexpr Tuple &position() const noexcept { return m_position; }

private:
  Color m_intensity;
  Tuple m_position;
};

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::LightPoint &LP);
