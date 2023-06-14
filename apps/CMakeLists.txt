macro(add_demo_app exe files)
  add_executable(${exe} ${files})
  target_link_libraries(${exe} ratrac)
  set_target_properties(${exe}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endmacro()

# Chapters from the book.
add_demo_app(tick ch2-tick.cpp)
add_demo_app(clock ch4-clock.cpp)
add_demo_app(silhouette ch5-silhouette.cpp)
add_demo_app(full3d ch6-full3d.cpp)
add_demo_app(scene ch7-scene.cpp)
add_demo_app(plane ch9-plane.cpp)
add_demo_app(patterns ch10-patterns.cpp)

# GOther programs and utilities.
add_demo_app(pattern-viewer pattern-viewer.cpp)