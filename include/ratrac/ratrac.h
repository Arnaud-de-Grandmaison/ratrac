#pragma once

#include <string>
#include <iostream>

namespace ratrac {

class World {
public:
  void say(std::ostream &os, const std::string &msg) const;
};

} // namespace ratrac
