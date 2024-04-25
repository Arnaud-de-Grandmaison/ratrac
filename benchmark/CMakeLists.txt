set(GOOGLEBENCHMARK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/benchmark-1.8.3)
add_subdirectory(${GOOGLEBENCHMARK_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include
                    ${GOOGLEBENCHMARK_SOURCE_DIR}/include)

set(RATRAC_BENCHMARK_SOURCE_FILES
  bench-Matrix.cpp
  bench-Tuple.cpp
)

add_executable(bench-ratrac bench-ratrac.cpp ${RATRAC_BENCHMARK_SOURCE_FILES})
target_link_libraries(bench-ratrac benchmark::benchmark ratrac)

add_custom_target(bench COMMENT "Running ratrac benchmarking")
add_custom_target(benchmark-ratrac bench-ratrac --benchmark_color=yes --benchmark_out_format=json --benchmark_out=bench-ratrac.json
                  DEPENDS bench-ratrac
                  COMMENT "Benchmarking ratrac")
add_dependencies(bench benchmark-ratrac)
