#pragma once

#include "ratrac/Color.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

#include <algorithm>
#include <cassert>
#include <vector>

namespace ratrac {

class Ray;
class Shape;
class World;

struct Intersection {
  Intersection() : t(RayTracerDataType()), object(nullptr) {}
  Intersection(const Intersection &) = default;
  Intersection(RayTracerDataType t, const Shape *object)
      : t(t), object(object) {}
  Intersection(RayTracerDataType t, const Shape &object)
      : t(t), object(&object) {}

  Intersection &operator=(const Intersection &) = default;

  bool operator<(const Intersection &rhs) const { return t < rhs.t; }

  bool operator==(const Intersection &Other) const {
    return t == Other.t && object == Other.object;
  }
  bool operator!=(const Intersection &Other) const {
    return t != Other.t || object != Other.object;
  }

  RayTracerDataType t;
  const Shape *object;
};

class Intersections {
public:
  Intersections() : m_xs() {}
  Intersections(const Intersections &) = default;
  Intersections(Intersections &&) = default;
  explicit Intersections(const Intersection &x0) : m_xs{x0} {}
  Intersections(const Intersection &x0, const Intersection &x1) : m_xs{x0, x1} {
    // Ensure our Intersection list is sorted.
    if (m_xs[0].t > m_xs[1].t)
      std::swap(m_xs[0], m_xs[1]);
  }

  Intersections &operator=(const Intersections &) = default;
  Intersections &operator=(Intersections &&) = default;

  typedef std::vector<Intersection>::iterator iterator;
  typedef std::vector<Intersection>::const_iterator const_iterator;

  iterator begin() { return m_xs.begin(); }
  iterator end() { return m_xs.end(); }
  const_iterator begin() const { return m_xs.begin(); }
  const_iterator end() const { return m_xs.end(); }

  Intersections &add(const Intersection &x) {
    // Keep our list sorted upon insertion of a new Intersection.
    if (m_xs.empty())
      m_xs.push_back(x);
    else {
      const_iterator it = std::lower_bound(m_xs.begin(), m_xs.end(), x);
      m_xs.insert(it, x);
    }
    return *this;
  }

  Intersections &add(const Intersections &xs) {
    for (const Intersection &x : xs)
      add(x);
    return *this;
  }

  unsigned count() const { return m_xs.size(); }
  bool empty() const { return m_xs.empty(); }

  const Intersection &operator[](unsigned i) const {
    assert(i < m_xs.size() && "Out of bounds access");
    return m_xs[i];
  }
  Intersection &operator[](unsigned i) {
    assert(i < m_xs.size() && "Out of bounds access");
    return m_xs[i];
  }

  const_iterator hit() const {
    return std::find_if(begin(), end(), [&](const Intersection &x) -> bool {
      return x.t >= 0.0;
    });
  }

private:
  std::vector<Intersection> m_xs;
};

struct Computations {
  Computations()
      : t(), object(nullptr), point(), over_point(), eyev(), normalv(),
        inside(false) {}
  Computations(const Computations &) = default;

  Computations(const Intersection &x, const Ray &ray);

  Computations &operator=(const Computations &) = default;

  Tuple::DataType t;
  const Shape *object;
  Tuple point;
  Tuple over_point;
  Tuple eyev;
  Tuple normalv;
  bool inside;
};

Color shade_hit(const World &world, const Computations &comps);
Color color_at(const World &world, const Ray &ray);

// Is point in shadow of light source i ?
bool is_shadowed(const World &world, const Tuple &point, unsigned i);

} // namespace ratrac
