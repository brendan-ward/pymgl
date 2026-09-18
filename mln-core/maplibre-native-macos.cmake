# match dependencies
set(CMAKE_OSX_DEPLOYMENT_TARGET "14.3")

set_target_properties(mln-core PROPERTIES XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES)

# Always require Metal
message(STATUS "Configuring Metal renderer backend")
find_library(METAL_FRAMEWORK Metal REQUIRED)

set(MLN_WITH_METAL ON)

target_compile_definitions(
    mln-core
    PRIVATE

    # See comments in vendor/maplibre-native/CMakeLists.txt about AutoreleasePool
    MLN_CREATE_AUTORELEASEPOOL=1

    MLN_RENDER_BACKEND_METAL=1
    MLN_RENDER_BACKEND_OPENGL=0
    PUBLIC

    MLN_WITH_METAL=1
    MLN_WITH_PMTILES=1
)

target_link_libraries(
    mln-core
    PRIVATE
    ${METAL_FRAMEWORK}
    mbgl-vendor-metal-cpp
)
target_sources(
    mln-core
    PRIVATE

    # from vendor/maplibre-native/cmake/metal.cmake
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/buffer_resource.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/command_encoder.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/context.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/drawable.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/drawable_builder.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/dynamic_texture.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/index_buffer_resource.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/layer_group.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/mtl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/offscreen_texture.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/renderer_backend.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/texture2d.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/render_pass.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/tile_layer_group.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/uniform_buffer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/upload_pass.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/vertex_attribute.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/vertex_buffer_resource.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/shader_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/background.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/circle.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/collision.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/clipping_mask.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/custom_geometry.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/custom_symbol_icon.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/debug.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/fill.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/fill_extrusion.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/heatmap.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/heatmap_texture.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/hillshade.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/hillshade_prepare.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/color_relief.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/line.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/location_indicator.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/raster.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/symbol.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/widevector.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/mtl/custom_layer_render_parameters.cpp

    # from vendor/maplibre-native/platform/macos/macos.cmake
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/mtl/headless_backend.cpp

    # from vendor/maplibre-native/platform/darwin/darwin.cmake
    ${MLN_SOURCE_DIR}/platform/darwin/core/async_task.cpp
    ${MLN_SOURCE_DIR}/platform/darwin/core/run_loop.cpp
    ${MLN_SOURCE_DIR}/platform/darwin/core/timer.cpp
    ${MLN_SOURCE_DIR}/platform/darwin/core/collator.mm
    ${MLN_SOURCE_DIR}/platform/darwin/core/http_file_source.mm
    ${MLN_SOURCE_DIR}/platform/darwin/core/image.mm
    ${MLN_SOURCE_DIR}/platform/darwin/core/local_glyph_rasterizer.mm
    ${MLN_SOURCE_DIR}/platform/darwin/core/logging_nslog.mm
    ${MLN_SOURCE_DIR}/platform/darwin/core/native_apple_interface.m
    ${MLN_SOURCE_DIR}/platform/darwin/core/nsthread.mm
    ${MLN_SOURCE_DIR}/platform/darwin/core/number_format.mm
    ${MLN_SOURCE_DIR}/platform/darwin/core/string_nsstring.mm
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