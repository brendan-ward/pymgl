cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project("_pymgl" VERSION "")

# Only use the active virtual environment of Python
set(Python_FIND_VIRTUALENV ONLY)
set(Python_FIND_FRAMEWORK NEVER)
find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(MLN_SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/maplibre-native)

# Adapted from maplibre-native/CMakeLists.txt
cmake_policy(SET CMP0063 NEW)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER Core)

if(NOT("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
endif()

# Hide warnings about no symbols
SET(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")

# Build mln-core
include(${PROJECT_SOURCE_DIR}/mln-core/maplibre-native-core.cmake)

# Include headers for pymgl and build the wrapper
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(
    mgl_wrapper STATIC
    ${PROJECT_SOURCE_DIR}/src/map.cpp
    ${PROJECT_SOURCE_DIR}/src/spng.c
)

# link against mln-core
target_link_libraries(
    mgl_wrapper PRIVATE
    Mapbox::Base
    mln-core
)

# Build Python module
add_subdirectory(vendor/nanobind)

nanobind_add_module(
    _pymgl
    NB_STATIC LTO
    ${PROJECT_SOURCE_DIR}/src/_pymgl.cpp
)

target_link_libraries(
    _pymgl
    PRIVATE
    mgl_wrapper
)

set_target_properties(_pymgl PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
target_compile_definitions(_pymgl PRIVATE VERSION_INFO=${VERSION_INFO})

# C++ tests
include(CTest)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests)