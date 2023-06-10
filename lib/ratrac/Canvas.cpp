#include "ratrac/ratrac.h"
#include "ratrac/Canvas.h"

#include <cstdio>
#include <limits>
#include <memory>
#include <string>

#ifdef RATRAC_USES_LIBPNG
#include <png.h>
#endif

using std::numeric_limits;
using std::ostream;
using std::string;
using std::to_string;
using std::unique_ptr;

using namespace ratrac;

namespace {
template <typename Ty> struct RGB {
  static constexpr const unsigned int MaxValue = numeric_limits<uint8_t>::max();
  RGB() = default;
  RGB(const Color &Pixel)
      : red(cap(Pixel.red() * MaxValue, MaxValue)),
        green(cap(Pixel.green() * MaxValue, MaxValue)),
        blue(cap(Pixel.blue() * MaxValue, MaxValue)) {}
  Ty red;
  Ty green;
  Ty blue;
};
template <typename Ty> struct RGBA : public RGB<Ty> {
  RGBA() = default;
  RGBA(const Color &Pixel)
      : RGB<Ty>(Pixel),
        alpha(cap(Pixel.alpha() * RGB<Ty>::MaxValue, RGB<Ty>::MaxValue)) {}
  uint8_t alpha;
};
} // namespace

void Canvas::to_ppm(ostream &os) const {
  os << "P3\n";
  os << m_width << " " << m_height << "\n";
  os << RGB<uint8_t>::MaxValue << '\n';
  for(unsigned y = 0; y < m_height; y++) {
    const char *delimiter = "";
    unsigned delimiter_size = 0;
    unsigned lineLength = 0;
    for (unsigned x = 0; x < m_width; x++) {
      const RGB<uint8_t> Pixel(at(x, y));
      const string r = to_string(Pixel.red);
      const string g = to_string(Pixel.green);
      const string b = to_string(Pixel.blue);
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

#ifdef RATRAC_USES_LIBPNG
void Canvas::to_png(const std::string &filename) const {
  FILE *fp = fopen(filename.c_str(), "wb");
  if (fp) {
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr,
                                              nullptr, nullptr);
    if (png) {
      png_infop info = png_create_info_struct(png);
      if (info) {
        if (!setjmp(png_jmpbuf(png))) {
          png_init_io(png, fp);

          png_set_IHDR(png, info, width(), height(), 8, PNG_COLOR_TYPE_RGBA,
                       PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                       PNG_FILTER_TYPE_DEFAULT);

          // Set an image title.
          png_text title;
          title.compression = PNG_TEXT_COMPRESSION_NONE;
          title.key = (png_charp) "Title";
          title.text = (png_charp) "Ratrac image";
          png_set_text(png, info, &title, 1);

          png_write_info(png, info);

          unique_ptr<RGBA<uint8_t>[]> row(new RGBA<uint8_t>[m_width]);
          if (row) {
            for (size_t y = 0; y < m_height; y++) {
              for (size_t x = 0; x < m_width; x++)
                row[x] = RGBA<uint8_t>(at(x, y));
              png_write_row(png, (png_const_bytep)row.get());
            }
          }
          png_write_end(png, NULL);
        }
        png_free_data(png, info, PNG_FREE_ALL, -1);
      }
      png_destroy_write_struct(&png, nullptr);
    }
    fclose(fp);
  }
}
#endif