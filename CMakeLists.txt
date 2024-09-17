cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0048 NEW)

project("_pymgl" VERSION "")

cmake_minimum_required(VERSION 3.18...3.22)

# find Python (note: this looks for at least Python 3.9)
set(Python_FIND_FRAMEWORK LAST) # Prefer Brew/Conda to Apple framework python
find_package(Python 3.9 COMPONENTS Interpreter Development.Module REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(MBGL_SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/maplibre-gl-native)

# Adapted from maplibre-gl-native/CMakeLists.txt
cmake_policy(SET CMP0063 NEW)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
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

# ## Include headers we need from from maplibre-gl-native
include_directories(${MBGL_SOURCE_DIR}/include)
include_directories(${MBGL_SOURCE_DIR}/platform/default/include)

# Some headers are in the src directory, just for fun
include_directories(${MBGL_SOURCE_DIR}/src)

# ## Include headers we need from from maplibre-gl-native dependencies
include_directories(${MBGL_SOURCE_DIR}/vendor/mapbox-base/include)
include_directories(${MBGL_SOURCE_DIR}/vendor/mapbox-base/deps/geometry.hpp/include)
include_directories(${MBGL_SOURCE_DIR}/vendor/mapbox-base/deps/optional)
include_directories(${MBGL_SOURCE_DIR}/vendor/mapbox-base/deps/variant/include)

# ## Build mbgl-core
include(${PROJECT_SOURCE_DIR}/mbgl-core/maplibre-gl-native-core.cmake)

# ## Include headers for pymgl and build the wrapper
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(
    mgl_wrapper STATIC
    ${PROJECT_SOURCE_DIR}/src/map.cpp
    ${PROJECT_SOURCE_DIR}/src/spng.c
)

target_link_libraries(
    mgl_wrapper PRIVATE
    Mapbox::Base
    mbgl-core
)

# ## Build Python module
add_subdirectory(vendor/nanobind)

nanobind_add_module(
    _pymgl
    NB_STATIC LTO
    ${PROJECT_SOURCE_DIR}/src/_pymgl.cpp
)

# link against mbgl-core
target_link_libraries(
    _pymgl
    PRIVATE
    mgl_wrapper
)

set_target_properties(_pymgl PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
target_compile_definitions(_pymgl PRIVATE VERSION_INFO=${VERSION_INFO})

# ## C++ tests
include(CTest)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests)