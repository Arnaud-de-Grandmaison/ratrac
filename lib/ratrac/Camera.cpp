#include "ratrac/Camera.h"
#include "ratrac/Intersections.h"
#include "ratrac/ratrac.h"

#include <cmath>

namespace ratrac {

Camera::Camera(unsigned hsize, unsigned vsize, RayTracerDataType fov)
    : m_transform(Matrix::identity()), m_hsize(hsize), m_vsize(vsize),
      m_fov(fov), m_half_width(), m_half_height(), m_pixel_size() {
  RayTracerDataType half_view = std::tan(m_fov / 2.0);
  RayTracerDataType aspect =
      RayTracerDataType(m_hsize) / RayTracerDataType(m_vsize);
  if (aspect >= 1.0) {
    m_half_width = half_view;
    m_half_height = half_view / aspect;
  } else {
    m_half_width = half_view * aspect;
    m_half_height = half_view;
  }
  m_pixel_size = (m_half_width * 2.0) / m_hsize;
}

Ray Camera::ray_for_pixel(unsigned px, unsigned py) const {
  // the offset from the edge of the canvas to the pixel's center.
  RayTracerDataType xoffset = (RayTracerDataType(px) + 0.5) * m_pixel_size;
  RayTracerDataType yoffset = (RayTracerDataType(py) + 0.5) * m_pixel_size;

  // The untransformed coordinates of the pixel in world space.
  // (remember that the camera looks toward -z, so +x is to the *left*.)
  RayTracerDataType world_x = m_half_width - xoffset;
  RayTracerDataType world_y = m_half_height - yoffset;

  // Using the camera matrix, transform the canvas point and the origin,
  // and then compute the ray's direction vector.
  // (remember that the canvas is at z=-1)
  Matrix i = inverse(m_transform);
  Tuple pixel = i * Point(world_x, world_y, -1);
  Tuple origin = i * Point(0, 0, 0);
  Tuple direction = normalize(pixel - origin);

  return Ray(origin, direction);
}

Canvas Camera::render(const World &world) const {
  Canvas image(m_hsize, m_vsize);

  for (unsigned y = 0; y < m_vsize; y++) {
    for (unsigned x = 0; x < m_hsize; x++) {
      Ray ray = ray_for_pixel(x, y);
      image.at(x, y) = color_at(world, ray);
    }
  }

  return image;
}

Matrix view_transform(const Tuple &from, const Tuple &to, const Tuple &up) {
  const Tuple forward = normalize(to - from);
  const Tuple upn = normalize(up);
  const Tuple left = cross(forward, upn);
  const Tuple true_up = cross(left, forward);
  const Matrix orientation({{left.x(), left.y(), left.z(), 0.},
                             {true_up.x(), true_up.y(), true_up.z(), 0.},
                             {-forward.x(), -forward.y(), -forward.z(), 0.},
                             {0., 0., 0., 1.}});
  return orientation * Matrix::translation(-from.x(), -from.y(), -from.z());
}

} // namespace ratrac
