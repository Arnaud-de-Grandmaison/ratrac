#include "ratrac/App.h"
#include "ratrac/Canvas.h"
#include "ratrac/Color.h"
#include "ratrac/Tuple.h"
#include "ratrac/ratrac.h"

#include <fstream>
#include <iomanip>
#include <iostream>

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

void experiment2(Canvas &C) {
  Projectile p(Point(0.0, 1.0, 0.0), normalize(Vector(1.0, 1.8, 0.0)) * 11.25);
  Environment e(p, Vector(0.0, -0.1, 0.0), Vector(-0.01, 0.0, 0.0));

  for (; e.projectile().position().y() >= 0.0; e.tick()) {
    Tuple Pos = e.projectile().position();
    unsigned x = cap(Pos.x(), C.width() - 1);
    unsigned y = C.height() - cap(Pos.y(), C.height() - 1);
    C.at(x, y) = Color(.8, .8, .8);
  };
}

int main(int argc, char *argv[]) {
  App app("tick", "tests tick", 900, 550);
  if (!app.parse(argc - 1, (const char **)argv + 1))
    app.error("command line arguments parsing failed.");
  if (app.verbose())
    cout << app.parameters() << '\n';

  experiment1();

  Canvas C(app.width(), app.height());
  experiment2(C);
  app.save(C);

  return 0;
}
