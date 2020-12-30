set(GOOGLETEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest-release-1.10.0)
add_subdirectory(${GOOGLETEST_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${GOOGLETEST_SOURCE_DIR}/include
                    ${GOOGLETEST_SOURCE_DIR}/gtest/include)

add_custom_target(check
  COMMENT "Running ratrac unit tests")

macro(add_test name)
  # Compile the specific test:
  add_executable(test-${name}
                 EXCLUDE_FROM_ALL
                 ${CMAKE_CURRENT_SOURCE_DIR}/test-${name}.cpp)
  target_include_directories(test-${name} PRIVATE ${WAN_SOURCE_DIR})
  target_link_libraries(test-${name} gtest ratrac)
  add_custom_target(check-${name} test-${name} --gtest_color=yes --gtest_output=xml:test-${name}.xml
                    DEPENDS test-${name}
                    COMMENT "Running ${name} unit tests")
  add_dependencies(check check-${name})
endmacro(add_test name)

add_test(Tuple)
add_test(Color)
add_test(Canvas)
add_test(Matrice)
add_test(Ray)
add_test(Shapes)
add_test(Light)
