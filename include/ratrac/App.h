#pragma once

#include "ratrac/ArgParse.h"

#include <string>

namespace ratrac {

/** Provide a common ground for all our ratrac applications as they are all
 * using the same options to set canvas size, verbosity, ....
 *
 * The supported options are:
 *   --help, -?            Display help message
 *   --verbose, -v         Increase verbosity
 *   --width=W, -w W       Set canvas width to W
 *   --height=H, -h H      Set canvas height to H
 *   --output=F, -o F      Save output to filename F
 *   --format=T, -f T      Save output in image format T (PPM ony for now)
 */
class App : public ArgParse {
public:
  enum OutputFormat { PPM };

  App(const std::string &programName, const std::string &description,
      size_t width = 320, size_t height = 240);

  const std::string &outputFilename() const { return m_outputFilename; }
  OutputFormat outputFormat() const { return m_outputFormat; }

  bool verbose() const { return m_verbosity >= 1; }
  unsigned verbosity() const { return m_verbosity; }

  size_t width() const { return m_width; }
  size_t height() const { return m_height; }

  std::string parameters() const;

private:
  std::string m_outputFilename;
  OutputFormat m_outputFormat;
  size_t m_width;
  size_t m_height;
  unsigned m_verbosity;
};

} // namespace ratrac