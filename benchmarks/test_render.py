from io import BytesIO

import pytest
import requests

# from PIL import Image
from pyvips import Image

from pymgl import Map

from pymgl.tests.common import FIXTURES_PATH, MAPBOX_TOKEN, read_style


MBGL_RENDERER_PATH = "http://localhost:8002/render"


COMPARE_MBGL_RENDERER = True


def render(style, width, height, ratio=1, bounds=None, **kwargs):
    map = Map(style, width, height, ratio, **kwargs)

    if bounds:
        map.setBounds(*bounds)

    map.renderPNG()


def render_buffer(style, width, height, ratio=1, bounds=None, **kwargs):
    map = Map(style, width, height, ratio, **kwargs)

    if bounds:
        map.setBounds(*bounds)

    buf = map.renderBuffer()
    # Pillow:
    # img = Image.frombytes("RGBA", (width, height), buf)
    # b = BytesIO()
    # img.save(b, format="PNG")
    # png = b.getvalue()

    # pyvips (about 2x faster write):
    img = Image.new_from_memory(buf, width, height, 4, "uchar")
    # match compression to internal
    b = img.pngsave_buffer(compression=3)


def render_mbgl_renderer(
    style, width, height, ratio=1, center=None, zoom=0, bounds=None, token=None
):
    data = {"style": style, "width": width, "height": height, "ratio": ratio}
    if token:
        data["token"] = token
    if bounds:
        data["bounds"] = ",".join(str(b) for b in bounds)
    else:
        if center is None:
            center = (0, 0)
        data["center"] = ",".join(str(c) for c in center)
        data["zoom"] = zoom

    r = requests.post(MBGL_RENDERER_PATH, data=data)
    r.raise_for_status()


@pytest.mark.benchmark(group="render-empty-256")
def test_render_empty_256(benchmark):
    style = read_style("example-style-empty.json")
    benchmark(render, style, 256, 256)


@pytest.mark.benchmark(group="render-empty-1024")
def test_render_empty_1024(benchmark):
    style = read_style("example-style-empty.json")
    benchmark(render, style, 1024, 1024)


@pytest.mark.benchmark(group="render-empty-2048")
def test_render_empty_2048(benchmark):
    style = read_style("example-style-empty.json")
    benchmark(render, style, 2048, 2048)


@pytest.mark.benchmark(group="render-empty-256")
def test_render_buffer_empty_256(benchmark):
    style = read_style("example-style-empty.json")
    benchmark(render_buffer, style, 256, 256)


@pytest.mark.benchmark(group="render-empty-1024")
def test_render_buffer_empty_1024(benchmark):
    style = read_style("example-style-empty.json")
    benchmark(render_buffer, style, 1024, 1024)


@pytest.mark.benchmark(group="render-empty-2048")
def test_render_buffer_empty_2048(benchmark):
    style = read_style("example-style-empty.json")
    benchmark(render_buffer, style, 2048, 2048)


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-empty-256")
def test_render_empty_mbglr_256(benchmark):
    style = read_style("example-style-empty.json")
    benchmark(
        render_mbgl_renderer,
        style,
        256,
        256,
    )


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-empty-1024")
def test_render_empty_mbglr_1024(benchmark):
    style = read_style("example-style-empty.json")
    benchmark(
        render_mbgl_renderer,
        style,
        1024,
        1024,
    )


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-empty-2048")
def test_render_empty_mbglr_2048(benchmark):
    style = read_style("example-style-empty.json")
    benchmark(
        render_mbgl_renderer,
        style,
        2048,
        2048,
    )


@pytest.mark.benchmark(group="render-geojson-256")
def test_render_geojson_256(benchmark):
    style = read_style("example-style-geojson.json")
    benchmark(render, style, 256, 256, bounds=(-125, 37.5, -115, 42.5))


@pytest.mark.benchmark(group="render-geojson-1024")
def test_render_geojson_1024(benchmark):
    style = read_style("example-style-geojson.json")
    benchmark(render, style, 1024, 1024, bounds=(-125, 37.5, -115, 42.5))


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-geojson-256")
def test_render_geojson_mbglr_256(benchmark):
    style = read_style("example-style-geojson.json")
    benchmark(
        render_mbgl_renderer,
        style,
        256,
        256,
    )


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-geojson-1024")
def test_render_geojson_mbglr_1024(benchmark):
    style = read_style("example-style-geojson.json")
    benchmark(
        render_mbgl_renderer,
        style,
        1024,
        1024,
    )


@pytest.mark.benchmark(group="render-geojson-256")
def test_render_file_geojson_256(benchmark):
    style = read_style("example-style-file-geojson.json")
    # update style from relative to absolute path
    style = style.replace("file://", f"file://{FIXTURES_PATH}/")
    benchmark(render, style, 256, 256, bounds=(-125, 37.5, -115, 42.5))


@pytest.mark.benchmark(group="render-remote-raster-256")
def test_render_remote_raster_256(benchmark):
    style = read_style("example-style-remote-raster.json")
    benchmark(render, style, 256, 256)


@pytest.mark.benchmark(group="render-remote-raster-1024")
def test_render_remote_raster_1024(benchmark):
    style = read_style("example-style-remote-raster.json")
    benchmark(render, style, 1024, 1024)


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-remote-raster-256")
def test_render_remote_raster_mbglr_256(benchmark):
    style = read_style("example-style-remote-raster.json")
    benchmark(
        render_mbgl_renderer,
        style,
        256,
        256,
    )


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-remote-raster-1024")
def test_render_remote_raster_mbglr_1024(benchmark):
    style = read_style("example-style-remote-raster.json")
    benchmark(
        render_mbgl_renderer,
        style,
        1024,
        1024,
    )


@pytest.mark.benchmark(group="render-remote-image-source-256")
def test_render_remote_image_source(benchmark):
    style = read_style("example-style-remote-image-source.json")
    benchmark(render, style, 256, 256)


# Uncomment these to run tests against Mapbox; these incur hits against the token
@pytest.mark.benchmark(group="render-mapbox-source-256")
def test_render_mapbox_256(benchmark):
    style = read_style("example-style-mapbox-source.json")
    benchmark(render, style, 256, 256, token=MAPBOX_TOKEN, provider="mapbox")


@pytest.mark.benchmark(group="render-mapbox-source-1024")
def test_render_mapbox_1024(benchmark):
    style = read_style("example-style-mapbox-source.json")
    benchmark(render, style, 1024, 1024, token=MAPBOX_TOKEN, provider="mapbox")


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-mapbox-source-256")
def test_render_mapbox_mbglr_256(benchmark):
    style = read_style("example-style-mapbox-source.json")
    benchmark(render_mbgl_renderer, style, 256, 256, token=MAPBOX_TOKEN)


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-mapbox-source-1024")
def test_render_mapbox_mbglr_1024(benchmark):
    style = read_style("example-style-mapbox-source.json")
    benchmark(render_mbgl_renderer, style, 1024, 1024, token=MAPBOX_TOKEN)


@pytest.mark.benchmark(group="render-labels")
def test_render_labels(benchmark):
    style = read_style("example-style-geojson-labels.json")
    benchmark(render, style, 256, 256)


@pytest.mark.benchmark(group="render-local-mbtiles-raster-256")
def test_render_local_mbtiles_raster_source_256(benchmark):
    style = read_style("example-style-mbtiles-raster-source.json")
    # update style from relative to absolute path
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    benchmark(render, style, 256, 256)


@pytest.mark.benchmark(group="render-local-mbtiles-raster-1024")
def test_render_local_mbtiles_raster_source_1024(benchmark):
    style = read_style("example-style-mbtiles-raster-source.json")
    # update style from relative to absolute path
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    benchmark(render, style, 1024, 1024)


@pytest.mark.benchmark(group="render-local-mbtiles-raster-2048")
def test_render_local_mbtiles_raster_source_2048(benchmark):
    style = read_style("example-style-mbtiles-raster-source.json")
    # update style from relative to absolute path
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    benchmark(render, style, 2048, 2048)


@pytest.mark.benchmark(group="render-local-mbtiles-raster-256")
def test_render_buffer_local_mbtiles_raster_source_256(benchmark):
    style = read_style("example-style-mbtiles-raster-source.json")
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    benchmark(render_buffer, style, 256, 256)


@pytest.mark.benchmark(group="render-local-mbtiles-raster-1024")
def test_render_buffer_local_mbtiles_raster_source_1024(benchmark):
    style = read_style("example-style-mbtiles-raster-source.json")
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    benchmark(render_buffer, style, 1024, 1024)


@pytest.mark.benchmark(group="render-local-mbtiles-raster-2048")
def test_render_buffer_local_mbtiles_raster_source_2048(benchmark):
    style = read_style("example-style-mbtiles-raster-source.json")
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    benchmark(render_buffer, style, 2048, 2048)


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-local-mbtiles-raster-256")
def test_render_local_mbtiles_raster_source_mbglr_256(benchmark):
    style = read_style("example-style-mbtiles-raster-source.json")
    style = style.replace(".mbtiles", "")
    benchmark(
        render_mbgl_renderer,
        style,
        256,
        256,
    )


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-local-mbtiles-raster-1024")
def test_render_local_mbtiles_raster_source_mbglr_1024(benchmark):
    style = read_style("example-style-mbtiles-raster-source.json")
    style = style.replace(".mbtiles", "")
    benchmark(
        render_mbgl_renderer,
        style,
        1024,
        1024,
    )


@pytest.mark.benchmark(group="render-local-mbtiles-vector-256")
def test_render_local_mbtiles_vector_source_256(benchmark):
    style = read_style("example-style-mbtiles-vector-source.json")
    # update style from relative to absolute path
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    benchmark(render, style, 256, 256)


@pytest.mark.benchmark(group="render-local-mbtiles-vector-1024")
def test_render_local_mbtiles_vector_source_1024(benchmark):
    style = read_style("example-style-mbtiles-vector-source.json")
    # update style from relative to absolute path
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    benchmark(render, style, 1024, 1024)


@pytest.mark.benchmark(group="render-local-mbtiles-vector-2048")
def test_render_local_mbtiles_vector_source_2048(benchmark):
    style = read_style("example-style-mbtiles-vector-source.json")
    # update style from relative to absolute path
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    benchmark(render, style, 2048, 2048)


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-local-mbtiles-vector-256")
def test_render_local_mbtiles_vector_source_mbglr_256(benchmark):
    style = read_style("example-style-mbtiles-vector-source.json")
    style = style.replace(".mbtiles", "")
    benchmark(
        render_mbgl_renderer,
        style,
        256,
        256,
    )


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-local-mbtiles-vector-1024")
def test_render_local_mbtiles_vector_source_mbglr_1024(benchmark):
    style = read_style("example-style-mbtiles-vector-source.json")
    style = style.replace(".mbtiles", "")
    benchmark(
        render_mbgl_renderer,
        style,
        1024,
        1024,
    )


@pytest.mark.skipif(not COMPARE_MBGL_RENDERER, reason="Not comparing to mbgl-renderer")
@pytest.mark.benchmark(group="render-local-mbtiles-vector-2048")
def test_render_local_mbtiles_vector_source_mbglr_2048(benchmark):
    style = read_style("example-style-mbtiles-vector-source.json")
    style = style.replace(".mbtiles", "")
    benchmark(
        render_mbgl_renderer,
        style,
        2048,
        2048,
    )


@pytest.mark.benchmark(group="render-pattern")
def test_image_pattern_256(benchmark):
    style = read_style("example-style-image-pattern.json")
    img = Image.open(FIXTURES_PATH / "example-pattern.png")

    def render_with_image():
        map = Map(style, 256, 256)
        map.setBounds(-125, 37.5, -115, 42.5)
        map.addImage("pattern", img.tobytes(), img.size[0], img.size[1], 1, False)
        map.renderPNG()

    benchmark(render_with_image)


@pytest.mark.benchmark(group="render-pattern")
def test_image_pattern_1024(benchmark):
    style = read_style("example-style-image-pattern.json")
    img = Image.open(FIXTURES_PATH / "example-pattern.png")

    def render_with_image():
        map = Map(style, 1024, 1024)
        map.setBounds(-125, 37.5, -115, 42.5)
        map.addImage("pattern", img.tobytes(), img.size[0], img.size[1], 1, False)
        map.renderPNG()

    benchmark(render_with_image)
