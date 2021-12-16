find_package(CURL REQUIRED)
find_package(JPEG REQUIRED)
find_package(OpenGL REQUIRED GLX)
find_package(PNG REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)

pkg_search_module(LIBUV libuv REQUIRED)

target_sources(
    mbgl-core
    PRIVATE
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/gfx/headless_backend.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/gfx/headless_frontend.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/gl/headless_backend.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/i18n/collator.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/i18n/number_format.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/layermanager/layer_manager.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/platform/time.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/asset_file_source.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/database_file_source.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/file_source_manager.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/file_source_request.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/http_file_source.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/local_file_request.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/local_file_source.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/main_resource_loader.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/offline.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/offline_database.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/offline_download.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/online_file_source.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/sqlite3.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/text/bidi.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/text/local_glyph_rasterizer.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/async_task.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/compression.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/image.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/jpeg_reader.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/logging_stderr.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/monotonic_timer.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/png_reader.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/png_writer.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/run_loop.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/string_stdlib.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/thread.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/thread_local.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/timer.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/utf.cpp
        ${MBGL_SOURCE_DIR}/platform/linux/src/gl_functions.cpp
        ${MBGL_SOURCE_DIR}/platform/linux/src/headless_backend_glx.cpp
)

# FIXME: Should not be needed, but now needed by node because of the headless frontend.
target_include_directories(
    mbgl-core
    PUBLIC ${MBGL_SOURCE_DIR}/platform/default/include
    PRIVATE
        ${CURL_INCLUDE_DIRS}
        ${JPEG_INCLUDE_DIRS}
        ${LIBUV_INCLUDE_DIRS}
        ${X11_INCLUDE_DIRS}
)

# Always use vendored ICU
# TODO: verify if ICU has been replaced / removed
include(${MBGL_SOURCE_DIR}/vendor/icu.cmake)

include(${MBGL_SOURCE_DIR}/vendor/nunicode.cmake)
include(${MBGL_SOURCE_DIR}/vendor/sqlite.cmake)

target_link_libraries(
    mbgl-core
    PRIVATE
        ${CURL_LIBRARIES}
        ${JPEG_LIBRARIES}
        ${LIBUV_LIBRARIES}
        ${X11_LIBRARIES}
        # TODO: remove GLX line here, not in maplibre linux makefile
        OpenGL::GLX
        PNG::PNG
        mbgl-vendor-icu
        mbgl-vendor-nunicode
        mbgl-vendor-sqlite
)
