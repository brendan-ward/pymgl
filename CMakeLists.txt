cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project("_pymbgl")

cmake_policy(SET CMP0079 NEW)

# Adapted from mapbox-gl-native
cmake_policy(SET CMP0063 NEW)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)  # 14 => 17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)


# Include required headers from mapbox-gl-native and dependencies
include_directories(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/vendor/mapbox-base/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/vendor/mapbox-base/deps/geometry.hpp/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/vendor/mapbox-base/deps/optional)
include_directories(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/vendor/mapbox-base/deps/variant/include)


# Build mbgl-core
include(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native-core.cmake)


add_subdirectory(vendor/pybind11)
pybind11_add_module(_pymbgl SHARED ${PROJECT_SOURCE_DIR}/src/_pymbgl.cpp)


target_link_libraries(
    _pymbgl
    PRIVATE
    Mapbox::Base
    mbgl-core
)

set_target_properties(_pymbgl PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
target_compile_definitions(_pymbgl PRIVATE VERSION_INFO=${VERSION_INFO})
