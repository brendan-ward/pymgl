# Build mbgl-core
set(MBGL_PUBLIC_BUILD TRUE)
option(MBGL_WITH_CORE_ONLY "Build only the core bits, no platform code" ON)
option(MBGL_WITH_OPENGL "Build with OpenGL renderer" ON)

### Include maplibre as a directory and build
add_subdirectory(${MBGL_SOURCE_DIR} build)


# from maplibre-gl-native/platform/macos and cmake files
# has notes about being needed by Node headless frontend
target_include_directories(
    mbgl-core
    PUBLIC ${MBGL_SOURCE_DIR}/platform/default/include
)

set_target_properties(mbgl-core PROPERTIES POSITION_INDEPENDENT_CODE TRUE)


# Build mbgl-core with specific platform requirements
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    message(STATUS "Building for Linux")
    include(${PROJECT_SOURCE_DIR}/vendor/maplibre-gl-native-linux.cmake)

elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    message(STATUS "Building for MacOS")
    include(${PROJECT_SOURCE_DIR}/vendor/maplibre-gl-native-macos.cmake)

else()
    message(FATAL_ERROR "Unsupported target platform: " ${CMAKE_SYSTEM_NAME})
endif()
