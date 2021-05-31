cmake_minimum_required (VERSION 3.16)
project(ratrac)

option(RATRAC_USES_ASAN "Use address sanitizer" OFF)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

function(append value)
  foreach(variable ${ARGN})
    set(${variable} "${${variable}} ${value}" PARENT_SCOPE)
  endforeach(variable)
endfunction()

# ========================================================
# Configure compilation options.
# --------------------------------------------------------


if(WIN32)
  set(CMAKE_C_FLAGS "/Wall")
  set(CMAKE_CXX_FLAGS "/std:c++14 ${CMAKE_C_FLAGS}")
  # This will make M_PI and other mathematical constants available.
  add_compile_definitions(_USE_MATH_DEFINES)
endif()

if(UNIX)
  # -pthread is needed for the testing framework.
  set(CMAKE_C_FLAGS "-Wall -pthread")
  if(RATRAC_USES_ASAN)
    append("-fsanitize=address" CMAKE_C_FLAGS)
  endif(RATRAC_USES_ASAN)
  set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_C_FLAGS}")
  set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
  set(CMAKE_C_FLAGS_DEBUG "-O1 -g -DDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")

  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" )
    if (CMAKE_GENERATOR STREQUAL "Ninja")
       append("-fcolor-diagnostics" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
    endif (CMAKE_GENERATOR STREQUAL "Ninja")
  endif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" )
endif(UNIX)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "Common C compiler flags" FORCE)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "C compiler flags (Release)" FORCE)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "C compiler flags (Debug)" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "Common C++ compiler flags" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "C++ compiler flags (Release)" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "C++ compiler flags (Debug)" FORCE)

# ========================================================
# Our own library of helpers.
# --------------------------------------------------------
set(RATRACLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/ratrac")
add_library(ratrac STATIC
  ${RATRACLIB_SOURCE_DIR}/Color.cpp
  ${RATRACLIB_SOURCE_DIR}/Canvas.cpp
  ${RATRACLIB_SOURCE_DIR}/Camera.cpp
  ${RATRACLIB_SOURCE_DIR}/Matrice.cpp
  ${RATRACLIB_SOURCE_DIR}/Shapes.cpp
  ${RATRACLIB_SOURCE_DIR}/Tuple.cpp
  ${RATRACLIB_SOURCE_DIR}/Ray.cpp
  ${RATRACLIB_SOURCE_DIR}/Light.cpp
  ${RATRACLIB_SOURCE_DIR}/Material.cpp
  ${RATRACLIB_SOURCE_DIR}/Patterns.cpp
  ${RATRACLIB_SOURCE_DIR}/Intersections.cpp
  ${RATRACLIB_SOURCE_DIR}/World.cpp
)
target_include_directories(ratrac PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(ratrac
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# ========================================================
# Unit testing.
# --------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

# ========================================================
# Ratrac demonstration apps
# --------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/apps)
