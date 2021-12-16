from io import BytesIO

from PIL import Image
import pytest

from pymbgl import Map


def test_style(geojson_style):
    map = Map(geojson_style)
    assert map


def test_missing_style():
    with pytest.raises(ValueError, match="style is not valid"):
        Map("")


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


def test_set_bearing_invalid(geojson_style):
    map = Map(geojson_style)
    with pytest.raises(ValueError, match="bearing must be at least 0"):
        map.setBearing(-1)

    with pytest.raises(ValueError, match="bearing must be no greater than 360"):
        map.setBearing(361)


def test_set_bounds(geojson_style):
    map = Map(geojson_style)
    map.setBounds(-100, -20, 100, 20)
    map.setBounds(-100, -20, 100, 20, 0)
    map.setBounds(-100, -20, 100, 20, 20)


def test_set_center(geojson_style):
    map = Map(geojson_style)
    map.setCenter(20, 10)
    map.setCenter(longitude=20, latitude=10)
    map.setCenter(latitude=10, longitude=20)


def test_set_pitch(geojson_style):
    map = Map(geojson_style)
    map.setPitch(20)


def test_set_pitch_invalid(geojson_style):
    map = Map(geojson_style)

    with pytest.raises(ValueError, match="pitch must be at least 0"):
        map.setPitch(-1)

    with pytest.raises(ValueError, match="pitch must be no greater than 85"):
        map.setPitch(90)


@pytest.mark.parametrize(
    "provider",
    [
        None,
        "",
        "mapbox",
        "maptiler",
        "maplibre",
    ],
)
def test_provider(geojson_style, provider):
    Map(geojson_style, provider=provider)


def test_invalid_provider(geojson_style):
    with pytest.raises(ValueError, match="invalid provider: foo"):
        Map(geojson_style, provider="foo")


def test_render(geojson_style):
    map = Map(geojson_style, 240, 200, 1, -120, 40, 4)
    img_data = map.render()

    buffer = BytesIO(img_data)
    img = Image.open(buffer)
    assert img.size == (240, 200)
