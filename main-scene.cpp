#include "ratrac/Camera.h"
#include "ratrac/Canvas.h"
#include "ratrac/Color.h"
#include "ratrac/Light.h"
#include "ratrac/Material.h"
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
  Sphere *floor = new Sphere();
  floor->transform(Matrice::scaling(10, 0.01, 10));
  Material m;
  m.color(Color(1, 0.9, 0.9));
  m.specular(0);
  floor->material() = m;
  world.append(floor);

  // The left Wall.
  Sphere *left_wall = new Sphere();
  left_wall->transform(
      Matrice::translation(0, 0, 5) * Matrice::rotation_y(-M_PI / 4.) *
      Matrice::rotation_x(M_PI / 2.) * Matrice::scaling(10, 0.01, 10));
  left_wall->material() = floor->material();
  world.append(left_wall);

  // The right Wall.
  Sphere *right_wall = new Sphere();
  right_wall->transform(
      Matrice::translation(0, 0, 5) * Matrice::rotation_y(M_PI / 4.) *
      Matrice::rotation_x(M_PI / 2.) * Matrice::scaling(10, 0.01, 10));
  right_wall->material() = floor->material();
  world.append(right_wall);

  // The middle sphere.
  Sphere *middle = new Sphere();
  middle->transform(Matrice::translation(-0.5, 1, 0.5));
  Material m2;
  m2.color(Color(0.1, 1, 0.5));
  m2.diffuse(0.7);
  m2.specular(0.3);
  middle->material() = m2;
  world.append(middle);

  // The right sphere.
  Sphere *right = new Sphere();
  right->transform(Matrice::translation(1.5, 0.5, -0.5) *
                   Matrice::scaling(0.5, 0.5, 0.5));
  Material m3;
  m3.color(Color(0.5, 1, 0.1));
  m3.diffuse(0.7);
  m3.specular(0.3);
  right->material() = m3;
  world.append(right);

  // The left sphere.
  Sphere *left = new Sphere();
  left->transform(Matrice::translation(-1.5, 0.33, -0.75) *
                  Matrice::scaling(0.33, 0.33, 0.33));
  Material m4;
  m4.color(Color(1, 0.8, 0.1));
  m4.diffuse(0.7);
  m4.specular(0.3);
  left->material() = m4;
  world.append(left);

  // The light source is white, shining from above and to the left:
  world.lights().push_back(LightPoint(Point(-10, 10, -10), Color(1, 1, 1)));

  Camera camera(100, 50, M_PI / 3.);
  camera.transform(
      view_transform(Point(0, 1.5, -5), Point(0, 1, 0), Vector(0, 1, 0)));

  // Render the world to a canvas.
  Canvas C = camera.render(world);

  // Save the scene.
  ofstream file("scene.ppm");
  C.to_ppm(file);

  return 0;
}
