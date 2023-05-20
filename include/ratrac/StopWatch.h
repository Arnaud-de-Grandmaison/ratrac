#pragma once

#include <chrono>
#include <ostream>
#include <string>

namespace ratrac {
/** StopWatch is a simplistic stopwatch implementation. */
class StopWatch {
public:
  using clock = std::chrono::high_resolution_clock;
  using time_point = clock::time_point;

  StopWatch() : m_start_time(), m_stop_time(), m_running(false) {}

  time_point now() const { return clock::now(); }

  bool isRunning() const { return m_running; }

  time_point start() {
    m_running = true;
    m_start_time = now();
    return m_start_time;
  }

  time_point stop() {
    m_running = false;
    m_stop_time = now();
    return m_stop_time;
  }

  double elapsed() const {
    time_point here = isRunning() ? now() : m_stop_time;

    return std::chrono::duration_cast<std::chrono::duration<double>>(
               here - m_start_time)
        .count();
  }

private:
  time_point m_start_time;
  time_point m_stop_time;
  bool m_running;
};

/** AutoStopWatch is a stopwatch wrapped so it's easy to use as RAI. */
class AutoStopWatch : public StopWatch {
public:
  AutoStopWatch(const std::string &caption, std::ostream &os,
                bool silent = false)
      : StopWatch(), m_caption(caption), m_os(os), m_silent(silent) {
    start();
  }

  ~AutoStopWatch() {
    stop();
    if (!m_silent)
      m_os << m_caption << ": " << elapsed() << "s\n";
  }

private:
  std::string m_caption;
  std::ostream &m_os;
  bool m_silent;
};

} // namespace ratrac