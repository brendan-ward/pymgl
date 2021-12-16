# Note: newer than required by maplibre-gl-native but needed for std::optional support
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")

set_target_properties(mbgl-core PROPERTIES XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES)

# Always build with OpenGL
find_package(OpenGL REQUIRED)

target_compile_definitions(
    mbgl-core
    PUBLIC GL_SILENCE_DEPRECATION
)

target_link_libraries(
    mbgl-core
    PRIVATE OpenGL::GL
)

target_sources(
    mbgl-core
    PRIVATE
        # for OpenGL
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/gl/headless_backend.cpp
        ${MBGL_SOURCE_DIR}/platform/darwin/src/gl_functions.cpp
        ${MBGL_SOURCE_DIR}/platform/darwin/src/headless_backend_cgl.mm

        # everything else
        ${MBGL_SOURCE_DIR}/platform/darwin/src/async_task.cpp
        ${MBGL_SOURCE_DIR}/platform/darwin/src/collator.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/http_file_source.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/image.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/local_glyph_rasterizer.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/logging_nslog.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/native_apple_interface.m
        ${MBGL_SOURCE_DIR}/platform/darwin/src/nsthread.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/number_format.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/run_loop.cpp
        ${MBGL_SOURCE_DIR}/platform/darwin/src/string_nsstring.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/timer.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/gfx/headless_backend.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/gfx/headless_frontend.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/layermanager/layer_manager.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/map/map_snapshotter.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/platform/time.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/asset_file_source.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/mbtiles_file_source.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/database_file_source.cpp
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

target_compile_options(mbgl-core PRIVATE -fobjc-arc)

target_include_directories(
    mbgl-core
    PRIVATE
    ${MBGL_SOURCE_DIR}/platform/darwin/include
    ${MBGL_SOURCE_DIR}/platform/darwin/src
    ${MBGL_SOURCE_DIR}/platform/macos/src
)

include(${MBGL_SOURCE_DIR}/vendor/icu.cmake)

target_link_libraries(
    mbgl-core
    PRIVATE
        "-framework AppKit"
        "-framework CoreGraphics"
        "-framework CoreLocation"
        "-framework SystemConfiguration"
        mbgl-vendor-icu
        sqlite3
        z
)