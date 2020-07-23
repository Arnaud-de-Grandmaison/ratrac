#pragma once

#include <cassert>
#include <ostream>
#include <vector>

#include "ratrac/Color.h"

namespace ratrac {

class Canvas {
public:
  Canvas() = delete;
  Canvas(unsigned width, unsigned height, const Color &C = Color(0., 0., 0.))
      : m_width(width), m_height(height), m_canvas(width * height, C) {}
  Canvas(const Canvas &Other)
      : m_width(Other.m_width), m_height(Other.m_height),
        m_canvas(Other.m_canvas) {}
  Canvas(Canvas &&Other)
      : m_width(Other.m_width), m_height(Other.m_height),
        m_canvas(std::move(Other.m_canvas)) {
    Other.m_width = 0;
    Other.m_height = 0;
  }

  Canvas &operator=(const Canvas &Other) {
    m_width = Other.m_width;
    m_height = Other.m_height;
    m_canvas = Other.m_canvas;
    return *this;
  }

  Canvas &operator=(Canvas &&Other) {
    m_width = Other.m_width;
    m_height = Other.m_height;
    m_canvas = std::move(Other.m_canvas);
    Other.m_width = 0;
    Other.m_height = 0;
    return *this;
  }

  unsigned width() const { return m_width; }
  unsigned height() const { return m_height; }

  Color &at(unsigned x, unsigned y) { return m_canvas[y * m_width + x]; }
  const Color &at(unsigned x, unsigned y) const {
    assert(x < m_width && "x is out of bounds.");
    assert(y < m_height && "y is out of bounds.");
    return m_canvas[y * m_width + x];
  }

  /** Outputs the canvas in PPM format to a stream. */
  void to_ppm(std::ostream &os) const;

private:
  unsigned m_width;
  unsigned m_height;
  std::vector<Color> m_canvas;
};

} // namespace ratrac
