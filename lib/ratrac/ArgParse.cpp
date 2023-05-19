#include "ratrac/ArgParse.h"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>

using std::cerr;
using std::clog;
using std::ostringstream;
using std::string;
using std::vector;

namespace ratrac {
ArgParse::ArgParse(const string &programName, const string &description)
    : m_shortOptions(), m_longOptions(), m_options(),
      m_programName(programName), m_description(description) {}

void ArgParse::processOptionNames(vector<char> &shortNames,
                                  vector<string> &longNames,
                                  const vector<string> &names, size_t idx) {

  shortNames.clear();
  longNames.clear();
  for (const auto &n : names) {
    size_t numDashes = n.find_first_not_of("-");
    switch (numDashes) {
    default:
      error(
          string("unexpected number of leading dashes for option '" + n + "'"));
    case 1:
      if (n.size() > 2)
        error(string("unexpected short option format (a single character is "
                     "expected) for option '" +
                     n + "'"));
      shortNames.push_back(n[1]);
      break;
    case 2:
      if (n.size() == 2)
        error(string("unexpected long option format (name must be non empty) "
                     "for option '" +
                     n + "'"));
      longNames.emplace_back(n.substr(2));
      break;
    }
  }

  // Add the short option names to the known short options iff there is no
  // duplicates.
  for (const auto &c : shortNames) {
    if (m_shortOptions.count(c) > 0)
      error(string("short option '") + c + "' appears multiple times");
    m_shortOptions.insert(std::make_pair(c, idx));
  }

  // Add the long option names to the known long options iff there is no
  // duplicates.
  for (const auto &n : longNames) {
    if (m_longOptions.count(n) > 0)
      error(string("long option '") + n + "' appears multiple times");
    m_longOptions.insert(std::make_pair(n, idx));
  }
}

void ArgParse::addOption(const vector<string> &names, const string &help,
                         actionNoArgFn action) {
  vector<char> shortNames;
  vector<string> longNames;
  processOptionNames(shortNames, longNames, names, m_options.size());
  m_options.emplace_back(Option::noValueOption(
      std::move(shortNames), std::move(longNames), help, action));
}

void ArgParse::addOptionWithValue(const vector<string> &names,
                                  const string &metavar, const string &help,
                                  actionOneArgFn action) {
  vector<char> shortNames;
  vector<string> longNames;
  processOptionNames(shortNames, longNames, names, m_options.size());
  m_options.emplace_back(Option::oneValueOption(
      std::move(shortNames), std::move(longNames), metavar, help, action));
}

bool ArgParse::parse(size_t argc, const char *argv[]) const {
  for (size_t i = 0; i < argc; i++) {
    string arg = argv[i];
    size_t idx;
    string value;
    size_t numDashes = arg.find_first_not_of("-");
    switch (numDashes) {
    default:
      error(string("unexpected number of dashes for argument '") + arg + "'");
    case 1:
      // Process this argument as a short option.
      {
        if (arg.size() != 2)
          error(string("malformed short option argument '") + arg + "'");
        const auto p = m_shortOptions.find(arg[1]);
        if (p == m_shortOptions.end())
          error(string("unrecognized short option for argument '") + arg + "'");
        idx = p->second;
        if (m_options[idx]->m_kind == Option::OneValue) {
          if (i + 1 >= argc)
            error(string("short option '") + arg + "' is missing an argument");
          i++;
          value = argv[i];
        }
        break;
      }
    case 2:
      // Process this argument as a long option.
      {
        if (arg.size() <= 2)
          error(string("malformed long option argument '") + arg + "'");
        // Support both forms: "--long=t" and  "--long t"
        size_t pos = arg.find_first_of("=");
        string name;
        if (pos == string::npos)
          name = arg.substr(2);
        else
          name = arg.substr(2, pos - 2);

        const auto p = m_longOptions.find(name);
        if (p == m_longOptions.end())
          error(string("unrecognized long option for argument '") + arg + "'");
        idx = p->second;

        if (m_options[idx]->m_kind == Option::OneValue) {
          if (pos == string::npos) {
            if (i + 1 >= argc)
              error(string("long option '") + arg + "' is missing an argument");
            i++;
            value = argv[i];
          } else
          value = arg.substr(pos + 1);
        }
        break;
      }
    }

    switch (m_options[idx]->m_kind) {
    case Option::NoValue:
      if (!value.empty())
        error(string("got a value for an option with no value ('") + arg + ")");
      if (!m_options[idx]->m_actionNoArg())
        return false;
      break;
    case Option::OneValue:
      if (value.empty())
        error(string("got no value for an option that takes a value ('") + arg + ")");
      if (!m_options[idx]->m_actionOneArg(value))
        return false;
      break;
    }
  }
  return true;
}

string ArgParse::help() const {
  ostringstream h;
  h << m_programName << ' ' << m_description;

  if (!m_options.empty()) {
    h << "\n\nOptions:";
    for (const auto &o : m_options) {
      const char *sep = "";
      h << "\n  ";

      for (const auto &n : o->m_longNames) {
        h << sep << "--" << n;
        if (o->m_kind == Option::OneValue)
          h << '=' << o->m_metavar;
        sep = ", ";
      }

      for (const auto &n : o->m_shortNames) {
        h << sep << "-" << n;
        if (o->m_kind == Option::OneValue)
          h << ' ' << o->m_metavar;
        sep = ", ";
      }

      h << ": " << o->m_help;
    }
  }

  return h.str();
}

void ArgParse::warn(const string &text) { clog << "Warning: " << text << '\n'; }

void ArgParse::error(const string &text) const {
  cerr << "Error: " << text << '\n';
  exit(EXIT_FAILURE);
}

} // namespace ratrac