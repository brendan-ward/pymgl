

# FIXME: only temporary testing of rendering
add_executable (
    pymbgl-test
    ${PROJECT_SOURCE_DIR}/src/map.cpp
    # ${PROJECT_SOURCE_DIR}/src/tests/test.cpp
    ${PROJECT_SOURCE_DIR}/src/tests/test_pymap.cpp
)

target_link_libraries(pymbgl-test mbgl-core)