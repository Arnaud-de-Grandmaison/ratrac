#include "ratrac/Camera.h"
#include "ratrac/Canvas.h"
#include "ratrac/Color.h"
#include "ratrac/Light.h"
#include "ratrac/Material.h"
#include "ratrac/Patterns.h"
#include "ratrac/Shapes.h"
#include "ratrac/Tuple.h"
#include "ratrac/World.h"

#include <cmath>
#include <fstream>

using namespace ratrac;
using namespace std;

int main(int argc, char *argv[]) {
  World world;

  // The floor.
  Plane *floor = new Plane();
  floor->transform(Matrix::rotation_y(-M_PI / 2));
  Material m;
  m.pattern(Ring(Color(0.25, 0.25, 0.25), Color(0.8, 0.8, 0.8),
                 Matrix::scaling(0.5, 0.5, 0.5)));
  m.specular(0);
  floor->material(m);
  world.append(floor);

  // The big sphere.
  Sphere *middle = new Sphere();
  middle->transform(Matrix::translation(-1.0, 1.0, 2.0) *
                    Matrix::rotation_y(-M_PI / 2));
  Material m2;
  m2.pattern(Stripes(Color(0.1, 0.8, 0.1), Color(0.5, 0.8, 0.5),
                     Matrix::scaling(0.15, 0.15, 0.15) *
                         Matrix::rotation_z(-M_PI / 8) *
                         Matrix::rotation_y(-M_PI / 3)));
  m2.diffuse(0.7);
  m2.specular(0.3);
  middle->material(m2);
  world.append(middle);

  // The right sphere.
  Sphere *right = new Sphere();
  right->transform(Matrix::translation(1.5, 0.5, -0.5) *
                   Matrix::scaling(0.5, 0.5, 0.5));
  Material m3;
  m3.pattern(Gradient(Color::RED(), Color(1, 1, 0)));
  m3.diffuse(0.7);
  m3.specular(0.3);
  right->material(m3);
  world.append(right);

  // The left sphere.
  Sphere *left = new Sphere();
  left->transform(Matrix::translation(-1.5, 0.33, -0.75) *
                  Matrix::scaling(0.33, 0.33, 0.33));
  Material m4;
  m4.pattern(Checkers(Color(0.05, 0.25, 0.05), Color(0.5, 0.8, 0.5),
                      Matrix::scaling(0.1, 0.1, 0.1)));
  m4.color(Color(1, 0.8, 0.1));
  m4.diffuse(0.7);
  m4.specular(0.3);
  left->material(m4);
  world.append(left);

  // The light source is white, shining from above and to the left:
  world.lights().push_back(LightPoint(Point(-10, 10, -10), Color::WHITE()));

  // Camera camera(100, 50, M_PI / 3.);
  Camera camera(400, 200, M_PI / 3.);
  camera.transform(
      view_transform(Point(0, 1.5, -5), Point(0, 1, 0), Vector(0, 1, 0)));

  // Render the world to a canvas.
  Canvas C = camera.render(world, /* verbose: */ true);

  // Save the scene.
  ofstream file("patterns.ppm");
  C.to_ppm(file);

  return 0;
}
