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
    string delimiter;
    string currentLine;
    unsigned lineLenght = 0;
    for (unsigned x = 0; x < m_width; x++) {
      const Color &Pixel = at(x, y);
      // Scale to MaxValue
      unsigned r = cap(Pixel.red() * MaxValue, MaxValue);
      unsigned g = cap(Pixel.green() * MaxValue, MaxValue);
      unsigned b = cap(Pixel.blue() * MaxValue, MaxValue);
      for (const unsigned &c: {r, g, b}) {
        string s = to_string(c);
        if (lineLenght + delimiter.size() + s.size() > 70) {
          os << '\n';
          delimiter = "";
          lineLenght = 0;
        }
        os << delimiter << s;
        lineLenght += delimiter.size() + s.size();
        delimiter = " ";
      }
    }
    os << '\n';
  }
}
