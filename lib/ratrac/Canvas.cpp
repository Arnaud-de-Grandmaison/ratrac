#include "ratrac/ratrac.h"
#include "ratrac/Canvas.h"

#include <string>

using namespace ratrac;
using namespace std;

void Canvas::to_ppm(ostream &os) const {
  const int MaxValue = 255;
  os << "P3\n";
  os << m_width << " " << m_height << "\n";
  os << MaxValue << '\n';
  for(unsigned y = 0; y < m_height; y++) {
    const char *delimiter = "";
    unsigned delimiter_size = 0;
    unsigned lineLength = 0;
    for (unsigned x = 0; x < m_width; x++) {
      const Color &Pixel = at(x, y);
      const string r = to_string(cap(Pixel.red() * MaxValue, MaxValue));
      const string g = to_string(cap(Pixel.green() * MaxValue, MaxValue));
      const string b = to_string(cap(Pixel.blue() * MaxValue, MaxValue));
      for (const auto &s: {r, g, b}) {
        if (lineLength + delimiter_size + s.size() > 70) {
          os << '\n';
          delimiter = "";
          delimiter_size = 0;
          lineLength = 0;
        }
        os << delimiter << s;
        lineLength += delimiter_size + s.size();
        delimiter = " ";
        delimiter_size = 1;
      }
    }
    os << '\n';
  }
}
