#pragma once

#include "ratrac/ratrac.h"

namespace ratrac {
RayTracerDataType getRandomData();
void getRandomData(RayTracerDataType &x, RayTracerDataType &y);
void getRandomData(RayTracerDataType &x, RayTracerDataType &y,
                   RayTracerDataType &z);
void getRandomData(RayTracerDataType &x, RayTracerDataType &y,
                   RayTracerDataType &z, RayTracerDataType &w);
void getRandomData(RayTracerDataType &x1, RayTracerDataType &x2,
                   RayTracerDataType &y1, RayTracerDataType &y2,
                   RayTracerDataType &z1, RayTracerDataType &z2);
} // namespace ratrac