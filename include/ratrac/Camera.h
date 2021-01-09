#pragma once

#include "ratrac/Matrice.h"
#include "ratrac/Tuple.h"

namespace ratrac {
Matrice view_transform(const Tuple &from, const Tuple &to, const Tuple &up);
} // namespace ratrac
