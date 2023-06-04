#include "ratrac/App.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using std::cout;
using std::ofstream;
using std::ostringstream;
using std::stoul;
using std::string;

namespace ratrac {

App::App(const string &programName, const string &description, size_t width,
         size_t height)
    : ArgParse(programName, description),
      m_outputFilename(programName + ".ppm"), m_outputFormat(App::PPM),
      m_width(width), m_height(height), m_verbosity(0) {
  addOption({"--help", "-?"}, "Display this help message.", [&]() {
    cout << help() << '\n';
    exit(EXIT_SUCCESS);
    return true; // Silent warning
  });

  addOption({"--verbose", "-v"}, "Increase program verbosity.", [&]() {
    m_verbosity += 1;
    return true;
  });

  addOptionWithValue({"--width", "-w"}, "W", "Set canvas width to W",
                     [&](const string &s) {
                       m_width = stoul(s, nullptr, 0);
                       return true;
                     });

  addOptionWithValue({"--height", "-h"}, "H", "Set canvas height to H",
                     [&](const string &s) {
                       m_height = stoul(s, nullptr, 0);
                       return true;
                     });

  addOptionWithValue({"--output", "-o"}, "F", "Save output to filename F",
                     [&](const string &s) {
                       m_outputFilename = s;
                       return true;
                     });

  addOptionWithValue(
      {"--format", "-f"}, "T",
      "Save output in image format T, PPM or PNG (if support built in)",
      [&](const string &s) {
        if (s == "PPM" || s == "ppm") {
          m_outputFormat = App::PPM;
          return true;
        }
#ifdef RATRAC_USES_LIBPNG
        if (s == "PNG" || s == "png") {
          m_outputFormat = App::PNG;
          return true;
        }
#endif
        return false;
      });
}

string App::parameters() const {
  ostringstream os;
  os << "Canvas size: " << width() << 'x' << height() << '\n';
  os << "Ouput file: " << outputFilename() << " (";
  switch (outputFormat()) {
  case App::PPM:
    os << "PPM";
#ifdef RATRAC_USES_LIBPNG
  case App::PNG:
    os << "PNG";
#endif
  }
  os << " format)";
  return os.str();
}

void App::save(const Canvas &C) const {
  switch (outputFormat()) {
  case App::PPM: {
    ofstream file(outputFilename());
    C.to_ppm(file);
  } break;
#ifdef RATRAC_USES_LIBPNG
  case App::PNG:
    C.to_png(outputFilename());
    break;
#endif
  }
}

} // namespace ratrac