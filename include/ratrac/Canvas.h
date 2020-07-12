#pragma once

#include <iostream>
#include <string>

#include "ratrac/Color.h"

namespace ratrac {

class Canvas {
public:
  Canvas(unsigned width, unsigned height)
      : m_width(width), m_height(height), m_canvas(new Color[width * height]) {}
  ~Canvas() { delete[] m_canvas; }

  unsigned width() const { return m_width; }
  unsigned height() const { return m_height; }

  Color &at(unsigned x, unsigned y) { return m_canvas[y * m_width + x]; }

private:
  unsigned m_width;
  unsigned m_height;
  Color *m_canvas;
};

/** Convert the canvas into a string. */
std::string canvas_to_ppm(Canvas C) {
  std::string result = "P3\n" + std::to_string(C.width()) + " " +
                       std::to_string(C.height()) + "\n255\n";
  return result;
}
} // namespace ratrac