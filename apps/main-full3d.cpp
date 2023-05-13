#include "ratrac/Canvas.h"
#include "ratrac/Color.h"
#include "ratrac/Intersections.h"
#include "ratrac/Light.h"
#include "ratrac/Material.h"
#include "ratrac/Matrix.h"
#include "ratrac/ProgressBar.h"
#include "ratrac/Ray.h"
#include "ratrac/Shapes.h"
#include "ratrac/Tuple.h"

#include <fstream>
#include <iostream>
#include <memory>

using namespace ratrac;
using std::cout;
using std::ofstream;
using std::unique_ptr;

int main(int argc, char *argv[]) {
  Canvas C(100, 100);

  Tuple ray_origin = Point(0, 0, -5);
  Matrix::DataType wall_z = 10;
  Matrix::DataType wall_size = 7;

  Matrix::DataType pixel_size = wall_size / C.width();
  Matrix::DataType half = wall_size / 2.;

  Material m;
  m.color(Color(1, 0.2, 1));

  unique_ptr<Sphere> s(new Sphere());
  s->material(m);

  LightPoint light(Point(-10, 10, -10), Color::WHITE());

  {
    ProgressBar PB("Rendering", C.height() * C.width(), cout);
    for (unsigned y = 0; y < C.height(); y++) {
      Matrix::DataType world_y = half - pixel_size * y;
      for (unsigned x = 0; x < C.width(); x++) {
        Matrix::DataType world_x = half - pixel_size * x;
        Ray r = Ray(ray_origin, normalize(Point(world_x, world_y, wall_z)));
        Intersections xs = s->intersect(r);
        if (xs.hit() != xs.end()) {
          Tuple point = position(r, xs.hit()->t);
          Tuple normal = xs.hit()->object->normal_at(point);
          Tuple eye = -r.direction();
          Color color = xs.hit()->object->material().lighting(light, point, eye,
                                                              normal, false);
          C.at(x, y) = color;
        }
        PB.incr();
      }
    }
  }

  ofstream file("full3d.ppm");
  C.to_ppm(file);

  return 0;
}
