TEST(Material, base) {
  // Default material
  Material m;
  ASSERT_EQ(m, Material(Color(1, 1, 1), 0.1, 0.9, 0.9, 200.0));

  EXPECT_EQ(m.color(), Color(1, 1, 1));
  EXPECT_TRUE(close_to_equal<RayTracerColorType>(m.ambient(), 0.1));
  EXPECT_TRUE(close_to_equal<RayTracerColorType>(m.diffuse(), 0.9));
  EXPECT_TRUE(close_to_equal<RayTracerColorType>(m.specular(), 0.9));
  EXPECT_TRUE(close_to_equal<RayTracerColorType>(m.shininess(), 200.0));

  // Setters.
  m.color(Color::BLACK());
  EXPECT_EQ(m.color(), Color::BLACK());
  m.ambient(0.5);
  EXPECT_TRUE(close_to_equal<RayTracerColorType>(m.ambient(), 0.5));
  m.diffuse(0.5);
  EXPECT_TRUE(close_to_equal<RayTracerColorType>(m.diffuse(), 0.5));
  m.specular(0.5);
  EXPECT_TRUE(close_to_equal<RayTracerColorType>(m.specular(), 0.5));
  m.shininess(100.0);
  EXPECT_TRUE(close_to_equal<RayTracerColorType>(m.shininess(), 100.0));
}

TEST(Material, output) {
  Material m;
  std::ostringstream string_stream;
  string_stream << m;
  EXPECT_EQ(string_stream.str(),
            "Material { color: Color { red:1, green:1, blue:1, alpha:1}, "
            "ambient: 0.1, diffuse: 0.9, specular: 0.9, shininess: 200}");
}

TEST(Material, lighting) {
  Tuple position = Point(0, 0, 0);
  Material m;

  // Lighting with the eye between the light and the surface.
  Tuple eyev = Vector(0, 0, -1);
  Tuple normalv = Vector(0, 0, -1);
  LightPoint light(Point(0, 0, -10), Color(1, 1, 1));
  Color result = m.lighting(light, position, eyev, normalv);
  EXPECT_EQ(result, Color(1.9, 1.9, 1.9));
  result = lighting(m, light, position, eyev, normalv);
  EXPECT_EQ(result, Color(1.9, 1.9, 1.9));

  // Lighting with the eye between light and surface, eye offset 45°.
  eyev = Vector(0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
  normalv = Vector(0, 0, -1);
  light = LightPoint(Point(0, 0, -10), Color(1, 1, 1));
  result = m.lighting(light, position, eyev, normalv);
  EXPECT_EQ(result, Color(1.0, 1.0, 1.0));
  result = lighting(m, light, position, eyev, normalv);
  EXPECT_EQ(result, Color(1.0, 1.0, 1.0));

  // Lighting with eye opposite surface, light offset 45°.
  eyev = Vector(0, 0, -1);
  normalv = Vector(0, 0, -1);
  light = LightPoint(Point(0, 10, -10), Color(1, 1, 1));
  result = m.lighting(light, position, eyev, normalv);
  EXPECT_EQ(result, Color(0.7364, 0.7364, 0.7364));
  result = lighting(m, light, position, eyev, normalv);
  EXPECT_EQ(result, Color(0.7364, 0.7364, 0.7364));

  // Lighting with eye in the path of the reflection vector.
  eyev = Vector(0, -sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
  normalv = Vector(0, 0, -1);
  light = LightPoint(Point(0, 10, -10), Color(1, 1, 1));
  result = m.lighting(light, position, eyev, normalv);
  EXPECT_EQ(result, Color(1.6364, 1.6364, 1.6364));
  result = lighting(m, light, position, eyev, normalv);
  EXPECT_EQ(result, Color(1.6364, 1.6364, 1.6364));

  // Lighting with the light behind the surface.
  eyev = Vector(0, 0, -1);
  normalv = Vector(0, 0, -1);
  light = LightPoint(Point(0, 0, 10), Color(1, 1, 1));
  result = m.lighting(light, position, eyev, normalv);
  EXPECT_EQ(result, Color(0.1, 0.1, 0.1));
  result = lighting(m, light, position, eyev, normalv);
  EXPECT_EQ(result, Color(0.1, 0.1, 0.1));
}
