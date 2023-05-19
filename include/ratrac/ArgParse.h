#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace ratrac {

class ArgParse {
public:
  using actionNoArgFn = std::function<bool()>;
  using actionOneArgFn = std::function<bool(const std::string &)>;

  ArgParse() = delete;
  ArgParse(const std::string &programName, const std::string &description);

  bool parse(size_t argc, const char *argv[]) const;

  void addOption(const std::vector<std::string> &names, const std::string &help,
                 actionNoArgFn action);
  void addOptionWithValue(const std::vector<std::string> &names,
                          const std::string &metavar, const std::string &help,
                          actionOneArgFn action);

  const std::string &programName() const { return m_programName; }

  std::string help() const;

  virtual void warn(const std::string &text);
  [[noreturn]] virtual void error(const std::string &text) const;

private:
  class Option {
  public:
    enum Kind { NoValue, OneValue };

    Option() = delete;

    static Option *noValueOption(std::vector<char> &&shortNames,
                                 std::vector<std::string> &&longNames,
                                 const std::string &help,
                                 actionNoArgFn action) {
      return new Option(Kind::NoValue, std::move(shortNames),
                        std::move(longNames), "", help, action, nullptr);
    }
    static Option *oneValueOption(std::vector<char> &&shortNames,
                                  std::vector<std::string> &&longNames,
                                  const std::string &metavar,
                                  const std::string &help,
                                  actionOneArgFn action) {
      return new Option(Kind::OneValue, std::move(shortNames),
                        std::move(longNames), metavar, help, nullptr, action);
    }

    std::vector<char> m_shortNames;
    std::vector<std::string> m_longNames;
    std::string m_metavar;
    std::string m_help;
    actionNoArgFn m_actionNoArg;
    actionOneArgFn m_actionOneArg;
    Kind m_kind;

  private:
    // Constructor for an option with no associated value.
    Option(Kind kind, std::vector<char> &&shortNames,
           std::vector<std::string> &&longNames, const std::string &metavar,
           const std::string &help, actionNoArgFn action0,
           actionOneArgFn action1)
        : m_shortNames(shortNames), m_longNames(longNames), m_metavar(metavar),
          m_help(help), m_actionNoArg(action0), m_actionOneArg(action1),
          m_kind(kind) {}
  };

  void processOptionNames(std::vector<char> &shortNames,
                          std::vector<std::string> &longNames,
                          const std::vector<std::string> &names, size_t idx);

  // Map a short option character to an index in  m_options.
  std::unordered_map<char, size_t> m_shortOptions;
  std::unordered_map<std::string, size_t> m_longOptions;
  std::vector<std::unique_ptr<Option>> m_options;
  std::string m_programName;
  std::string m_description;
};

} // namespace ratrac