#include "ratrac/Matrix.h"

#include <benchmark/benchmark.h>

#include <random>

std::random_device RD;
std::mt19937 MT(RD());
std::uniform_real_distribution<ratrac::Matrix::DataType> Dist(-1000.0, 1000.0);

using ratrac::Matrix;

namespace {
void BM_Matrix_Identity(benchmark::State &state) {
  for (auto _ : state) {
    Matrix M = Matrix::identity();
    benchmark::DoNotOptimize(M);
  }
}

void BM_Matrix_Translation(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Matrix::DataType x = Dist(MT);
    Matrix::DataType y = Dist(MT);
    Matrix::DataType z = Dist(MT);
    state.ResumeTiming();
    Matrix M = Matrix::translation(x, y, z);
    benchmark::DoNotOptimize(M);
  }
}

void BM_Matrix_Scaling(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Matrix::DataType x = Dist(MT);
    Matrix::DataType y = Dist(MT);
    Matrix::DataType z = Dist(MT);
    state.ResumeTiming();
    Matrix M = Matrix::scaling(x, y, z);
    benchmark::DoNotOptimize(M);
  }
}

void BM_Matrix_Rotation_X(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Matrix::DataType x = Dist(MT);
    state.ResumeTiming();
    Matrix M = Matrix::rotation_x(x);
    benchmark::DoNotOptimize(M);
  }
}

void BM_Matrix_Rotation_Y(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Matrix::DataType y = Dist(MT);
    state.ResumeTiming();
    Matrix M = Matrix::rotation_y(y);
    benchmark::DoNotOptimize(M);
  }
}

void BM_Matrix_Rotation_Z(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Matrix::DataType z = Dist(MT);
    state.ResumeTiming();
    Matrix M = Matrix::rotation_z(z);
    benchmark::DoNotOptimize(M);
  }
}

void BM_Matrix_Shearing(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Matrix::DataType xy = Dist(MT);
    Matrix::DataType xz = Dist(MT);
    Matrix::DataType yx = Dist(MT);
    Matrix::DataType yz = Dist(MT);
    Matrix::DataType zx = Dist(MT);
    Matrix::DataType zy = Dist(MT);
    state.ResumeTiming();
    Matrix M = Matrix::shearing(xy, xz, yx, yz, zx, zy);
    benchmark::DoNotOptimize(M);
  }
}

// ================================================================
// Fluent matrix transformation API benchmarking.
void BM_Matrix_Translate(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Matrix::DataType x = Dist(MT);
    Matrix::DataType y = Dist(MT);
    Matrix::DataType z = Dist(MT);
    Matrix id = Matrix::identity();
    state.ResumeTiming();
    Matrix M = id.translate(x, y, z);
    benchmark::DoNotOptimize(M);
  }
}

void BM_Matrix_Scale(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Matrix::DataType x = Dist(MT);
    Matrix::DataType y = Dist(MT);
    Matrix::DataType z = Dist(MT);
    Matrix id = Matrix::identity();
    state.ResumeTiming();
    Matrix M = id.scale(x, y, z);
    benchmark::DoNotOptimize(M);
  }
}

void BM_Matrix_Rotate_X(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Matrix::DataType x = Dist(MT);
    Matrix id = Matrix::identity();
    state.ResumeTiming();
    Matrix M = id.rotate_x(x);
    benchmark::DoNotOptimize(M);
  }
}

void BM_Matrix_Rotate_Y(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Matrix::DataType y = Dist(MT);
    Matrix id = Matrix::identity();
    state.ResumeTiming();
    Matrix M = id.rotate_y(y);
    benchmark::DoNotOptimize(M);
  }
}

void BM_Matrix_Rotate_Z(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Matrix::DataType z = Dist(MT);
    Matrix id = Matrix::identity();
    state.ResumeTiming();
    Matrix M = id.rotate_z(z);
    benchmark::DoNotOptimize(M);
  }
}

void BM_Matrix_Shear(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    Matrix::DataType xy = Dist(MT);
    Matrix::DataType xz = Dist(MT);
    Matrix::DataType yx = Dist(MT);
    Matrix::DataType yz = Dist(MT);
    Matrix::DataType zx = Dist(MT);
    Matrix::DataType zy = Dist(MT);
    Matrix id = Matrix::identity();
    state.ResumeTiming();
    Matrix M = id.shear(xy, xz, yx, yz, zx, zy);
    benchmark::DoNotOptimize(M);
  }
}
} // namespace

BENCHMARK(BM_Matrix_Identity);
BENCHMARK(BM_Matrix_Translation);
BENCHMARK(BM_Matrix_Scaling);
BENCHMARK(BM_Matrix_Rotation_X);
BENCHMARK(BM_Matrix_Rotation_Y);
BENCHMARK(BM_Matrix_Rotation_Z);
BENCHMARK(BM_Matrix_Shearing);

BENCHMARK(BM_Matrix_Translate);
BENCHMARK(BM_Matrix_Scale);
BENCHMARK(BM_Matrix_Rotate_X);
BENCHMARK(BM_Matrix_Rotate_Y);
BENCHMARK(BM_Matrix_Rotate_Z);
BENCHMARK(BM_Matrix_Shear);
