#include "ratrac/Ray.h"
#include "ratrac/Shapes.h"
#include "ratrac/Intersections.h"
#include "ratrac/World.h"

#include "gtest/gtest.h"

using namespace ratrac;
using namespace testing;

TEST(World, base) {
  // Creating an (empty) world.
  World w;

  EXPECT_TRUE(w.lights().empty());
  EXPECT_TRUE(w.objects().empty());

  // Create a default world.
  w = World::get_default();
  EXPECT_EQ(w.lights().size(), 1);
  EXPECT_EQ(w.lights()[0], LightPoint(Point(-10, 10, -10), Color::WHITE()));

  EXPECT_EQ(w.objects().size(), 2);
  Sphere s1 =
      Sphere().material(Material(Color(0.8, 1.0, 0.6), /* ambient: */ 0.1,
                                 /* diffuse: */ 0.7, /* specular: */ 0.2,
                                 /* shininess: */ 200.0));
  Sphere s2 = Sphere().transform(scaling(0.5, 0.5, 0.5));
  EXPECT_EQ(w.objects()[0], s1);
  EXPECT_EQ(w.objects()[1], s2);
}

TEST(World, intersection) {
  World w = World::get_default();
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  Intersections xs = intersect(w, r);
  EXPECT_EQ(xs.count(), 4);
  EXPECT_EQ(xs[0].t, 4);
  EXPECT_EQ(xs[1].t, 4.5);
  EXPECT_EQ(xs[2].t, 5.5);
  EXPECT_EQ(xs[3].t, 6);
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
