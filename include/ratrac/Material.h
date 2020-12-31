#pragma once

#include <ostream>

#include "ratrac/Color.h"
#include "ratrac/Light.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

namespace ratrac {
/** A lightPoint represents a point of light, i.e. a light source with no size,
 * existing at a single point in space. */
class Material {

public:
  Material()
      : m_color(1, 1, 1), m_ambient(0.1), m_diffuse(0.9), m_specular(0.9),
        m_shininess(200.0) {}
  Material(const Color &color, RayTracerColorType ambient,
           RayTracerColorType diffuse, RayTracerColorType specular,
           RayTracerColorType shininess)
      : m_color(color), m_ambient(ambient), m_diffuse(diffuse),
        m_specular(specular), m_shininess(shininess) {}
  Material(const Material &) = default;

  Material &operator=(const Material &) = default;

  bool operator==(const Material &rhs) const {
    return m_color == rhs.m_color && close_to_equal(m_ambient, rhs.m_ambient) &&
           close_to_equal(m_diffuse, rhs.m_diffuse) &&
           close_to_equal(m_specular, rhs.m_specular) &&
           close_to_equal(m_shininess, rhs.m_shininess);
  }

  // Getters.
  const Color &color() const { return m_color; }
  const RayTracerColorType &ambient() const { return m_ambient; }
  const RayTracerColorType &diffuse() const { return m_diffuse; }
  const RayTracerColorType &specular() const { return m_specular; }
  const RayTracerColorType &shininess() const { return m_shininess; }

  // Setters.
  Material &color(const Color &color) {
    m_color = color;
    return *this;
  }
  Material &ambient(RayTracerColorType ambient) {
    m_ambient = ambient;
    return *this;
  }
  Material &diffuse(RayTracerColorType diffuse) {
    m_diffuse = diffuse;
    return *this;
  }
  Material &specular(RayTracerColorType specular) {
    m_specular = specular;
    return *this;
  }
  Material &shininess(RayTracerColorType shininess) {
    m_shininess = shininess;
    return *this;
  }

  Color lighting(const LightPoint &light, const Tuple &position,
                 const Tuple &eyev, const Tuple &normalv) const {
    // Combine the surface color with the light's color/intensity.
    Color effective_color = m_color * light.intensity();

    // Find the direction to the light source.
    Tuple lightv = normalize(light.position() - position);

    // Compute the ambient contribution.
    Color ambient = effective_color * m_ambient;

    /* light_dot_normal represents the cosine of the angle between the
     * light vector and the normal vector. A negative number means the
     * light is on the other side of the surface. */
    RayTracerColorType light_dot_normal = dot(lightv, normalv);
    Color diffuse = Color::BLACK();
    Color specular = Color::BLACK();
    if (light_dot_normal >= 0.0) {
      // Compute the diffuse contribution.
      diffuse = effective_color * m_diffuse * light_dot_normal;

      /* reflect_dot_eye represents the cosine of the angle between the
       * reflection vector and the eye vector. A negative number means
       * the light reflects away from the eye. */
      Tuple reflectv = reflect(-lightv, normalv);
      RayTracerDataType reflect_dot_eye = dot(reflectv, eyev);
      if (reflect_dot_eye > 0.0) {
        // Compute the specular contribution
        RayTracerColorType factor = pow(reflect_dot_eye, m_shininess);
        specular = light.intensity() * m_specular * factor;
      }
    }

    // Add the three contributions together to get the final shading.
    return ambient + diffuse + specular;
  }

private:
  Color m_color;
  RayTracerColorType m_ambient;
  RayTracerColorType m_diffuse;
  RayTracerColorType m_specular;
  RayTracerColorType m_shininess;
};

inline Color lighting(const Material &material, const LightPoint &light,
                      const Tuple &position, const Tuple &eyev,
                      const Tuple &normalv) {
  return material.lighting(light, position, eyev, normalv);
}

} // namespace ratrac

std::ostream &operator<<(std::ostream &os, const ratrac::Material &M);
