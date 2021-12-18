from io import BytesIO
import os

from dotenv import load_dotenv
from PIL import Image
import pytest
import numpy as np

from pymbgl import Map


load_dotenv()
MAPBOX_TOKEN = os.getenv("MAPBOX_TOKEN", None)


def test_default_map(geojson_style):
    map = Map(geojson_style)
    assert map

    # verify default values
    assert map.size == (256, 256)
    assert map.center == (0, 0)
    assert map.bearing == 0
    assert map.pitch == 0
    assert map.zoom == 0


def test_map_create_params(geojson_style):
    assert Map(geojson_style, width=500).size == (500, 256)
    assert Map(geojson_style, height=500).size == (256, 500)
    assert Map(geojson_style, width=500, height=250).size == (500, 250)
    assert np.allclose(Map(geojson_style, latitude=10).center, (0, 10))
    assert np.allclose(Map(geojson_style, longitude=10).center, (10, 0))
    assert np.allclose(Map(geojson_style, longitude=20, latitude=10).center, (20, 10))
    assert Map(geojson_style, zoom=2).zoom == 2


@pytest.mark.skipif(
    MAPBOX_TOKEN is None, reason="MAPBOX_TOKEN env variable is not defined"
)
def test_mapbox_style():
    Map("mapbox://styles/mapbox/streets-v11", provider="mapbox", token=MAPBOX_TOKEN)


def test_mapbox_style_missing_token():
    with pytest.raises(ValueError, match="provider 'mapbox' requires a token"):
        Map("mapbox://styles/mapbox/streets-v11", provider="mapbox")


def test_missing_style():
    with pytest.raises(ValueError, match="style is not valid"):
        Map("")


def test_invalid_style():
    with pytest.raises(ValueError, match="style is not valid"):
        Map("foo")

    # also for short mapbox style IDs
    with pytest.raises(ValueError, match="style is not valid"):
        Map("streets-v11")


def test_invalid_size(geojson_style):
    with pytest.raises(ValueError, match="width must be greater than 0"):
        Map(geojson_style, width=0)

    with pytest.raises(TypeError, match="incompatible constructor arguments"):
        Map(geojson_style, width=-1)

    with pytest.raises(ValueError, match="height must be greater than 0"):
        Map(geojson_style, height=0)

    with pytest.raises(TypeError, match="incompatible constructor arguments"):
        Map(geojson_style, height=-1)


def test_set_bearing(geojson_style):
    map = Map(geojson_style)
    map.setBearing(20)
    assert map.bearing == 20


def test_set_bearing_invalid(geojson_style):
    map = Map(geojson_style)
    with pytest.raises(ValueError, match="bearing must be at least 0"):
        map.setBearing(-1)

    with pytest.raises(ValueError, match="bearing must be no greater than 360"):
        map.setBearing(361)


def test_set_bounds(geojson_style):
    map = Map(geojson_style)
    map.setBounds(0, 0, 50, 60)
    assert np.allclose(map.center, (25, 35.26438968275954))

    map.setBounds(0, 0, 50, 60, padding=0)
    assert np.allclose(map.center, (25, 35.26438968275954))

    map.setBounds(0, 0, 50, 60, 20)
    assert np.allclose(map.center, (25, 35.26438968275954))


def test_set_center(geojson_style):
    map = Map(geojson_style)
    map.setCenter(20, 10)
    assert np.allclose(map.center, (20, 10))

    map.setCenter(longitude=25, latitude=15)
    assert np.allclose(map.center, (25, 15))

    map.setCenter(latitude=5, longitude=30)
    assert np.allclose(map.center, (30, 5))


def test_set_pitch(geojson_style):
    map = Map(geojson_style)
    map.setPitch(20)

    assert map.pitch == 20


def test_set_pitch_invalid(geojson_style):
    map = Map(geojson_style)

    with pytest.raises(ValueError, match="pitch must be at least 0"):
        map.setPitch(-1)

    with pytest.raises(ValueError, match="pitch must be no greater than 85"):
        map.setPitch(90)


def test_invalid_provider(geojson_style):
    with pytest.raises(ValueError, match="invalid provider: foo"):
        Map(geojson_style, provider="foo")


def test_render(geojson_style):
    map = Map(geojson_style, 240, 200, 1, -120, 40, 4)
    img_data = map.render()

    buffer = BytesIO(img_data)
    img = Image.open(buffer)
    assert img.size == (240, 200)
