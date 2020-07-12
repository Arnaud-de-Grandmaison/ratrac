#pragma once

#include <iostream>
#include <string>

#include "Tuple.h"

namespace ratrac {
// Classes
// =======

/** Useless class, mainly used for testing on project initialisation. */
class World {
public:
  void say(std::ostream &os, const std::string &msg) const;
};

class Projectile {
public:
  Projectile(Tuple position, Tuple velocity)
      : m_position{position}, m_velocity{velocity} {};

  // Accessors
  const Tuple &position() const { return m_position; }
  const Tuple &velocity() const { return m_velocity; }

private:
  // a Point
  Tuple m_position;
  // a Vector
  Tuple m_velocity;
};

class Environment {
public:
  Environment(Tuple gravity, Tuple wind) : m_gravity{gravity}, m_wind{wind} {};

  // Accessors
  const Tuple &gravity() const { return m_gravity; }
  const Tuple &wind() const { return m_wind; }

private:
  // a Vector
  Tuple m_gravity;
  // a Vector
  Tuple m_wind;
};

inline Projectile tick(Environment e, Projectile p) {
  Tuple position = p.position() + p.velocity();
  Tuple velocity = p.velocity() + e.gravity() + e.wind();
  return Projectile(position, velocity);
}

} // namespace ratrac
