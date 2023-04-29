struct TestShape : public Shape {
  TestShape() : Shape(), saved_ray() {}

  virtual Intersections local_intersect(const Ray &r) const override {
    saved_ray = r;
    return Intersections();
  }

  virtual Tuple local_normal_at(const Tuple &local_point) const override {
    return Vector(local_point.x(), local_point.y(), local_point.z());
  }

  mutable Ray saved_ray;
};

TEST(Shapes, base) {

  // Default transformation.
  std::unique_ptr<TestShape> s(new TestShape());
  EXPECT_EQ(s->transform(), Matrix::identity());

  // Assign a transformation.
  s.reset(new TestShape());
  Matrix t = Matrix::translation(2, 3, 4);
  s->transform(t);
  EXPECT_EQ(s->transform(), t);

  // Default material.
  s.reset(new TestShape());
  EXPECT_EQ(s->material(), Material());

  // Assign a material.
  s.reset(new TestShape());
  Material m;
  m.ambient(1);
  s->material(m);
  EXPECT_EQ(s->material(), m);

  // Equality and inequality.
  s.reset(new TestShape());
  EXPECT_TRUE(*s == TestShape());
  EXPECT_FALSE(*s != TestShape());
  std::unique_ptr<TestShape> s2(new TestShape());
  s2->transform(Matrix::translation(1, 2, 3));
  EXPECT_FALSE(*s == *s2);
  EXPECT_TRUE(*s != *s2);
  s2.reset(new TestShape());
  s2->material(Material(Color(0.8, 1.0, 0.6), /* ambient: */ 0.1,
                        /* diffuse: */ 0.7, /* specular: */ 0.2,
                        /* shininess: */ 200.0));
  EXPECT_FALSE(*s == *s2);
  EXPECT_TRUE(*s != *s2);
}

TEST(Shapes, sphere) {

  // Basic sphere creation.
  std::unique_ptr<Sphere> s(new Sphere());
  EXPECT_EQ(s->center(), Point(0, 0, 0));
  EXPECT_EQ(s->radius(), 1.0);
  EXPECT_EQ(s->transform(), Matrix::identity());
  EXPECT_EQ(s->material(), Material());

  // Change a sphere's transformation.
  s.reset(new Sphere());
  Matrix t = Matrix::translation(2, 3, 4);
  s->transform(t);
  EXPECT_EQ(s->transform(), t);

  // A sphere may be assigned a material.
  s.reset(new Sphere());
  Material m;
  m.ambient(1);
  s->material(m);
  EXPECT_EQ(s->material(), m);

  // Equality and inequality.
  s.reset(new Sphere());
  EXPECT_TRUE(*s == Sphere());
  EXPECT_FALSE(*s != Sphere());
  std::unique_ptr<Sphere> s2(new Sphere());
  s2->transform(Matrix::translation(1, 2, 3));
  EXPECT_FALSE(*s == *s2);
  EXPECT_TRUE(*s != *s2);
  s2.reset(new Sphere());
  s2->material(Material(Color(0.8, 1.0, 0.6), /* ambient: */ 0.1,
                        /* diffuse: */ 0.7, /* specular: */ 0.2,
                        /* shininess: */ 200.0));
  EXPECT_FALSE(*s == *s2);
  EXPECT_TRUE(*s != *s2);
}

TEST(Shapes, plane) {
  // Basic plane creation.
  std::unique_ptr<Plane> p(new Plane());

  EXPECT_EQ(p->transform(), Matrix::identity());
  EXPECT_EQ(p->material(), Material());

  // Equality and inequality.
  std::unique_ptr<Plane> p2(new Plane());
  EXPECT_TRUE(*p == *p2);
  EXPECT_FALSE(*p != *p2);
}

TEST(Shapes, output) {
  Sphere s;
  std::ostringstream string_stream;
  string_stream << s;
  EXPECT_EQ(string_stream.str(),
            "Sphere { center: Tuple { 0, 0, 0, 1}, radius: 1, transform: "
            "Matrix {    1.0,    0.0,    0.0,    0.0},\n\t{    0.0,    1.0,   "
            " 0.0,    0.0},\n\t{    0.0,    0.0,    1.0,    0.0},\n\t{    0.0, "
            "   0.0,    0.0,    1.0}}\n, material: Material { color: Color { "
            "red:1, green:1, blue:1, alpha:1}, pattern: Pattern {}, ambient: "
            "0.1, diffuse: 0.9, specular: 0.9, shininess: 200.0}}");
}

TEST(Shapes, intersections) {
  // Intersecting a scaled shape with a ray.
  Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  std::unique_ptr<TestShape> shape(new TestShape());
  shape->transform(Matrix::scaling(2, 2, 2));
  Intersections xs = shape->intersect(r);
  EXPECT_EQ(shape->saved_ray.origin(), Point(0, 0, -2.5));
  EXPECT_EQ(shape->saved_ray.direction(), Vector(0, 0, 0.5));

  // Intersecting a translated shape with a ray.
  r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  shape.reset(new TestShape());
  shape->transform(Matrix::translation(5, 0, 0));
  xs = shape->intersect(r);
  EXPECT_EQ(shape->saved_ray.origin(), Point(-5, 0, -5));
  EXPECT_EQ(shape->saved_ray.direction(), Vector(0, 0, 1));

  // A ray intersects a sphere at two points.
  r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  std::unique_ptr<Sphere> s(new Sphere());
  xs = s->intersect(r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].t, 4.0);
  EXPECT_EQ(xs[1].t, 6.0);

  // A ray intersects a sphere at a tangent.
  r = Ray(Point(0, 1, -5), Vector(0, 0, 1));
  xs = s->intersect(r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].t, 5.0);
  EXPECT_EQ(xs[1].t, 5.0);

  // A ray misses a sphere.
  r = Ray(Point(0, 2, -5), Vector(0, 0, 1));
  xs = s->intersect(r);
  EXPECT_EQ(xs.count(), 0);
  EXPECT_TRUE(xs.empty());

  // A ray intersects a sphere at a tangent.
  r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
  xs = s->intersect(r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].t, -1.0);
  EXPECT_EQ(xs[1].t, 1.0);

  // A sphere is behind a ray.
  r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
  xs = s->intersect(r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].t, -6.0);
  EXPECT_EQ(xs[1].t, -4.0);

  // Intersect sets the object on the intersection.
  r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
  xs = s->intersect(r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].object, s.get());
  EXPECT_EQ(xs[1].object, s.get());

  // Intersecting a scaled sphere with a ray.
  r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  s.reset(new Sphere());
  s->transform(Matrix::scaling(2, 2, 2));
  xs = s->intersect(r);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].t, 3);
  EXPECT_EQ(xs[1].t, 7);

  // Intersecting a translated sphere with a ray.
  r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  s.reset(new Sphere());
  s->transform(Matrix::translation(5, 0, 0));
  xs = s->intersect(r);
  EXPECT_EQ(xs.count(), 0);
  EXPECT_TRUE(xs.empty());

  // Intersecting with a ray parallel to the plane.
  std::unique_ptr<Plane> p(new Plane());
  r = Ray(Point(0, 10, 0), Vector(0, 0, 1));
  xs = p->local_intersect(r);
  EXPECT_EQ(xs.count(), 0);
  EXPECT_TRUE(xs.empty());

  // Intersecting with a coplanar ray.
  p.reset(new Plane());
  r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
  xs = p->local_intersect(r);
  EXPECT_EQ(xs.count(), 0);
  EXPECT_TRUE(xs.empty());

  // A ray intersecting a plane from above.
  p.reset(new Plane());
  r = Ray(Point(0, 1, 0), Vector(0, -1, 0));
  xs = p->local_intersect(r);
  EXPECT_EQ(xs.count(), 1);
  EXPECT_FALSE(xs.empty());
  EXPECT_EQ(xs[0].t, 1);
  EXPECT_EQ(xs[0].object, p.get());

  // A ray intersecting a plane from below.
  p.reset(new Plane());
  r = Ray(Point(0, -1, 0), Vector(0, 1, 0));
  xs = p->local_intersect(r);
  EXPECT_EQ(xs.count(), 1);
  EXPECT_FALSE(xs.empty());
  EXPECT_EQ(xs[0].t, 1);
  EXPECT_EQ(xs[0].object, p.get());
}

TEST(Shapes, normal) {
  // Computing the normal on a translated Shape.
  std::unique_ptr<TestShape> shape(new TestShape());
  shape->transform(Matrix::translation(0, 1, 0));
  Tuple n = shape->normal_at(Point(0, 1.70711, -0.70711));
  EXPECT_EQ(n, Vector(0, 0.70711, -0.70711));

  // Computing the normal on a transformed Shape.
  shape.reset(new TestShape());
  shape->transform(Matrix::scaling(1, 0.5, 1) *
                   Matrix::rotation_z(M_PI / 5.0));
  n = shape->normal_at(Point(0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0));
  EXPECT_EQ(n, Vector(0, 0.97014, -0.24254));

  std::unique_ptr<Sphere> s(new Sphere());

  // The normal on a sphere at a point on the x axis.
  n = s->normal_at(Point(1, 0, 0));
  EXPECT_EQ(n, Vector(1, 0, 0));

  // The normal on a sphere at a point on the y axis.
  n = s->normal_at(Point(0, 1, 0));
  EXPECT_EQ(n, Vector(0, 1, 0));

  // The normal on a sphere at a point on the z axis.
  n = s->normal_at(Point(0, 0, 1));
  EXPECT_EQ(n, Vector(0, 0, 1));

  // The normal on a sphere at a non axial point.
  n = s->normal_at(Point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
  EXPECT_EQ(n, Vector(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));

  // The normal is a normalized vector.
  n = s->normal_at(Point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
  EXPECT_EQ(n, normalize(n));

  // Computing the normal on a translated sphere.
  s.reset(new Sphere());
  s->transform(Matrix::translation(0, 1, 0));
  n = s->normal_at(Point(0, 1.70711, -0.70711));
  EXPECT_EQ(n, Vector(0, 0.70711, -0.70711));

  // Computing the normal on a transformed sphere.
  s.reset(new Sphere());
  s->transform(Matrix::scaling(1, 0.5, 1) * Matrix::rotation_z(M_PI / 5.0));
  n = s->normal_at(Point(0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0));
  EXPECT_EQ(n, Vector(0, 0.97014, -0.24254));

  std::unique_ptr<Plane> p(new Plane());

  // The normal of  plane is constant everywhere.
  Tuple n1 = p->local_normal_at(Point(0, 0, 0));
  Tuple n2 = p->local_normal_at(Point(10, 0, -10));
  Tuple n3 = p->local_normal_at(Point(-5, 0, 150));
  EXPECT_EQ(n1, Vector(0, 1, 0));
  EXPECT_EQ(n2, Vector(0, 1, 0));
  EXPECT_EQ(n3, Vector(0, 1, 0));
}
