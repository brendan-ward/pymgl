cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project("PyMBGL")

# from mapbox-gl-core
cmake_policy(SET CMP0063 NEW)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_EXTENSIONS OFF)
# set(CMAKE_C_STANDARD 99)
# set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_VISIBILITY_PRESET hidden)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
# set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)




# Build mbgl-core
option(MBGL_WITH_CORE_ONLY "Build only the core bits, no platform code" ON)
option(MBGL_WITH_OPENGL "Build with OpenGL renderer" ON)
add_subdirectory(vendor/mapbox-gl-native)

# find_package(mbgl-core REQUIRED)
# include_directories(SYSTEM ${mbgl-core_INCLUDE_DIRS})
# link_directories(${mbgl-core_LIBRARY_DIRS})

# target_sources(
#     pymap
#     INTERFACE
#         ${PROJECT_SOURCE_DIR/src/map.cpp}
# )

include_directories(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/vendor/mapbox-base/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/vendor/mapbox-base/deps/geometry.hpp/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/vendor/mapbox-base/deps/optional)
include_directories(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/vendor/mapbox-base/deps/variant/include)




add_subdirectory(vendor/pybind11)
pybind11_add_module(pymbgl SHARED ${PROJECT_SOURCE_DIR}/src/main.cpp)
# pybind11_add_module(pymbgl SHARED src/main.cpp src/map.cpp)

# TODO
target_link_libraries(pymbgl INTERFACE Mapbox::Map)



# # EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# # define (VERSION_INFO) here.
# target_compile_definitions(cmake_example PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})



# target_sources(
#     mbgl-node
#     INTERFACE
#         ${PROJECT_SOURCE_DIR}/platform/node/src/node_conversion.hpp
#         ${PROJECT_SOURCE_DIR}/platform/node/src/node_expression.cpp
#         ${PROJECT_SOURCE_DIR}/platform/node/src/node_expression.hpp
#         ${PROJECT_SOURCE_DIR}/platform/node/src/node_feature.cpp
#         ${PROJECT_SOURCE_DIR}/platform/node/src/node_feature.hpp
#         ${PROJECT_SOURCE_DIR}/platform/node/src/node_logging.cpp
#         ${PROJECT_SOURCE_DIR}/platform/node/src/node_logging.hpp
#         ${PROJECT_SOURCE_DIR}/platform/node/src/node_map.cpp
#         ${PROJECT_SOURCE_DIR}/platform/node/src/node_map.hpp
#         ${PROJECT_SOURCE_DIR}/platform/node/src/node_mapbox_gl_native.cpp
#         ${PROJECT_SOURCE_DIR}/platform/node/src/node_request.cpp
#         ${PROJECT_SOURCE_DIR}/platform/node/src/node_request.hpp
#         ${PROJECT_SOURCE_DIR}/platform/node/src/util/async_queue.hpp
# )

# target_link_libraries(
#     mbgl-node
#     INTERFACE Mapbox::Map
# )