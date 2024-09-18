# Note: newer than required by maplibre-gl-native but needed for std::optional support
set(CMAKE_OSX_DEPLOYMENT_TARGET "12")

set_target_properties(mln-core PROPERTIES XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES)

# Always build with OpenGL
find_package(OpenGL REQUIRED)

target_compile_definitions(
    mln-core
    PUBLIC GL_SILENCE_DEPRECATION
)

target_link_libraries(
    mln-core
    PRIVATE OpenGL::GL
)

target_sources(
    mln-core
    PRIVATE

    # MacOS specific files
    ${MLN_SOURCE_DIR}/platform/darwin/src/async_task.cpp
    ${MLN_SOURCE_DIR}/platform/darwin/src/gl_functions.cpp
    ${MLN_SOURCE_DIR}/platform/darwin/src/run_loop.cpp
    ${MLN_SOURCE_DIR}/platform/darwin/src/timer.cpp

    ${MLN_SOURCE_DIR}/platform/darwin/src/collator.mm
    ${MLN_SOURCE_DIR}/platform/darwin/src/headless_backend_cgl.mm
    ${MLN_SOURCE_DIR}/platform/darwin/src/http_file_source.mm
    ${MLN_SOURCE_DIR}/platform/darwin/src/image.mm
    ${MLN_SOURCE_DIR}/platform/darwin/src/local_glyph_rasterizer.mm
    ${MLN_SOURCE_DIR}/platform/darwin/src/logging_nslog.mm
    ${MLN_SOURCE_DIR}/platform/darwin/src/native_apple_interface.m
    ${MLN_SOURCE_DIR}/platform/darwin/src/nsthread.mm
    ${MLN_SOURCE_DIR}/platform/darwin/src/number_format.mm
    ${MLN_SOURCE_DIR}/platform/darwin/src/string_nsstring.mm
)

target_compile_options(mln-core PRIVATE -fobjc-arc)

target_include_directories(
    mln-core
    PRIVATE
    ${MLN_SOURCE_DIR}/platform/darwin/include
    ${MLN_SOURCE_DIR}/platform/darwin/src
    ${MLN_SOURCE_DIR}/platform/macos/src
)

include(${MLN_SOURCE_DIR}/vendor/icu.cmake)

target_link_libraries(
    mln-core
    PRIVATE
    "-framework AppKit"
    "-framework CoreGraphics"
    "-framework CoreLocation"
    "-framework SystemConfiguration"
    mbgl-vendor-icu
    sqlite3
    z
)