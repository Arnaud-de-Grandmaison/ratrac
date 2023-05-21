#include "bench-ratrac.h"

#include <benchmark/benchmark.h>

#include <random>

namespace {
std::random_device RD;
std::mt19937 MT(RD());
std::uniform_real_distribution<ratrac::RayTracerDataType> Dist(-1000.0, 1000.0);
} // namespace

namespace ratrac {
RayTracerDataType getRandomData() { return Dist(MT); }

void getRandomData(RayTracerDataType &x, RayTracerDataType &y) {
  x = Dist(MT);
  y = Dist(MT);
}

void getRandomData(RayTracerDataType &x, RayTracerDataType &y,
                   RayTracerDataType &z) {
  x = Dist(MT);
  y = Dist(MT);
  z = Dist(MT);
}

void getRandomData(RayTracerDataType &x, RayTracerDataType &y,
                   RayTracerDataType &z, RayTracerDataType &w) {
  x = Dist(MT);
  y = Dist(MT);
  z = Dist(MT);
  w = Dist(MT);
}

void getRandomData(RayTracerDataType &x1, RayTracerDataType &x2,
                   RayTracerDataType &y1, RayTracerDataType &y2,
                   RayTracerDataType &z1, RayTracerDataType &z2) {
  x1 = Dist(MT);
  y1 = Dist(MT);
  z1 = Dist(MT);
  x2 = Dist(MT);
  y2 = Dist(MT);
  z2 = Dist(MT);
}

} // namespace ratrac

BENCHMARK_MAIN();