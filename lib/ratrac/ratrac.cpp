#include "ratrac/ratrac.h"

#include <string>
#include <iostream>

using namespace ratrac;
using namespace std;

void World::say(const string &msg) const { cout << msg << "\n"; }
