### C++ unit tests

# Add GoogleTest
include_directories(
    ${MBGL_SOURCE_DIR}/vendor/googletest/googletest
    ${MBGL_SOURCE_DIR}/vendor/googletest/googletest/include
)
add_library(
    gtest STATIC EXCLUDE_FROM_ALL
    ${MBGL_SOURCE_DIR}/vendor/googletest/googletest/src/gtest-all.cc
)
add_library(
    gtest_main STATIC
    ${MBGL_SOURCE_DIR}/vendor/googletest/googletest/src/gtest_main.cc
)
target_link_libraries(gtest_main gtest)

# Find all tests with pattern *Test.cpp in this directory
include_directories(${MBGL_SOURCE_DIR}/tests)
file(GLOB_RECURSE _sources ${PROJECT_SOURCE_DIR}/tests/*.cpp CONFIGURE_DEPEND)
add_executable(
    pymgl_test
    ${PROJECT_SOURCE_DIR}/tests/util.cpp
    ${_sources}
)
unset(_sources)

target_link_libraries(
    pymgl_test PRIVATE
    gtest_main
    Mapbox::Base::pixelmatch-cpp
    mgl_wrapper
)

file(GLOB_RECURSE _testfiles ${PROJECT_SOURCE_DIR}/tests/*Test.cpp CONFIGURE_DEPEND)
foreach(_testfile ${_testfiles})
    string(REPLACE ${PROJECT_SOURCE_DIR}/ "" _testname ${_testfile})
    string(REPLACE "Test.cpp" "" _testname ${_testname})
    string(REPLACE "/" "-" _cmake_testname  ${_testname})
    string(REPLACE "/" "::" _testname ${_testname})
    add_test(NAME unit-${_cmake_testname} COMMAND pymgl_test ${_testname})
endforeach()

unset(_cmake_testname)
unset(_testfile)
unset(_testfiles)
unset(_testname)
