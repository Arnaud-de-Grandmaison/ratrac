#pragma once

#include "ratrac/Light.h"
#include "ratrac/Shapes.h"
#include "ratrac/ratrac.h"

#include <ostream>
#include <vector>

namespace ratrac {
class World {
public:
  World() : m_lights(), m_objects() {}
  World(const World &) = default;
  World(World &&) = default;

  World &operator=(const World &) = default;
  World &operator=(World &&) = default;

  const std::vector<LightPoint> &lights() const { return m_lights; }
  const std::vector<Sphere> &objects() const { return m_objects; }

  std::vector<LightPoint> &lights() { return m_lights; }
  std::vector<Sphere> &objects() { return m_objects; }

  // Get a default World, with a light and some objects.
  static World get_default();

private:
  std::vector<LightPoint> m_lights;
  std::vector<Sphere> m_objects;
};

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::World &world);
