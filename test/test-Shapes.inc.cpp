TEST(Shapes, base) {

  // Basic sphere creation.
  Sphere s;
  EXPECT_EQ(s.center(), Point(0, 0, 0));
  EXPECT_EQ(s.radius(), 1.0);
  EXPECT_EQ(s.transform(), Matrice::identity());

  // Change a sphere's transformation.
  s = Sphere();
  Matrice t = Matrice::translation(2, 3, 4);
  s.transform(t);
  EXPECT_EQ(s.transform(), t);

  // Equality and inequality.
  s = Sphere();
  EXPECT_TRUE(s == Sphere());
  EXPECT_FALSE(s != Sphere());
  Sphere s2 = Sphere().transform(Matrice::translation(1, 2, 3));
  EXPECT_FALSE(s == s2);
  EXPECT_TRUE(s != s2);
  s2 = Sphere().material(Material(Color(0.8, 1.0, 0.6), /* ambient: */ 0.1,
                                  /* diffuse: */ 0.7, /* specular: */ 0.2,
                                  /* shininess: */ 200.0));
  EXPECT_FALSE(s == s2);
  EXPECT_TRUE(s != s2);
}

TEST(Shapes, output) {
  Sphere s;
  std::ostringstream string_stream;
  string_stream << s;
  EXPECT_EQ(string_stream.str(),
            "Sphere { center: Tuple { 0, 0, 0, 1}, radius: 1, transform: "
            "Matrice {    1.0,    0.0,    0.0,    0.0},\n\t{    0.0,    1.0,   "
            " 0.0,    0.0},\n\t{    0.0,    0.0,    1.0,    0.0},\n\t{    0.0, "
            "   0.0,    0.0,    1.0}}\n, material: Material { color: Color { "
            "red:1.0, green:1.0, blue:1.0, alpha:1.0}, ambient: 0.1, diffuse: "
            "0.9, specular: 0.9, shininess: 200.0}}");
}

TEST(Shapes, intersections) {
  // A ray intersects a sphere at two points.
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  Sphere s;
  Intersections xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].t, 4.0);
  EXPECT_EQ(xs[1].t, 6.0);

  // A ray intersects a sphere at a tangent.
  r = Ray(Point(0, 1, -5), Vector(0, 0, 1));
  xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].t, 5.0);
  EXPECT_EQ(xs[1].t, 5.0);

  // A ray misses a sphere.
  r = Ray(Point(0, 2, -5), Vector(0, 0, 1));
  xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 0);
  EXPECT_TRUE(xs.empty());

  // A ray intersects a sphere at a tangent.
  r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
  xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].t, -1.0);
  EXPECT_EQ(xs[1].t, 1.0);

  // A sphere is behind a ray.
  r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
  xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].t, -6.0);
  EXPECT_EQ(xs[1].t, -4.0);

  // Intersect sets the object on the intersection.
  r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
  xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].object, &s);
  EXPECT_EQ(xs[1].object, &s);

  // Intersecting a scaled sphere with a ray.
  r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  s = Sphere().transform(Matrice::scaling(2, 2, 2));
  xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].t, 3);
  EXPECT_EQ(xs[1].t, 7);

  // Intersecting a translated sphere with a ray.
  r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  s = Sphere().transform(Matrice::translation(5, 0, 0));
  xs = intersect(s, r);
  EXPECT_EQ(xs.count(), 0);
  EXPECT_TRUE(xs.empty());
}

TEST(Shapes, normal) {
  Sphere s;

  // The normal on a sphere at a point on the x axis.
  Tuple n = s.normal_at(Point(1, 0, 0));
  EXPECT_EQ(n, Vector(1, 0, 0));

  // The normal on a sphere at a point on the y axis.
  n = s.normal_at(Point(0, 1, 0));
  EXPECT_EQ(n, Vector(0, 1, 0));

  // The normal on a sphere at a point on the z axis.
  n = s.normal_at(Point(0, 0, 1));
  EXPECT_EQ(n, Vector(0, 0, 1));

  // The normal on a sphere at a non axial point.
  n = s.normal_at(Point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
  EXPECT_EQ(n, Vector(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));

  // The normal is a normalized vector.
  n = s.normal_at(Point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
  EXPECT_EQ(n, normalize(n));

  // Computing the normal on a translated sphere.
  s = Sphere().transform(Matrice::translation(0, 1, 0));
  n = s.normal_at(Point(0, 1.70711, -0.70711));
  EXPECT_EQ(n, Vector(0, 0.70711, -0.70711));

  // Computing the normal on a transformed sphere.
  s = Sphere().transform(Matrice::scaling(1, 0.5, 1) * Matrice::rotation_z(M_PI / 5.0));
  n = s.normal_at(Point(0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0));
  EXPECT_EQ(n, Vector(0, 0.97014, -0.24254));
}

TEST(Shapes, material) {
  // A sphere has a default material.
  Sphere s;
  EXPECT_EQ(s.material(), Material());

  // A sphere may be assigned a material.
  Material m;
  m.ambient(1);
  s.material(m);
  EXPECT_EQ(s.material(), m);
}