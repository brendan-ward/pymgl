find_package(CURL REQUIRED)

# NOTE: ICU is optional and backfilled by vendored library in maplibre-gl-native
# but we require installing a modern version of it
find_package(ICU REQUIRED i18n)
find_package(ICU REQUIRED uc)

find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)

# NOTE: EGL is optional and backfilled by GLX in mapblibre-gl-native, but
# we require installing EGL to simplify build
find_package(OpenGL REQUIRED EGL)

pkg_search_module(LIBUV libuv REQUIRED)

target_sources(
    mln-core
    PRIVATE
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/http_file_source.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/i18n/collator.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/i18n/number_format.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/text/local_glyph_rasterizer.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/async_task.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/compression.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/image.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/jpeg_reader.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/logging_stderr.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/png_reader.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/png_writer.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/run_loop.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/string_stdlib.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/thread.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/timer.cpp
    ${MLN_SOURCE_DIR}/platform/linux/src/gl_functions.cpp

    ${MLN_SOURCE_DIR}/platform/linux/src/headless_backend_egl.cpp
)

include(${MLN_SOURCE_DIR}/vendor/nunicode.cmake)
include(${MLN_SOURCE_DIR}/vendor/sqlite.cmake)

target_link_libraries(
    mln-core
    PRIVATE
    ${CURL_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${LIBUV_LIBRARIES}
    ${X11_LIBRARIES}
    ICU::i18n
    ICU::uc
    PNG::PNG
    mbgl-vendor-nunicode
    mbgl-vendor-sqlite
    OpenGL::EGL
)
