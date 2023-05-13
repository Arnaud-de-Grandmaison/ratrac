#include <gtest/gtest.h>

TEST(Camera, view_transform) {
  // The transformation matrix for the default orientation.
  Tuple from = Point(0, 0, 0);
  Tuple to = Point(0, 0, -1);
  Tuple up = Vector(0, 1, 0);
  Matrix t = view_transform(from, to, up);
  EXPECT_EQ(t, Matrix::identity());

  // View transformation matrix looking in positive z direction.
  from = Point(0, 0, 0);
  to = Point(0, 0, 1);
  up = Vector(0, 1, 0);
  t = view_transform(from, to, up);
  EXPECT_EQ(t, Matrix::scaling(-1, 1, -1));

  // The view transformation moves the world.
  from = Point(0, 0, 8);
  to = Point(0, 0, 0);
  up = Vector(0, 1, 0);
  t = view_transform(from, to, up);
  EXPECT_EQ(t, Matrix::translation(0, 0, -8));

  // An arbitrary view transformation.
  from = Point(1, 3, 2);
  to = Point(4, -2, 8);
  up = Vector(1, 1, 0);
  t = view_transform(from, to, up);
  EXPECT_TRUE(
      t.approximatly_equal(Matrix({{-0.50709, 0.50709, 0.67612, -2.36643},
                                    {0.76772, 0.60609, 0.12122, -2.82843},
                                    {-0.35857, 0.59761, -0.71714, 0.00000},
                                    {0.00000, 0.00000, 0.00000, 1.00000}})));
}

TEST(Camera, base) {
  // Constructing a camera.
  unsigned hsize = 160;
  unsigned vsize = 120;
  RayTracerDataType field_of_view = M_PI / 2.0;
  Camera c(hsize, vsize, field_of_view);
  EXPECT_EQ(c.hsize(), 160);
  EXPECT_EQ(c.vsize(), 120);
  EXPECT_EQ(c.field_of_view(), M_PI / 2.0);
  EXPECT_EQ(c.transform(), Matrix::identity());

  // The pixel size for a horizontal canvas.
  c = Camera(200, 125, M_PI / 2.0);
  EXPECT_TRUE(close_to_equal(c.pixel_size(), 0.01));

  // The pixel size for a vertical canvas.
  c = Camera(125, 200, M_PI / 2.0);
  EXPECT_TRUE(close_to_equal(c.pixel_size(), 0.01));
}

TEST(Camera, ray_construct) {
  // Constructing a ray through the center of the canvas
  Camera c(201, 101, M_PI / 2.0);
  Ray r = c.ray_for_pixel(100, 50);
  EXPECT_EQ(r.origin(), Point(0, 0, 0));
  EXPECT_EQ(r.direction(), Vector(0, 0, -1));

  // Constructing a ray through a corner of the canvas
  c = Camera(201, 101, M_PI / 2.0);
  r = c.ray_for_pixel(0, 0);
  EXPECT_EQ(r.origin(), Point(0, 0, 0));
  EXPECT_EQ(r.direction(), Vector(0.66519, 0.33259, -0.66851));

  // Constructing a ray when the camera is transformed
  c = Camera(201, 101, M_PI / 2.0);
  c.transform(Matrix::rotation_y(M_PI / 4.0) * Matrix::translation(0, -2, 5));
  r = c.ray_for_pixel(100, 50);
  EXPECT_EQ(r.origin(), Point(0, 2, -5));
  EXPECT_EQ(r.direction(), Vector(sqrt(2.0) / 2.0, 0, -sqrt(2.0) / 2.0));
}

TEST(Camera, world_rendering) {
  // Rendering a world with a camera.
  World w = World::get_default();
  Camera c(11, 11, M_PI / 2.0);
  Tuple from = Point(0, 0, -5);
  Tuple to = Point(0, 0, 0);
  Tuple up = Vector(0, 1, 0);
  c.transform(view_transform(from, to, up));
  Canvas image = c.render(w, /* verbose: */ false);
  EXPECT_EQ(image.at(5, 5), Color(0.38066, 0.47583, 0.2855));
}
