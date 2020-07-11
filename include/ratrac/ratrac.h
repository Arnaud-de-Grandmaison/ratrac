#pragma once

#include <iostream>
#include <string>

namespace ratrac {
// Classes
// =======

/** Useless class, mainly used for testing on project initialisation. */
class World {
public:
  void say(std::ostream &os, const std::string &msg) const;
};

} // namespace ratrac
