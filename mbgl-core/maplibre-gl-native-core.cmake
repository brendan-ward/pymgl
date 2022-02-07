### Build mbgl-core
set(MBGL_PUBLIC_BUILD TRUE)
option(MBGL_WITH_CORE_ONLY "Build only the core bits, no platform code" ON)
option(MBGL_WITH_OPENGL "Build with OpenGL renderer" ON)

# Include maplibre as a directory and build
add_subdirectory(${MBGL_SOURCE_DIR} build)
set_target_properties(mbgl-core PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

# Build common mbgl-core
target_sources(
    mbgl-core
    PRIVATE
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/gfx/headless_backend.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/gfx/headless_frontend.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/gl/headless_backend.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/layermanager/layer_manager.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/map/map_snapshotter.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/platform/time.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/asset_file_source.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/database_file_source.cpp

        # TODO: change this once https://github.com/maplibre/maplibre-gl-native/pull/198 is merged
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/mbtiles_file_source.cpp

        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/file_source_manager.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/file_source_request.cpp

        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/local_file_request.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/local_file_source.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/main_resource_loader.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/offline.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/offline_database.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/offline_download.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/online_file_source.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/sqlite3.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/text/bidi.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/compression.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/monotonic_timer.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/png_writer.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/thread_local.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/utf.cpp
)



# Build mbgl-core with specific platform requirements
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    message(STATUS "Building for Linux")
    include(${PROJECT_SOURCE_DIR}/mbgl-core/maplibre-gl-native-linux.cmake)

elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    message(STATUS "Building for MacOS")
    include(${PROJECT_SOURCE_DIR}/mbgl-core/maplibre-gl-native-macos.cmake)

else()
    message(FATAL_ERROR "Unsupported target platform: " ${CMAKE_SYSTEM_NAME})
endif()