cmake_minimum_required (VERSION 3.16)
project(ratrac)

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

if(WIN32)
  set(CMAKE_C_FLAGS "/Wall")
  set(CMAKE_CXX_FLAGS "/std:c++14 ${CMAKE_C_FLAGS}")
endif()

if(UNIX)
  # -pthread is needed for the testing framework.
  set(CMAKE_C_FLAGS "-Wall -pthread")
  set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_C_FLAGS}")
  set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG")
  set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
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
  ${RATRACLIB_SOURCE_DIR}/ratrac.cpp
)
target_include_directories(ratrac PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ========================================================
# Unit testing.
# --------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

# ========================================================
# Ratrac main
# --------------------------------------------------------
add_executable(tick-main main-tick.cpp)
target_link_libraries(tick-main ratrac)
