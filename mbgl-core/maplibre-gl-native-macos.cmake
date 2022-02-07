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
        # MacOS specific files
        ${MBGL_SOURCE_DIR}/platform/darwin/src/async_task.cpp
        ${MBGL_SOURCE_DIR}/platform/darwin/src/gl_functions.cpp
        ${MBGL_SOURCE_DIR}/platform/darwin/src/run_loop.cpp
        ${MBGL_SOURCE_DIR}/platform/darwin/src/timer.cpp

        ${MBGL_SOURCE_DIR}/platform/darwin/src/collator.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/headless_backend_cgl.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/http_file_source.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/image.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/local_glyph_rasterizer.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/logging_nslog.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/native_apple_interface.m
        ${MBGL_SOURCE_DIR}/platform/darwin/src/nsthread.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/number_format.mm
        ${MBGL_SOURCE_DIR}/platform/darwin/src/string_nsstring.mm
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