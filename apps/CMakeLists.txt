macro(add_demo_app exe files)
  add_executable(${exe} ${files})
  target_link_libraries(${exe} ratrac)
  set_target_properties(${exe}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endmacro()

add_demo_app(tick main-tick.cpp)
add_demo_app(clock main-clock.cpp)
add_demo_app(silhouette main-silhouette.cpp)
add_demo_app(full3d main-full3d.cpp)
add_demo_app(scene main-scene.cpp)
add_demo_app(plane main-plane.cpp)
add_demo_app(patterns main-patterns.cpp)
