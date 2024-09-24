set(CMAKE_OSX_DEPLOYMENT_TARGET "12")

set_target_properties(mln-core PROPERTIES XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES)

# Always require Metal
message(STATUS "Configuring Metal renderer backend")
find_library(METAL_FRAMEWORK Metal REQUIRED)

include(${MLN_SOURCE_DIR}/vendor/metal-cpp.cmake)

target_compile_definitions(
    mln-core
    PRIVATE
    MLN_RENDER_BACKEND_METAL=1
    MLN_RENDER_BACKEND_OPENGL=0
    PUBLIC
    MLN_DRAWABLE_RENDERER=1
    MLN_LEGACY_RENDERER=0
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

    # compiled from top-level maplibre-native CMakeLists.txt drawable sections
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/drawable.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/drawable_atlases_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/drawable_builder.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/drawable_builder_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/drawable_custom_layer_host_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/gpu_expression.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/uniform_block.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/uniform_buffer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/vertex_attribute.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/custom_drawable_layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/change_request.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layer_group.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/background_layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/circle_layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/collision_layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/fill_extrusion_layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/fill_layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/heatmap_layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/heatmap_texture_layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/hillshade_layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/hillshade_prepare_layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/line_layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/raster_layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_custom_drawable_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/symbol_layer_tweaker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/render_target.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/tile_layer_group.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/shader_program_base.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/custom_drawable_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/custom_drawable_layer_impl.cpp

    # from top-level CMakeLists.txt metal section
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/buffer_resource.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/command_encoder.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/context.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/drawable.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/drawable_builder.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/index_buffer_resource.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/layer_group.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/mtl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/offscreen_texture.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/render_pass.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/renderer_backend.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/texture2d.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/tile_layer_group.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/uniform_block.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/uniform_buffer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/upload_pass.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/vertex_attribute.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/mtl/vertex_buffer_resource.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/background.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/background_pattern.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/circle.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/clipping_mask.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/collision_box.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/collision_circle.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/custom_symbol_icon.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/debug.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/fill.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/fill_extrusion.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/fill_extrusion_pattern.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/heatmap.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/heatmap_texture.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/hillshade.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/hillshade_prepare.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/line.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/raster.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/shader_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/symbol_icon.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/symbol_sdf.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/shaders/mtl/symbol_text_and_icon.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/mtl/custom_layer_render_parameters.cpp

    # from macos.cmake metal section
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/mtl/headless_backend.cpp

    # MacOS specific files
    ${MLN_SOURCE_DIR}/platform/darwin/src/async_task.cpp
    ${MLN_SOURCE_DIR}/platform/darwin/src/run_loop.cpp
    ${MLN_SOURCE_DIR}/platform/darwin/src/timer.cpp
    ${MLN_SOURCE_DIR}/platform/darwin/src/collator.mm
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