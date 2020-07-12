#include "ratrac/ratrac.h"

#include <iomanip>
#include <iostream>

using namespace ratrac;
using namespace std;

class Projectile {
public:
  Projectile(Tuple position, Tuple velocity)
      : m_position{position}, m_velocity{velocity} {};

  // Accessors
  const Tuple &position() const { return m_position; }
  const Tuple &velocity() const { return m_velocity; }

private:
  // a Point
  Tuple m_position;
  // a Vector
  Tuple m_velocity;
};

class Environment {
public:
  Environment(Tuple gravity, Tuple wind) : m_gravity{gravity}, m_wind{wind} {};

  // Accessors
  const Tuple &gravity() const { return m_gravity; }
  const Tuple &wind() const { return m_wind; }

private:
  // a Vector
  Tuple m_gravity;
  // a Vector
  Tuple m_wind;
};

inline Projectile tick(Environment e, Projectile p) {
  Tuple position = p.position() + p.velocity();
  Tuple velocity = p.velocity() + e.gravity() + e.wind();
  return Projectile(position, velocity);
}

int main(int argc, char *argv[]) {
  ios state(nullptr);

  Projectile p(Point(0.0, 1.0, 0.0), Vector(1.0, 1.0, 0.0).normalize());
  Environment e(Vector(0.0, -0.1, 0.0), Vector(-0.01, 0.0, 0.0));

  string answer = "";
  Tuple p_preposition = p.position();
  Tuple p_postposition = {0.0, 1.0, 0.0, 1.0};
  int it = 0;
  do {

    p_preposition = p.position();
    p = tick(e, p);
    p_postposition = p.position();
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
  } while (p_postposition.y() > 0);
  return 0;
}
