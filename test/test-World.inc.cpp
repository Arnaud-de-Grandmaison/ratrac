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

TEST(World, output) {
  World w = World::get_default();
  std::ostringstream string_stream;
  string_stream << w;
  EXPECT_EQ(
      string_stream.str(),
      "World {  lights: [LightPoint { intensity: Color { red:1, green:1, "
      "blue:1, alpha:1}, position: Tuple { -10, 10, -10, 1}}  ],  objects: "
      "[Sphere { center: Tuple { 0, 0, 0, 1}, radius: 1, transform: Matrice {  "
      "  1.0,    0.0,    0.0,    0.0},\n\t{    0.0,    1.0,    0.0,    "
      "0.0},\n\t{    0.0,    0.0,    1.0,    0.0},\n\t{    0.0,    0.0,    "
      "0.0,    1.0}}\n, material: Material { color: Color { red:0.8, "
      "green:1.0, blue:0.6, alpha:1.0}, ambient: 0.1, diffuse: 0.7, specular: "
      "0.2, shininess: 200.0}}, Sphere { center: Tuple { 0.0, 0.0, 0.0, 1.0}, "
      "radius: 1.0, transform: Matrice {    0.5,    0.0,    0.0,    0.0},\n\t{ "
      "   0.0,    0.5,    0.0,    0.0},\n\t{    0.0,    0.0,    0.5,    "
      "0.0},\n\t{    0.0,    0.0,    0.0,    1.0}}\n, material: Material { "
      "color: Color { red:1.0, green:1.0, blue:1.0, alpha:1.0}, ambient: 0.1, "
      "diffuse: 0.9, specular: 0.9, shininess: 200.0}}  ]}");
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

TEST(World, shading) {
  // Shading an intersection.
  World w = World::get_default();
  Ray r(Point(0, 0, -5), Vector(0, 0, 1));
  Sphere *s = w.object(0);
  Intersection i(4, s);
  Computations comps(i, r);
  Color c = shade_hit(w, comps);
  EXPECT_EQ(c, Color(0.380661, 0.475826, 0.285496));

  // Shading an intersection from the inside..
  w = World::get_default();
  *w.light(0) = LightPoint(Point(0, 0.25, 0), Color::WHITE());
  r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
  s = w.object(1);
  i = Intersection(0.5, s);
  comps = Computations(i, r);
  c = shade_hit(w, comps);
  EXPECT_EQ(c, Color(0.904984, 0.904984, 0.904984));

  // The color when a ray misses.
  w = World::get_default();
  r = Ray(Point(0, 0, -5), Vector(0, 1, 0));
  c = color_at(w, r);
  EXPECT_EQ(c, Color(0, 0, 0));

  // The color when a ray hits.
  w = World::get_default();
  r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
  c = color_at(w, r);
  EXPECT_EQ(c, Color(0.380661, 0.475826, 0.285496));

  // The color with an intersection behind the ray.
  w = World::get_default();
  Sphere *outer = w.object(0);
  outer->material().ambient(1);
  Sphere *inner = w.object(1);
  inner->material().ambient(1);
  r = Ray(Point(0, 0, 0.75), Vector(0, 0, -1));
  c = color_at(w, r);
  EXPECT_EQ(c, inner->material().color());
}
