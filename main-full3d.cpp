#include "ratrac/Canvas.h"
#include "ratrac/Color.h"
#include "ratrac/Matrice.h"
#include "ratrac/Ray.h"
#include "ratrac/Shapes.h"
#include "ratrac/Light.h"
#include "ratrac/Material.h"
#include "ratrac/Tuple.h"

#include <fstream>

using namespace ratrac;
using namespace std;

int main(int argc, char *argv[]) {
  Canvas C(100, 100);

  Tuple ray_origin = Point(0, 0, -5);
  Matrice::DataType wall_z = 10;
  Matrice::DataType wall_size = 7;

  Matrice::DataType pixel_size = wall_size / C.width();
  Matrice::DataType half = wall_size / 2.;

  Material m;
  m.color(Color(1, 0.2, 1));

  Sphere s;
  s.material(m);

  LightPoint light(Point(-10, 10, -10), Color(1, 1, 1));

  for (unsigned y = 0; y < C.height(); y++) {
    Matrice::DataType world_y = half - pixel_size * y;
    for (unsigned x = 0; x < C.width(); x++) {
      Matrice::DataType world_x = half - pixel_size * x;
      Ray r = Ray(ray_origin, normalize(Point(world_x, world_y, wall_z)));
      Intersections xs = intersect(s, r);
      if (xs.hit() != xs.end()) {
        Tuple point = position(r, xs.hit()->t);
        Tuple normal = xs.hit()->object->normal_at(point);
        Tuple eye = -r.direction();
        Color color = xs.hit()->object->material().lighting(light, point, eye, normal);
        C.at(x, y) = color;
      }
    }
  }

  ofstream file("full3d.ppm");
  C.to_ppm(file);

  return 0;
}
