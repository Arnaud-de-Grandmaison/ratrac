TEST(Intersections, intersection) {
  // An intersection encapsulates t and object.
  Sphere s;
  Intersection i(3.5, s);
  EXPECT_EQ(i.t, 3.5);
  EXPECT_EQ(i.object, &s);

  // Agregating intersections.
  Intersection i1(1, s);
  Intersection i2(2, s);
  Intersections xs(i1, i2);
  EXPECT_EQ(xs.count(), 2);
  EXPECT_EQ(xs[0].t, 1);
  EXPECT_EQ(xs[1].t, 2);
}

TEST(Intersections, hit) {
  // The hit, when all intersections have positive t.
  std::unique_ptr<Sphere> s(new Sphere());
  Intersection i1(1, *s.get());
  Intersection i2(2, *s.get());
  Intersections xs(i1, i2);
  EXPECT_EQ(*xs.hit(), i1);

  // The hit, when some intersections have negative t.
  i1 = Intersection(-1, *s.get());
  i2 = Intersection(1, *s.get());
  EXPECT_EQ(*Intersections(i1, i2).hit(), i2);
  EXPECT_EQ(*Intersections(i2, i1).hit(), i2);

  // The hit, when all intersections have negative t.
  i1 = Intersection(-2, *s.get());
  i2 = Intersection(-1, *s.get());
  xs = Intersections(i1, i2);
  EXPECT_EQ(xs.hit(), xs.end());
  xs = Intersections(i2, i1);
  EXPECT_EQ(xs.hit(), xs.end());

  // The hit is always the lowest non negative intersection.
  i1 = Intersection(5, *s.get());
  i2 = Intersection(7, *s.get());
  Intersection i3(-3, *s.get());
  Intersection i4(2, *s.get());
  xs = Intersections(i1, i2).add(i3).add(i4);
  EXPECT_EQ(*xs.hit(), i4);

  // The hit sould offset the point.
  Ray r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  s.reset(new Sphere());
  s->transform(Matrix::translation(0, 0, 1));
  Intersection i = Intersection(5, *s.get());
  Computations comps(i, r);
  EXPECT_LT(comps.over_point.z(), -EPSILON<Tuple::DataType>()/2);
  EXPECT_GT(comps.point.z(), comps.over_point.z());
}

TEST(Intersections, computations) {
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  std::unique_ptr<Sphere> s(new Sphere());
  Intersection i(4, *s.get());
  Computations c(i, r);
  EXPECT_EQ(c.t , i.t);
  EXPECT_EQ(c.object , i.object);
  EXPECT_EQ(c.point , Point(0, 0, -1));
  EXPECT_EQ(c.eyev , Vector(0, 0, -1));
  EXPECT_EQ(c.normalv , Vector(0, 0, -1));

  // The hit, when an intersection occurs on the outside.
  s.reset(new Sphere());
  r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  i = Intersection(4, *s.get());
  c = Computations(i, r);
  EXPECT_FALSE(c.inside);

  // The hit, when an intersection occurs on the inside.
  s.reset(new Sphere());
  r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
  i = Intersection(1, *s.get());
  c = Computations(i, r);
  EXPECT_EQ(c.point , Point(0, 0, 1));
  EXPECT_EQ(c.eyev , Vector(0, 0, -1));
  EXPECT_EQ(c.normalv , Vector(0, 0, -1));
  EXPECT_TRUE(c.inside);
}
