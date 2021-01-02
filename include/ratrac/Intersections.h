#pragma once

#include "ratrac/Shapes.h"
#include "ratrac/Ray.h"
#include "ratrac/ratrac.h"

#include <algorithm>
#include <vector>

namespace ratrac {

struct Intersection {
  Intersection() : t(RayTracerDataType()), object(nullptr) {}
  Intersection(const Intersection &) = default;
  Intersection(RayTracerDataType t, const Sphere *object)
      : t(t), object(object) {}
  Intersection(RayTracerDataType t, const Sphere &object)
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
  const Sphere *object;
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

  Intersections &add(Intersection &x) {
    // Keep our list sorted upon insertion of a new Intersection.
    iterator it = std::lower_bound(begin(), end(), x);
    m_xs.insert(it, x);
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

Intersections intersect(const Sphere &s, const Ray &r);

} // namespace ratrac
