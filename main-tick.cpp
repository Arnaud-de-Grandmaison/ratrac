#include "ratrac/ratrac.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace ratrac;
using namespace std;

class Projectile {
public:
  Projectile(const Tuple &position, const Tuple &velocity)
      : m_position{position}, m_velocity{velocity} {};

  // Accessors
  const Tuple &position() const { return m_position; }
  const Tuple &velocity() const { return m_velocity; }

  void update(const Tuple &position, const Tuple &velocity) {
    m_position = position;
    m_velocity = velocity;
  }

private:
  // a Point
  Tuple m_position;
  // a Vector
  Tuple m_velocity;
};

class Environment {
public:
  Environment(const Projectile &projectile, const Tuple &gravity,
              const Tuple &wind) : m_proj(projectile), m_gravity{gravity}, m_wind{wind} {};

  // Accessors
  const Tuple &gravity() const { return m_gravity; }
  const Tuple &wind() const { return m_wind; }

  void tick() {
    Tuple position = m_proj.position() + m_proj.velocity();
    Tuple velocity = m_proj.velocity() + m_gravity + m_wind;
    m_proj.update(position, velocity);
  }

  const Projectile &projectile() const { return m_proj; }

private:
  // Our projectile
  Projectile m_proj;
  // a Vector
  Tuple m_gravity;
  // a Vector
  Tuple m_wind;
};

void experiment1() {
  ios state(nullptr);

  Projectile p(Point(0.0, 1.0, 0.0), Vector(1.0, 1.0, 0.0).normalize());
  Environment e(p, Vector(0.0, -0.1, 0.0), Vector(-0.01, 0.0, 0.0));

  unsigned it = 0;
  while (e.projectile().position().y() >= 0.0) {
    Tuple p_preposition = e.projectile().position();
    e.tick();
    Tuple p_postposition = e.projectile().position();
    cout << "Iteration n " << it << ":\n";

    state.copyfmt(cout);

    cout << "Projectile movement : " << left << setw(9)
         << p_postposition.x() - p_preposition.x() << " " << setw(9)
         << p_postposition.y() - p_postposition.y() << " " << setw(9)
         << p_postposition.z() - p_postposition.z() << ".\n"

         << "Projectile Position : " << setw(9) << p_postposition.x() << " "
         << setw(9) << p_postposition.y() << " " << setw(9)
         << p_postposition.z() << ".\n";

    cout.copyfmt(state);

    it++;
  };
}

void experiment2(const char *filename) {
  Canvas C(900, 550);

  Projectile p(Point(0.0, 1.0, 0.0), normalize(Vector(1.0, 1.8, 0.0)) * 11.25);
  Environment e(p, Vector(0.0, -0.1, 0.0), Vector(-0.01, 0.0, 0.0));

  while (e.projectile().position().y() >= 0.0) {
    Tuple Pos = e.projectile().position();
    int x = round(Pos.x());
    if (x < 0)
      x = 0;
    else if (x > C.width())
      x = C.width();

    int y = round(Pos.y());
    if (y < 0)
      y = 0;
    else if (y > C.height())
      y = C.height();

    C.at(x, C.height() - y) = Color(.8, .8, .8);
    e.tick();
  };

  ofstream file(filename);
  C.to_ppm(file);
}

int main(int argc, char *argv[]) {
  experiment1();
  experiment2("tick.ppm");
  return 0;
}
