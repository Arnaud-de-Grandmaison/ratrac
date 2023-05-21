#include "ratrac/Tuple.h"
#include "bench-ratrac.h"
#include "ratrac/ratrac.h"

#include <benchmark/benchmark.h>

using ratrac::RayTracerDataType;
using ratrac::Tuple;

namespace {
Tuple getRandomTuple() {
    RayTracerDataType x, y, z, w;
    ratrac::getRandomData(x, y, z, w);
    return Tuple(x, y, z, w);
}

void BM_Tuple_Magnitude(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Tuple T = getRandomTuple();
    state.ResumeTiming();
    RayTracerDataType V = T.magnitude();
    benchmark::DoNotOptimize(V);
  }
}

void BM_Tuple_Normalize(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Tuple T = getRandomTuple();
    state.ResumeTiming();
    Tuple NT = T.normalize();
    benchmark::DoNotOptimize(NT);
  }
}

void BM_Tuple_Dot(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Tuple T1 = getRandomTuple();
    Tuple T2 = getRandomTuple();
    state.ResumeTiming();
    RayTracerDataType V = T1.dot(T2);
    benchmark::DoNotOptimize(V);
  }
}

void BM_Tuple_Cross(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Tuple T1 = getRandomTuple();
    Tuple T2 = getRandomTuple();
    state.ResumeTiming();
    Tuple R = T1.cross(T2);
    benchmark::DoNotOptimize(R);
  }
}

void BM_Tuple_Reflect(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Tuple T1 = getRandomTuple();
    Tuple T2 = getRandomTuple();
    state.ResumeTiming();
    Tuple R = T1.reflect(T2);
    benchmark::DoNotOptimize(R);
  }
}

void BM_Tuple_Magnitude2(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Tuple T = getRandomTuple();
    state.ResumeTiming();
    RayTracerDataType V = magnitude(T);
    benchmark::DoNotOptimize(V);
  }
}

void BM_Tuple_Normalize2(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Tuple T = getRandomTuple();
    state.ResumeTiming();
    Tuple NT = normalize(T);
    benchmark::DoNotOptimize(NT);
  }
}

void BM_Tuple_Dot2(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Tuple T1 = getRandomTuple();
    Tuple T2 = getRandomTuple();
    state.ResumeTiming();
    RayTracerDataType V = dot(T1, T2);
    benchmark::DoNotOptimize(V);
  }
}

void BM_Tuple_Cross2(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Tuple T1 = getRandomTuple();
    Tuple T2 = getRandomTuple();
    state.ResumeTiming();
    Tuple R = cross(T1, T2);
    benchmark::DoNotOptimize(R);
  }
}

void BM_Tuple_Reflect2(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Tuple T1 = getRandomTuple();
    Tuple T2 = getRandomTuple();
    state.ResumeTiming();
    Tuple R = reflect(T1, T2);
    benchmark::DoNotOptimize(R);
  }
}

} // namespace

BENCHMARK(BM_Tuple_Magnitude);
BENCHMARK(BM_Tuple_Normalize);
BENCHMARK(BM_Tuple_Dot);
BENCHMARK(BM_Tuple_Cross);
BENCHMARK(BM_Tuple_Reflect);

BENCHMARK(BM_Tuple_Magnitude2);
BENCHMARK(BM_Tuple_Normalize2);
BENCHMARK(BM_Tuple_Dot2);
BENCHMARK(BM_Tuple_Cross2);
BENCHMARK(BM_Tuple_Reflect2);