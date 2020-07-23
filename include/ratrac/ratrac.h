#pragma once

#include <cmath>

namespace ratrac {

using RayTracerDataType = double;

// Cap a component to [0:MaxValue]
template <class DataTy>
inline unsigned cap(DataTy c, int MaxValue) {
  int v = std::round(c);
  if (v < 0)
    return 0;
  else if (v > MaxValue)
    return MaxValue;
  return v;
}

}
