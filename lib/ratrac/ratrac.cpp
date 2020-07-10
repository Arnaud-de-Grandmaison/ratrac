#include "ratrac/ratrac.h"

#include <string>
#include <iostream>

using namespace ratrac;
using namespace std;

void World::say(ostream &os, const string &msg) const { os << msg << "\n"; }