#include "ratrac/App.h"
#include "ratrac/Camera.h"
#include "ratrac/Color.h"
#include "ratrac/Light.h"
#include "ratrac/Matrix.h"
#include "ratrac/Patterns.h"
#include "ratrac/Shapes.h"
#include "ratrac/World.h"

#include <iostream>
#include <memory>

using namespace ratrac;
using namespace std;

int main(int argc, char *argv[]) {
  enum {
    STRIPES,
    GRADIENT,
    RING,
    COLOR_CHECKER,
    RADIAL_GRADIENT,
    PATTERN_CHECKERS,
    PATTERN_BLENDER,
  } pattern = STRIPES;

  App app("pattern-viewer", "view the available patterns");
  app.addOption({"--stripes"}, "display the Stripes pattern", [&]() {
    pattern = STRIPES;
    return true;
  });
  app.addOption({"--gradient"}, "display the Gradient pattern", [&]() {
    pattern = GRADIENT;
    return true;
  });
  app.addOption({"--ring"}, "display the Ring pattern", [&]() {
    pattern = RING;
    return true;
  });
  app.addOption({"--color-checker"}, "display the ColorChecker pattern", [&]() {
    pattern = COLOR_CHECKER;
    return true;
  });
  app.addOption({"--radial-gradient"}, "display the RadialGradient pattern",
                [&]() {
                  pattern = RADIAL_GRADIENT;
                  return true;
                });
  app.addOption({"--pattern-checkers"}, "display the PatternCheckers pattern",
                [&]() {
                  pattern = PATTERN_CHECKERS;
                  return true;
                });
  app.addOption({"--pattern-blender"}, "display the PatternBlender pattern",
                [&]() {
                  pattern = PATTERN_BLENDER;
                  return true;
                });
  if (!app.parse(argc - 1, (const char **)argv + 1))
    app.error("command line arguments parsing failed.");
  if (app.verbose())
    cout << app.parameters() << '\n';

  World world;

  // The floor.
  Plane *floor = new Plane();
  floor->transform(Matrix::rotation_y(-M_PI / 2));
  Material m;
  m.specular(0);
  unique_ptr<Pattern> p;
  switch (pattern) {
  case STRIPES:
    p.reset(new Stripes(Color::RED(), Color::BLUE()));
    break;
  case GRADIENT:
    p.reset(new Gradient(Color::RED(), Color::BLUE()));
    break;
  case RING:
    p.reset(new Ring(Color::RED(), Color::BLUE()));
    break;
  case COLOR_CHECKER:
    p.reset(new ColorCheckers(Color::RED(), Color::BLUE()));
    break;
  case RADIAL_GRADIENT:
    p.reset(new RadialGradient(Color::RED(), Color::BLUE()));
    break;
  case PATTERN_CHECKERS:
    p.reset(new PatternCheckers(new Stripes(Color::RED(), Color::BLUE(),
                                            Matrix::scaling(0.2, 0.2, 0.2) *
                                                Matrix::rotation_y(-M_PI / 4)),
                                new Stripes(Color::WHITE(), Color::GREEN(),
                                            Matrix::scaling(0.2, 0.2, 0.2) *
                                                Matrix::rotation_y(M_PI / 4))));
    break;
  case PATTERN_BLENDER:
    p.reset(new PatternBlender(
        new Stripes(Color::RED(), Color::BLUE(), Matrix::rotation_y(M_PI / 4)),
        new Stripes(Color::WHITE(), Color::BLACK(),
                    Matrix::rotation_y(-M_PI / 4))));
    break;
  }

  m.pattern(p);
  floor->material(m);
  world.append(floor);

  // The light source is white, shining from above and to the left:
  world.lights().push_back(LightPoint(Point(-10, 10, -10), Color::WHITE()));

  // Camera camera(100, 50, M_PI / 3.);
  Camera camera(app.width(), app.height(), M_PI / 3.);
  camera.transform(
      view_transform(Point(0, 1.5, -5), Point(0, 1, 0), Vector(0, 1, 0)));

  // Render the world to a canvas.
  Canvas C = camera.render(world, app.verbose());

  // Save the scene.
  app.save(C);

  return 0;
}