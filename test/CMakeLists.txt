set(GOOGLETEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest-1.14.0)
add_subdirectory(${GOOGLETEST_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include
                    ${GOOGLETEST_SOURCE_DIR}/include
                    ${GOOGLETEST_SOURCE_DIR}/gtest/include)

set(RATRAC_TEST_SOURCE_FILES
  test-App.cpp
  test-ArgParse.cpp
  test-Camera.cpp
  test-Canvas.cpp
  test-Color.cpp
  test-Intersections.cpp
  test-Light.cpp
  test-Material.cpp
  test-Matrix.cpp
  test-Patterns.cpp
  test-ProgressBar.cpp
  test-Ray.cpp
  test-Shapes.cpp
  test-StopWatch.cpp
  test-Tuple.cpp
  test-World.cpp
)

add_executable(test-ratrac test-ratrac.cpp ${RATRAC_TEST_SOURCE_FILES})
target_link_libraries(test-ratrac gtest ratrac)

add_custom_target(check COMMENT "Running ratrac unit tests")
add_custom_target(check-ratrac test-ratrac --gtest_color=yes --gtest_output=xml:test-ratrac.xml
                  DEPENDS test-ratrac
                  COMMENT "Running ratrac unit tests")
add_dependencies(check check-ratrac)
