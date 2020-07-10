#include "ratrac/ratrac.h"

#include <iostream>
#include <string>

using namespace ratrac;
using namespace std;

void World::say(ostream &os, const string &msg) const { os << msg << "\n"; }
