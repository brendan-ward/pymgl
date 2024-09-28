import json
import os

from PIL import Image
import pytest

from pymgl import Map

from .common import FIXTURES_PATH, MAPBOX_TOKEN, read_style, image_matches


has_poorconn = False
try:
    import poorconn  # noqa

    pytest_plugins = ("poorconn",)
    has_poorconn = True
except ImportError:
    pass


def test_empty_style(empty_style):
    img_data = Map(empty_style, 10, 10).renderPNG()

    assert image_matches(img_data, "example-style-empty.png")


def test_geojson():
    test = "example-style-geojson"
    map = Map(read_style(f"{test}.json"), 100, 100)
    map.setBounds(-125, 37.5, -115, 42.5)
    img_data = map.renderPNG()

    assert image_matches(img_data, f"{test}.png")


def test_file_geojson():
    test = "example-style-file-geojson"
    style = read_style(f"{test}.json")

    # update style from relative to absolute path
    style = style.replace("file://", f"file://{FIXTURES_PATH}/")

    map = Map(style, 100, 100)
    map.setBounds(-125, 37.5, -115, 42.5)
    img_data = map.renderPNG()

    assert image_matches(img_data, f"{test}.png")


def test_remote_raster():
    test = "example-style-remote-raster"
    img_data = Map(read_style(f"{test}.json"), 256, 256).renderPNG()

    assert image_matches(img_data, f"{test}.png")


def test_remote_image_source():
    test = "example-style-remote-image-source"
    img_data = Map(read_style(f"{test}.json"), 256, 256).renderPNG()

    assert image_matches(img_data, f"{test}.png")


@pytest.mark.skipif(not MAPBOX_TOKEN, reason="MAPBOX_TOKEN not available")
def test_mapbox_source():
    test = "example-style-mapbox-source"
    img_data = Map(
        read_style(f"{test}.json"), 256, 256, token=MAPBOX_TOKEN, provider="mapbox"
    ).renderPNG()

    assert image_matches(img_data, f"{test}.png", 10)


@pytest.mark.skipif(not MAPBOX_TOKEN, reason="MAPBOX_TOKEN not available")
def test_mapbox_base_style():
    img_data = Map(
        "mapbox://styles/mapbox/streets-v11",
        256,
        256,
        1,
        token=MAPBOX_TOKEN,
        provider="mapbox",
    ).renderPNG()
    assert image_matches(img_data, "mapbox-streets-v11.png", 100)


def test_mapbox_style_missing_token():
    with pytest.raises(ValueError, match="provider 'mapbox' requires a token"):
        Map("mapbox://styles/mapbox/streets-v11", provider="mapbox")


def test_bad_remote_style_url():
    with pytest.raises(RuntimeError, match="HTTP status code 404"):
        Map("https://google.com/bogus_style").renderPNG()


@pytest.mark.skipif(not has_poorconn, reason="poorconn test lib not available")
@pytest.mark.poorconn_http_server_config(port=8111, t=1, length=10)
def test_slow_remote_style_url(poorconn_http_server, tmp_path):
    with pytest.raises(RuntimeError, match="request timed out"):
        Map(
            "http://localhost:8111/bogus_style",
        ).load()
        # NOTE: this emits errors from poorconn


def test_labels():
    test = "example-style-geojson-labels"

    map = Map(read_style(f"{test}.json"), 256, 256, 1)
    map.setBounds(-125, 37.5, -115, 42.5)
    img_data = map.renderPNG()

    assert image_matches(img_data, f"{test}.png")


def test_local_mbtiles_raster_source():
    test = "example-style-mbtiles-raster-source"
    style = read_style(f"{test}.json")

    # update style from relative to absolute path
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    img_data = Map(style, 256, 256).renderPNG()

    assert image_matches(img_data, f"{test}.png")


def test_local_mbtiles_vector_source():
    test = "example-style-mbtiles-vector-source"
    style = read_style(f"{test}.json")

    # update style from relative to absolute path
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    img_data = Map(style, 256, 256).renderPNG()

    assert image_matches(img_data, f"{test}.png", 100)


def test_local_mbtiles_vector_source_2x():
    test = "example-style-mbtiles-vector-source"
    style = read_style(f"{test}.json")

    # update style from relative to absolute path
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    img_data = Map(style, 256, 256, 2).renderPNG()

    # New Metal renderer differs by a bit more than standard tolerance
    assert image_matches(img_data, f"{test}@2x.png", 250)


def test_invalid_local_mbtiles_raster_source():
    test = "example-style-mbtiles-raster-source"
    style = read_style(f"{test}.json")

    style = style.replace("mbtiles://", "mbtiles:///invalid/")

    with pytest.raises(RuntimeError, match="path not found"):
        _ = Map(style, 256, 256).renderPNG()


def test_image_pattern():
    test = "example-style-image-pattern"
    style = read_style(f"{test}.json")

    map = Map(style, 256, 256, 1)
    map.setBounds(-125, 37.5, -115, 42.5)

    img = Image.open(FIXTURES_PATH / "example-pattern.png")
    map.addImage("pattern", img.tobytes(), img.size[0], img.size[1], 1, False)

    img_data = map.renderPNG()

    assert image_matches(img_data, f"{test}.png", tolerance=350)


def test_bad_source():
    test = "example-style-bad-source"
    style = read_style(f"{test}.json")

    map = Map(style, 100, 100, 1)
    map.setBounds(-79.98, 32.64, -79.84, 32.79)
    img_data = map.renderPNG()

    assert image_matches(img_data, f"{test}.png")


def test_bad_glyphs():
    # NOTE: this returns different error messages on MacOS vs Linux
    with pytest.raises(RuntimeError):
        map = Map(read_style("example-style-bad-glyphs.json"), 100, 100, 1)
        map.setBounds(-125, 37.5, -115, 42.5)
        map.renderPNG()


@pytest.mark.parametrize(
    "style,error_type,match",
    [
        ("invalid style", ValueError, "style is not valid"),
        ("{invalid style", RuntimeError, "Failed to parse style"),
        ("{invalid style}", RuntimeError, "Failed to parse style"),
        ("{[]}", RuntimeError, "Failed to parse style"),
        # also for short style IDs
        ("streets-v11", ValueError, "style is not valid"),
    ],
)
def test_invalid_style(style, error_type, match):
    with pytest.raises(error_type, match=match):
        Map(style)


# NOTE: these will emit warnings to stderr but are not capturable from python
@pytest.mark.parametrize(
    "style",
    [
        """{"version":0}""",
        """{"sources": []}""",
    ],
)
def test_style_parse_warnings(style):
    Map(style)


@pytest.mark.skipif(not has_poorconn, reason="poorconn test lib not available")
@pytest.mark.poorconn_http_server_config(port=8111, t=1, length=10)
def test_slow_tile_server(poorconn_http_server, tmp_path, capsys):
    # write fake tile
    tile_path = tmp_path / "0/0"
    os.makedirs(tile_path)
    (tile_path / "0").write_bytes(b"h" * 1024)

    with pytest.raises(RuntimeError, match="request timed out"):
        Map(read_style("example-style-bad-remote.json")).renderPNG()
        # NOTE: this emits errors from poorconn


def test_layer_visibility():
    test = "example-style-geojson-hidden-box"
    map = Map(read_style(f"{test}.json"), 100, 100)
    map.setBounds(-125, 37.5, -115, 42.5)

    map.setVisibility("box", False)
    assert not map.getVisibility("box")

    map.setVisibility("box2", True)
    assert map.getVisibility("box2")

    img_data = map.renderPNG()

    assert image_matches(img_data, f"{test}.png")


def test_layer_filter():
    test = "example-style-geojson-filter"
    map = Map(read_style(f"{test}.json"), 100, 100)
    map.setBounds(-125, 37.5, -115, 42.5)

    assert map.getFilter("box") is None

    map.setFilter("box", """["==", "id", 2]""")
    assert map.getFilter("box") == """["==", "id", 2]"""

    img_data = map.renderPNG()

    assert image_matches(img_data, f"{test}.png")


def test_add_background_layer():
    test = "add-background-layer"

    map = Map("", 100, 100, 1)

    map.addLayer(
        json.dumps(
            {
                "id": "background",
                "type": "background",
                "paint": {"background-color": "#0000FF"},
            }
        )
    )

    img_data = map.renderPNG()

    assert image_matches(img_data, f"{test}.png")


def test_add_geojson_point_layer():
    test = "add-geojson-point-layer"

    map = Map("", 100, 100, 1, 0, 0, 0)

    map.addSource(
        "geojson",
        json.dumps(
            {"type": "geojson", "data": {"type": "Point", "coordinates": [0, 0]}}
        ),
    )

    map.addLayer(
        json.dumps(
            {
                "id": "geojson-point",
                "source": "geojson",
                "type": "circle",
                "paint": {"circle-radius": 10, "circle-color": "red"},
            }
        )
    )

    img_data = map.renderPNG()

    assert image_matches(img_data, f"{test}.png")
