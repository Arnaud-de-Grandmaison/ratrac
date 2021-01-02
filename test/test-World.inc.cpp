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
