# ## Build mln-core
# Adapted from maplibre-gl-core/CMakeLists.txt

add_library(
    mbgl-compiler-options INTERFACE
)

# Derived from settings in maplibre-native CMakeLists.txt and extended to suppress more warnings
target_compile_options(
    mbgl-compiler-options
    INTERFACE
    "-Wall"
    "-Wextra"
    $<$<CXX_COMPILER_ID:GNU>:-Wno-error=maybe-uninitialized>
    $<$<CXX_COMPILER_ID:GNU>:-Wno-error=return-type>
    $<$<CXX_COMPILER_ID:GNU>:-Wno-error=unknown-pragmas>
    $<$<CXX_COMPILER_ID:GNU>:-Wno-psabi >
    $<$<CXX_COMPILER_ID:AppleClang>:-Wno-error=deprecated-declarations>
    $<$<CXX_COMPILER_ID:AppleClang>:-Wno-error=unused-parameter>
    $<$<CXX_COMPILER_ID:AppleClang>:-Wno-error=unused-property-ivar>
    $<$<CXX_COMPILER_ID:AppleClang>:-Wno-defaulted-function-deleted>
    "-Wno-unused-parameter"
    "-Wno-unused-variable"
    "-Wno-deprecated-declarations"
)

# Include headers we need from from maplibre-native
include_directories(${MLN_SOURCE_DIR}/include)
include_directories(${MLN_SOURCE_DIR}/platform/default/include)

# Some headers are in the src directory, just for fun
include_directories(${MLN_SOURCE_DIR}/src)

# Include headers we need from from maplibre-native dependencies
include_directories(${MLN_SOURCE_DIR}/vendor/mapbox-base/include)
include_directories(${MLN_SOURCE_DIR}/vendor/mapbox-base/deps/geometry.hpp/include)
include_directories(${MLN_SOURCE_DIR}/vendor/mapbox-base/deps/optional)
include_directories(${MLN_SOURCE_DIR}/vendor/mapbox-base/deps/variant/include)

# Set RPATH so that auditwheel repair fixes references within mln-core
set(RPATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
list(APPEND CMAKE_BUILD_RPATH ${RPATH})

# NOTE: removed all *.hpp file entries from below
add_library(
    mln-core STATIC
    ${MLN_SOURCE_DIR}/src/mbgl/actor/mailbox.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/actor/scheduler.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/annotation/annotation_manager.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/annotation/annotation_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/annotation/annotation_tile.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/annotation/fill_annotation_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/annotation/line_annotation_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/annotation/render_annotation_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/annotation/shape_annotation_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/annotation/symbol_annotation_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/geometry/dem_data.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/geometry/feature_index.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/geometry/line_atlas.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/attribute.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/polyline_generator.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/fill_generator.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/renderer_backend.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/rendering_stats.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/shader_group.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/gfx/shader_registry.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/background_layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/circle_layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/custom_layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/fill_extrusion_layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/fill_layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/heatmap_layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/hillshade_layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/layer_manager.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/line_layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/raster_layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layermanager/symbol_layer_factory.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layout/clip_lines.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layout/merge_lines.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layout/symbol_instance.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layout/symbol_layout.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/layout/symbol_projection.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/map/map.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/map/map_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/map/map_options.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/map/transform.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/map/transform_state.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/math/log2.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/platform/settings.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/background_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/circle_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/clipping_mask_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/collision_box_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/debug_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/fill_extrusion_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/fill_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/heatmap_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/heatmap_texture_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/hillshade_prepare_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/hillshade_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/line_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/program_parameters.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/programs.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/raster_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/programs/symbol_program.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/backend_scope.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/bucket_parameters.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/buckets/circle_bucket.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/buckets/debug_bucket.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/buckets/fill_bucket.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/buckets/fill_extrusion_bucket.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/buckets/heatmap_bucket.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/buckets/hillshade_bucket.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/buckets/line_bucket.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/buckets/raster_bucket.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/buckets/symbol_bucket.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/cross_faded_property_evaluator.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/group_by_layout.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/image_atlas.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/image_manager.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_background_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_circle_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_custom_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_fill_extrusion_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_fill_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_heatmap_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_hillshade_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_line_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_raster_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/layers/render_symbol_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/paint_parameters.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/pattern_atlas.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/render_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/render_light.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/render_orchestrator.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/render_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/render_static_data.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/render_tile.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/renderer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/renderer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/renderer_state.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/sources/render_custom_geometry_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/sources/render_geojson_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/sources/render_image_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/sources/render_raster_dem_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/sources/render_raster_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/sources/render_tile_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/sources/render_vector_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/source_state.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/style_diff.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/tile_pyramid.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/renderer/tile_render_data.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/sprite/sprite_loader.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/sprite/sprite_parser.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/storage/file_source_manager.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/storage/network_status.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/storage/resource.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/storage/resource_options.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/storage/resource_transform.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/storage/response.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/color_ramp_property_value.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/constant.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/coordinate.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/custom_geometry_source_options.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/filter.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/function.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/geojson.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/geojson_options.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/get_json_type.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/light.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/position.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/property_value.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/rotation.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/sprite.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/tileset.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/conversion/transition_options.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/custom_tile_loader.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/assertion.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/at.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/boolean_operator.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/case.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/check_subtype.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/coalesce.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/coercion.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/collator.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/collator_expression.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/comparison.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/compound_expression.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/distance.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/dsl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/expression.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/find_zoom_curve.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/format_expression.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/formatted.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/get_covering_stops.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/image.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/image_expression.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/in.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/index_of.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/interpolate.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/is_constant.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/is_expression.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/length.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/let.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/literal.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/match.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/number_format.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/parsing_context.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/slice.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/step.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/util.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/value.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/expression/within.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/filter.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/image.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/image_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/light.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/light_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/parser.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/property_expression.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/source_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/sprite.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/style.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/style_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/types.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/background_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/background_layer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/background_layer_properties.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/circle_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/circle_layer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/circle_layer_properties.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/custom_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/custom_layer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/custom_layer_render_parameters.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/fill_extrusion_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/fill_extrusion_layer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/fill_extrusion_layer_properties.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/fill_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/fill_layer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/fill_layer_properties.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/heatmap_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/heatmap_layer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/heatmap_layer_properties.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/hillshade_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/hillshade_layer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/hillshade_layer_properties.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/line_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/line_layer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/line_layer_properties.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/raster_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/raster_layer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/raster_layer_properties.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/symbol_layer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/symbol_layer_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/layers/symbol_layer_properties.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/sources/custom_geometry_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/sources/custom_geometry_source_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/sources/geojson_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/sources/geojson_source_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/sources/image_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/sources/image_source_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/sources/raster_dem_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/sources/raster_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/sources/raster_source_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/sources/vector_source.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/style/sources/vector_source_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/check_max_angle.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/collision_feature.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/collision_index.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/cross_tile_symbol_index.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/get_anchors.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/glyph.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/glyph_atlas.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/glyph_manager.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/glyph_pbf.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/language_tag.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/placement.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/quads.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/shaping.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/text/tagged_string.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/custom_geometry_tile.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/geojson_tile.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/geometry_tile.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/geometry_tile_data.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/geometry_tile_worker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/raster_dem_tile.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/raster_dem_tile_worker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/raster_tile.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/raster_tile_worker.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/tile.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/tile_cache.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/tile_id_hash.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/tile_id_io.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/vector_tile.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/tile/vector_tile_data.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/bounding_volumes.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/camera.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/chrono.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/client_options.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/color.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/constants.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/convert.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/event.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/font_stack.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/geo.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/geojson_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/geometry_util.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/grid_index.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/http_header.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/http_timeout.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/i18n.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/identity.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/interpolate.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/intersection_tests.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/io.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/logging.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/mapbox.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/mat2.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/mat3.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/mat4.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/premultiply.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/quaternion.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/rapidjson.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/stopwatch.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/string.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/string_indexer.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/thread.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/thread_pool.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/tile_cover.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/tile_cover_impl.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/default_style.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/tile_server_options.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/tiny_sdf.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/url.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/version.cpp
    ${MLN_SOURCE_DIR}/src/mbgl/util/work_request.cpp

    # Consolidated from platform / default plus common ones shared between MacOS and Linux
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/gfx/headless_backend.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/gfx/headless_frontend.cpp

    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/layermanager/layer_manager.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/map/map_snapshotter.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/platform/time.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/asset_file_source.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/database_file_source.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/mbtiles_file_source.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/file_source_manager.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/file_source_request.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/local_file_request.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/local_file_source.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/main_resource_loader.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/offline.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/offline_database.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/offline_download.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/online_file_source.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/storage/sqlite3.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/text/bidi.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/compression.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/filesystem.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/monotonic_timer.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/png_writer.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/thread_local.cpp
    ${MLN_SOURCE_DIR}/platform/default/src/mbgl/util/utf.cpp
)

set_target_properties(mln-core PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

target_compile_definitions(
    mln-core
    PUBLIC
    MLN_USE_UNORDERED_DENSE=1

    # disable location indicator; not used for static rendering
    MBGL_LAYER_LOCATION_INDICATOR_DISABLE_ALL=1
)

if(EXISTS ${MLN_SOURCE_DIR}/.git/HEAD)
    execute_process(
        COMMAND
        git
        rev-parse
        --short=8
        HEAD
        WORKING_DIRECTORY ${MLN_SOURCE_DIR}
        OUTPUT_VARIABLE MBGL_VERSION_REV
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(MBGL_VERSION_REV 00000000)
endif()

set_source_files_properties(
    ${MLN_SOURCE_DIR}/src/mbgl/util/version.cpp
    PROPERTIES
    COMPILE_DEFINITIONS
    MBGL_VERSION_REV="${MBGL_VERSION_REV}"
)

include(${MLN_SOURCE_DIR}/vendor/boost.cmake)
include(${MLN_SOURCE_DIR}/vendor/csscolorparser.cmake)
include(${MLN_SOURCE_DIR}/vendor/earcut.hpp.cmake)
include(${MLN_SOURCE_DIR}/vendor/eternal.cmake)
include(${MLN_SOURCE_DIR}/vendor/mapbox-base.cmake)
include(${MLN_SOURCE_DIR}/vendor/parsedate.cmake)
include(${MLN_SOURCE_DIR}/vendor/polylabel.cmake)
include(${MLN_SOURCE_DIR}/vendor/protozero.cmake)
include(${MLN_SOURCE_DIR}/vendor/unique_resource.cmake)
add_subdirectory(${MLN_SOURCE_DIR}/vendor/unordered_dense)
include(${MLN_SOURCE_DIR}/vendor/vector-tile.cmake)
include(${MLN_SOURCE_DIR}/vendor/wagyu.cmake)

target_link_libraries(
    mln-core
    PRIVATE
    Mapbox::Base::Extras::kdbush.hpp
    Mapbox::Base::supercluster.hpp
    Mapbox::Base::shelf-pack-cpp
    Mapbox::Base::geojson-vt-cpp
    Mapbox::Base::cheap-ruler-cpp
    mbgl-compiler-options
    mbgl-vendor-boost
    mbgl-vendor-csscolorparser
    mbgl-vendor-earcut.hpp
    mbgl-vendor-eternal
    mbgl-vendor-parsedate
    mbgl-vendor-polylabel
    mbgl-vendor-protozero
    mbgl-vendor-unique_resource
    mbgl-vendor-vector-tile
    mbgl-vendor-wagyu
    PUBLIC
    Mapbox::Base
    Mapbox::Base::Extras::expected-lite
    Mapbox::Base::Extras::rapidjson
    Mapbox::Base::geojson.hpp
    Mapbox::Base::geometry.hpp
    Mapbox::Base::variant
    unordered_dense
)

add_library(
    Mapbox::Map ALIAS mln-core
)

# # End maplibre-core/CMakeLists.txt

# Build mln-core with specific platform requirements
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    message(STATUS "Building for Linux")
    include(${PROJECT_SOURCE_DIR}/mln-core/maplibre-native-linux.cmake)

elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    message(STATUS "Building for MacOS")
    include(${PROJECT_SOURCE_DIR}/mln-core/maplibre-native-macos.cmake)

else()
    message(FATAL_ERROR "Unsupported target platform: " ${CMAKE_SYSTEM_NAME})
endif()