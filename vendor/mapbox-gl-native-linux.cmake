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
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/gfx/headless_backend.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/gfx/headless_frontend.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/gl/headless_backend.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/i18n/collator.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/i18n/number_format.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/layermanager/layer_manager.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/platform/time.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/asset_file_source.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/database_file_source.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/file_source_manager.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/file_source_request.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/http_file_source.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/local_file_request.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/local_file_source.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/main_resource_loader.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/offline.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/offline_database.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/offline_download.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/online_file_source.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/storage/sqlite3.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/text/bidi.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/text/local_glyph_rasterizer.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/async_task.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/compression.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/image.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/jpeg_reader.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/logging_stderr.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/monotonic_timer.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/png_reader.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/png_writer.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/run_loop.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/string_stdlib.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/thread.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/thread_local.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/timer.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/src/mbgl/util/utf.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/linux/src/gl_functions.cpp
        ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/linux/src/headless_backend_glx.cpp
)

# FIXME: Should not be needed, but now needed by node because of the headless frontend.
target_include_directories(
    mbgl-core
    PUBLIC ${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/platform/default/include
    PRIVATE
        ${CURL_INCLUDE_DIRS}
        ${JPEG_INCLUDE_DIRS}
        ${LIBUV_INCLUDE_DIRS}
        ${X11_INCLUDE_DIRS}
)

# Always use vendored ICU
include(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/vendor/icu.cmake)

include(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/vendor/nunicode.cmake)
include(${PROJECT_SOURCE_DIR}/vendor/mapbox-gl-native/vendor/sqlite.cmake)

target_link_libraries(
    mbgl-core
    PRIVATE
        ${CURL_LIBRARIES}
        ${JPEG_LIBRARIES}
        ${LIBUV_LIBRARIES}
        ${X11_LIBRARIES}
        OpenGL::GLX
        PNG::PNG
        mbgl-vendor-icu
        mbgl-vendor-nunicode
        mbgl-vendor-sqlite
)
