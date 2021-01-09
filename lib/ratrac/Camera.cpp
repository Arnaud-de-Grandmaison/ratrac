#include "ratrac/Camera.h"

namespace ratrac {

Matrice view_transform(const Tuple &from, const Tuple &to, const Tuple &up) {
  const Tuple forward = normalize(to - from);
  const Tuple upn = normalize(up);
  const Tuple left = cross(forward, upn);
  const Tuple true_up = cross(left, forward);
  const Matrice orientation({{left.x(), left.y(), left.z(), 0.},
                             {true_up.x(), true_up.y(), true_up.z(), 0.},
                             {-forward.x(), -forward.y(), -forward.z(), 0.},
                             {0., 0., 0., 1.}});
  return orientation * translation(-from.x(), -from.y(), -from.z());
}

} // namespace ratrac
