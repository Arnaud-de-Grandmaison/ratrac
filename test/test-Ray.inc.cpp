TEST(Ray, base) {
  // Testing Ray
  // ===========

  // Creating and querying a Ray
  Tuple origin = Point(1, 2, 3);
  Tuple direction = Vector(4, 5, 6);
  Ray ray(origin, direction);
  EXPECT_EQ(ray.origin(), origin);
  EXPECT_EQ(ray.direction(), direction);

  // Computing a point from a distance
  ray = Ray(Point(2, 3, 4), Vector(1, 0, 0));
  EXPECT_EQ(position(ray, 0), Point(2, 3, 4));
  EXPECT_EQ(position(ray, 1), Point(3, 3, 4));
  EXPECT_EQ(position(ray, -1), Point(1, 3, 4));
  EXPECT_EQ(position(ray, 2.5), Point(4.5, 3, 4));
}

TEST(Ray, transform) {
  // Translating a ray
  Ray r(Point(1, 2, 3), Vector(0, 1, 0));
  Matrice m = translation( 3, 4, 5);
  Ray r2 = transform(r, m);
  EXPECT_EQ(r2.origin(), Point(4, 6, 8));
  EXPECT_EQ(r2.direction(), Vector(0, 1, 0));

  // Scaling a ray
  r = Ray(Point(1, 2, 3), Vector(0, 1, 0));
  m = scaling(2, 3, 4);
  r2 = transform(r, m);
  EXPECT_EQ(r2.origin(), Point(2, 6, 12));
  EXPECT_EQ(r2.direction(), Vector(0, 3, 0));
}
