from PIL import Image
import pytest

from pymgl import Map

from .common import FIXTURES_PATH, MAPBOX_TOKEN, read_style, image_matches


def test_empty_style(empty_style):
    img_data = Map(empty_style, 10, 10).render()

    assert image_matches(img_data, "example-style-empty.png")


def test_geojson():
    test = "example-style-geojson"
    map = Map(read_style(f"{test}.json"), 100, 100)
    map.setBounds(-125, 37.5, -115, 42.5)
    img_data = map.render()

    assert image_matches(img_data, f"{test}.png")


def test_file_geojson():
    test = "example-style-file-geojson"
    style = read_style(f"{test}.json")

    # update style from relative to absolute path
    style = style.replace("file://", f"file://{FIXTURES_PATH}/")

    map = Map(style, 100, 100)
    map.setBounds(-125, 37.5, -115, 42.5)
    img_data = map.render()

    assert image_matches(img_data, f"{test}.png")


def test_remote_raster():
    test = "example-style-remote-raster"
    img_data = Map(read_style(f"{test}.json"), 256, 256).render()

    assert image_matches(img_data, f"{test}.png")


def test_remote_image_source():
    test = "example-style-remote-image-source"
    img_data = Map(read_style(f"{test}.json"), 256, 256).render()

    assert image_matches(img_data, f"{test}.png")


def test_mapbox_source():
    test = "example-style-mapbox-source"
    img_data = Map(
        read_style(f"{test}.json"), 256, 256, token=MAPBOX_TOKEN, provider="mapbox"
    ).render()

    assert image_matches(img_data, f"{test}.png")


def test_labels():
    test = "example-style-geojson-labels"

    map = Map(read_style(f"{test}.json"), 256, 256, 1)
    map.setBounds(-125, 37.5, -115, 42.5)
    img_data = map.render()

    assert image_matches(img_data, f"{test}.png")


def test_local_mbtiles_raster_source():
    test = "example-style-mbtiles-raster-source"
    style = read_style(f"{test}.json")

    # update style from relative to absolute path
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    img_data = Map(style, 256, 256).render()

    assert image_matches(img_data, f"{test}.png")


def test_local_mbtiles_vector_source():
    test = "example-style-mbtiles-vector-source"
    style = read_style(f"{test}.json")

    # update style from relative to absolute path
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    img_data = Map(style, 256, 256).render()

    assert image_matches(img_data, f"{test}.png")


def test_local_mbtiles_vector_source_2x():
    test = "example-style-mbtiles-vector-source"
    style = read_style(f"{test}.json")

    # update style from relative to absolute path
    style = style.replace("mbtiles://", f"mbtiles://{FIXTURES_PATH}/")

    img_data = Map(style, 256, 256, 2).render()

    assert image_matches(img_data, f"{test}@2x.png")


def test_image_pattern():
    test = "example-style-image-pattern"
    style = read_style(f"{test}.json")

    map = Map(style, 256, 256, 1)
    map.setBounds(-125, 37.5, -115, 42.5)

    img = Image.open(FIXTURES_PATH / "example-pattern.png")
    map.addImage("pattern", img.tobytes(), img.size[0], img.size[1], 1, False)

    img_data = map.render()

    assert image_matches(img_data, f"{test}.png")


def test_bad_source():
    test = "example-style-bad-source"
    style = read_style(f"{test}.json")

    map = Map(style, 100, 100, 1)
    map.setBounds(-79.98, 32.64, -79.84, 32.79)
    img_data = map.render()

    assert image_matches(img_data, f"{test}.png")


def test_bad_glyphs():
    with pytest.raises(
        RuntimeError, match="A server with the specified hostname could not be found."
    ):
        map = Map(read_style("example-style-bad-glyphs.json"), 100, 100, 1)
        map.setBounds(-125, 37.5, -115, 42.5)
        map.render()
