#pragma once

#include "ratrac/StopWatch.h"

#include <ostream>
#include <string>

namespace ratrac {
/** ProgressBar is a minimalistic progress bar. */
class ProgressBar {
public:
  ProgressBar(const std::string &caption, size_t steps, std::ostream &os,
              bool silent = false)
      : m_os(os), m_caption(caption), m_steps(steps), m_current(0), m_last(-1),
        m_silent(silent) {
    show();
  }

  ~ProgressBar() {
    if (!isSilent()) {
      m_os << '\n';
      m_os.flush();
    }
  }

  void incr(size_t steps = 1) {
    m_current += steps;
    show();
  }

  bool isSilent() const { return m_silent; }

protected:
  std::ostream &m_os;

private:
  const std::string m_caption;
  const size_t m_steps;
  size_t m_current;
  unsigned m_last;
  bool m_silent;

  void show() {
    if (isSilent())
      return;

    unsigned percent = 100 * m_current / m_steps;
    if (percent != m_last) {
      m_os << '\r' << m_caption << ": " << percent << '%';
      m_os.flush();
      m_last = percent;
    }
  }
};

/** TimedProgressBar adds the elapsed time when the progress bar object gets
 * destroyed. */
class TimedProgressBar : public ProgressBar {
public:
  TimedProgressBar(const std::string &caption, size_t steps, std::ostream &os,
                   bool silent = false)
      : ProgressBar(caption, steps, os, silent), m_time() {
    m_time.start();
  }

  ~TimedProgressBar() {
    m_time.stop();
    if (!isSilent())
      m_os << " (" << m_time.elapsed() << "s)";
  }

private:
  StopWatch m_time;
};
} // namespace ratrac