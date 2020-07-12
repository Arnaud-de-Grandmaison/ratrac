#include "ratrac/ratrac.h"

#include <iomanip>
#include <iostream>

using namespace ratrac;
using namespace std;

int main(int argc, char *argv[]) {
  ios state(nullptr);

  World w;

  w.say(cout, "Hello World!");

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
