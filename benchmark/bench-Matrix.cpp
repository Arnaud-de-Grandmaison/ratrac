#include "ratrac/Matrix.h"

#include <benchmark/benchmark.h>

using ratrac::Matrix;

static void BM_Matrix_Identity(benchmark::State& state) {
  for (auto _ : state)
    Matrix::identity();
}
BENCHMARK(BM_Matrix_Identity);
