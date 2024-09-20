find_package(CURL REQUIRED)

# NOTE: ICU is optional and backfilled by vendored library in maplibre-native
# but we require installing a modern version of it
find_package(ICU REQUIRED i18n)
find_package(ICU REQUIRED uc)

find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)

# NOTE: EGL is optional and backfilled by GLX in mapblibre-native, but
# we require installing EGL to simplify build
find_package(OpenGL REQUIRED EGL)

pkg_search_module(WEBP libwebp REQUIRED)
pkg_search_module(LIBUV libuv REQUIRED)

target_compile_definitions(
    mln-core
    PRIVATE
    MLN_RENDER_BACKEND_OPENGL=1
    PUBLIC
    MLN_LEGACY_RENDERER=1
    MLN_DRAWABLE_RENDERER=0
)

# NOTE: might need to use the legacy renderer and disable drawable renderer
target_sources(
    mln-core
    PRIVATE

    # OpenGL-specific files
    ${MLN_SOURCE_DIR}/src/mbgl/gl/attribute.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/command_encoder.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/context.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/debugging_extension.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/enum.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/fence.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/index_buffer_resource.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/object.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/offscreen_texture.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/render_pass.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/renderer_backend.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/renderbuffer_resource.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/resource_pool.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/texture.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/texture_resource.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/timestamp_query_extension.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/uniform.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/upload_pass.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/value.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/vertex_array.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gl/vertex_buffer_resource.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/location_indicator_layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/platform/gl_functions.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_custom_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_location_indicator_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/location_indicator_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/location_indicator_layer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/location_indicator_layer_properties.cpp

    # other Linux-specific files
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/i18n/collator.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/i18n/number_format.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/http_file_source.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/text/local_glyph_rasterizer.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/async_task.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/compression.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/image.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/jpeg_reader.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/logging_stderr.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/png_reader.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/run_loop.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/string_stdlib.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/thread.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/timer.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/webp_reader.cpp

    ${MLN_SOURCE_DIR}/platform/linux/src/gl_functions.cpp

    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/gl/headless_backend.cpp
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
    ${CMAKE_THREAD_LIBS_INIT}
    ${WEBP_LIBRARIES}
    ICU::i18n
    ICU::uc
    PNG::PNG
    mbgl-vendor-nunicode
    mbgl-vendor-sqlite
    OpenGL::EGL
)
