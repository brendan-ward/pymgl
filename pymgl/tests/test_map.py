import pytest
import numpy as np

from pymgl import Map

from .common import MAPBOX_TOKEN


def test_default_map(empty_style):
    map = Map(empty_style)
    assert map

    # verify default values
    assert map.size == (256, 256)
    assert map.center == (0, 0)
    assert map.bearing == 0
    assert map.pitch == 0
    assert map.zoom == 0


def test_map_delete(empty_style):
    map = Map(empty_style)
    map.renderPNG()

    # ensure that manually deleting map allows us to reassign; otherwise
    # the following segfaults
    del map

    # this should not segfault
    map = Map(empty_style)
    map.renderPNG()


def test_render_buffer(empty_style):
    img_data = Map(empty_style, 256, 256, 1).renderBuffer()
    assert len(img_data) == 4 * 256 * 256


def test_map_context_manager(empty_style):
    with Map(empty_style) as map:
        map.renderPNG()

    # this should not segfault
    with Map(empty_style) as map:
        map.renderPNG()


def test_map_create_params(empty_style):
    assert Map(empty_style, width=500).size == (500, 256)
    assert Map(empty_style, height=500).size == (256, 500)
    assert Map(empty_style, width=500, height=250).size == (500, 250)
    assert np.allclose(Map(empty_style, latitude=10).center, (0, 10))
    assert np.allclose(Map(empty_style, longitude=10).center, (10, 0))
    assert np.allclose(Map(empty_style, longitude=20, latitude=10).center, (20, 10))
    assert Map(empty_style, zoom=2).zoom == 2


def test_invalid_size(empty_style):
    with pytest.raises(ValueError, match="width must be greater than 0"):
        Map(empty_style, width=0)

    with pytest.raises(TypeError, match="incompatible constructor arguments"):
        Map(empty_style, width=-1)

    with pytest.raises(ValueError, match="height must be greater than 0"):
        Map(empty_style, height=0)

    with pytest.raises(TypeError, match="incompatible constructor arguments"):
        Map(empty_style, height=-1)


def test_set_bearing(empty_style):
    map = Map(empty_style)
    map.setBearing(20)
    assert map.bearing == 20


def test_set_bearing_invalid(empty_style):
    map = Map(empty_style)
    with pytest.raises(ValueError, match="bearing must be at least 0"):
        map.setBearing(-1)

    with pytest.raises(ValueError, match="bearing must be no greater than 360"):
        map.setBearing(361)


def test_set_bounds(empty_style):
    map = Map(empty_style)
    map.setBounds(0, 0, 50, 60)
    assert np.allclose(map.center, (25, 35.26438968275954))

    map.setBounds(0, 0, 50, 60, padding=0)
    assert np.allclose(map.center, (25, 35.26438968275954))

    map.setBounds(0, 0, 50, 60, 20)
    assert np.allclose(map.center, (25, 35.26438968275954))


def test_set_center(empty_style):
    map = Map(empty_style)
    map.setCenter(20, 10)
    assert np.allclose(map.center, (20, 10))

    map.setCenter(longitude=25, latitude=15)
    assert np.allclose(map.center, (25, 15))

    map.setCenter(latitude=5, longitude=30)
    assert np.allclose(map.center, (30, 5))


def test_set_pitch(empty_style):
    map = Map(empty_style)
    map.setPitch(20)

    assert map.pitch == 20


def test_set_pitch_invalid(empty_style):
    map = Map(empty_style)

    with pytest.raises(ValueError, match="pitch must be at least 0"):
        map.setPitch(-1)

    with pytest.raises(ValueError, match="pitch must be no greater than 85"):
        map.setPitch(90)


def test_invalid_provider(empty_style):
    with pytest.raises(ValueError, match="invalid provider: foo"):
        Map(empty_style, provider="foo")
