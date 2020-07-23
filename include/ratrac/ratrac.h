#pragma once

#include <cmath>

namespace ratrac {

using RayTracerDataType = double;

// Cap a component to [0:MaxValue]
template <class DataTy> inline unsigned cap(DataTy c, unsigned MaxValue) {
  int v = std::round(c);
  if (v < 0)
    return 0;
  else if ((unsigned) v > MaxValue)
    return MaxValue;
  return v;
}

template <class DataTy> inline bool close_to_equal(DataTy a, DataTy b) {
  const DataTy EPSILON = 0.00001;
  return std::fabs(a - b) < EPSILON;
}

} // namespace ratrac
