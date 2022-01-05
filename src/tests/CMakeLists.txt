

# To create test executable to exercise the MBGL wrapper in C++
add_executable (
    test_wrapper
    ${PROJECT_SOURCE_DIR}/src/map.cpp
    ${PROJECT_SOURCE_DIR}/src/tests/test_mbgl_wrapper.cpp
)

target_link_libraries(test_wrapper mbgl-core)