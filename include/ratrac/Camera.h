#pragma once

#include "ratrac/Canvas.h"
#include "ratrac/Matrice.h"
#include "ratrac/Ray.h"
#include "ratrac/Tuple.h"
#include "ratrac/World.h"
#include "ratrac/ratrac.h"

namespace ratrac {
Matrice view_transform(const Tuple &from, const Tuple &to, const Tuple &up);

class Camera {
public:
  Camera(unsigned hsize, unsigned vsize, RayTracerDataType fov);

  unsigned hsize() const { return m_hsize; }
  unsigned vsize() const { return m_vsize; }
  RayTracerDataType field_of_view() const { return m_fov; }
  const Matrice &transform() const { return m_transform; }
  RayTracerDataType pixel_size() const { return m_pixel_size; }

  Camera &transform(const Matrice &transform) {
    m_transform = transform;
    return *this;
  }
  Camera &transform(Matrice &&transform) {
    m_transform = std::move(transform);
    return *this;
  }

  Ray ray_for_pixel(unsigned px, unsigned py) const;

  Canvas render(const World &w) const;

private:
  Matrice m_transform;
  unsigned m_hsize;
  unsigned m_vsize;
  RayTracerDataType m_fov; // Field of view, angle in radians.
  RayTracerDataType m_half_width;
  RayTracerDataType m_half_height;
  RayTracerDataType m_pixel_size;
};

} // namespace ratrac
