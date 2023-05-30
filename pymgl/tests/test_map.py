import json

import pytest
import numpy as np

from pymgl import Map

from .common import MAPBOX_TOKEN, read_style


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

    with pytest.raises(ValueError, match="height must be greater than 0"):
        Map(empty_style, height=0)

    with pytest.raises(TypeError, match="incompatible function arguments"):
        Map(empty_style, width=-1)

    with pytest.raises(TypeError, match="incompatible function arguments"):
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


@pytest.mark.parametrize("visibility", [True, False])
def test_layer_visibility_no_layers(empty_style, visibility):
    with pytest.raises(RuntimeError, match="any_layer is not a valid layer id in map"):
        Map(empty_style).getVisibility("any_layer")

    with pytest.raises(RuntimeError, match="any_layer is not a valid layer id in map"):
        Map(empty_style).setVisibility("any_layer", visibility)


@pytest.mark.parametrize("filter", ["""["==", "foo", "bar"]""", "", None])
def test_layer_filter_no_layers(empty_style, filter):
    with pytest.raises(RuntimeError, match="any_layer is not a valid layer id in map"):
        Map(empty_style).getFilter("any_layer")

    with pytest.raises(RuntimeError, match="any_layer is not a valid layer id in map"):
        Map(empty_style).setFilter("any_layer", filter)


def test_layer_paint_property_no_layers(empty_style):
    with pytest.raises(RuntimeError, match="any_layer is not a valid layer id in map"):
        Map(empty_style).getPaintProperty("any_layer", "fill-color")

    with pytest.raises(RuntimeError, match="any_layer is not a valid layer id in map"):
        Map(empty_style).setPaintProperty("any_layer", "fill-color", "#FF0000")


def test_layer_paint_property():
    map = Map(read_style("example-style-geojson.json"))

    map.getPaintProperty("box", "fill-opacity") == "0.5"
    map.setPaintProperty("box", "fill-opacity", "0.75")
    map.getPaintProperty("box", "fill-opacity") == "0.75"

    map.getPaintProperty("box", "fill-color") == '["rgba", 255, 0, 0, 1]'
    map.getPaintProperty("box", "fill-color") == '"#00FF00"'
    map.getPaintProperty("box", "fill-color") == '["rgba",0, 255,  0, 1]'
    map.setPaintProperty("box", "fill-color", '"rgba(0,255,0,0.5)"')
    map.getPaintProperty("box", "fill-color") == '["rgba", 0, 255, 0, 0.5]'


def test_layer_paint_property_invalid():
    map = Map(read_style("example-style-geojson.json"))
    with pytest.raises(RuntimeError, match="Invalid value"):
        map.setPaintProperty("box", "invalid_property", "invalid_value")

    with pytest.raises(RuntimeError, match="Invalid value"):
        map.setPaintProperty("box", "invalid_property", "#FF0000")


def test_layer_json_no_layers(empty_style):
    with pytest.raises(RuntimeError, match="any_layer is not a valid layer id in map"):
        Map(empty_style).getLayerJSON("any_layer")


def test_layer_json():
    map = Map(read_style("example-style-geojson.json"))
    actual = json.loads(map.getLayerJSON("box"))

    expected = {
        "source": "geojson",
        "type": "fill",
        "paint": {"fill-opacity": 0.5, "fill-color": ["rgba", 255, 0, 0, 1]},
        "id": "box",
    }

    assert actual["id"] == expected["id"]
    assert actual["source"] == expected["source"]
    assert actual["type"] == expected["type"]
    assert actual["paint"]["fill-color"] == expected["paint"]["fill-color"]
    assert actual["paint"]["fill-opacity"] == expected["paint"]["fill-opacity"]

    map.setVisibility("box", False)
    actual = json.loads(map.getLayerJSON("box"))

    assert actual["layout"] == {"visibility": "none"}

    map.setFilter("box", """["==", "id", 2]""")
    actual = json.loads(map.getLayerJSON("box"))

    assert actual["filter"] == ["==", "id", 2]


def test_list_layers():
    map = Map(read_style("example-style-geojson.json"))
    assert map.listLayers() == ["box", "box-outline"]

    map = Map(read_style("example-style-empty.json"))
    assert map.listLayers() == []


@pytest.mark.skipif(not MAPBOX_TOKEN, reason="MAPBOX_TOKEN not available")
def test_list_layers_remote_style():
    map = Map(
        "mapbox://styles/mapbox/streets-v11",
        10,
        10,
        token=MAPBOX_TOKEN,
        provider="mapbox",
    )

    # remote map must be loaded to fetch all remote assets
    map.load()

    assert len(map.listLayers()) == 111


def test_list_sources():
    map = Map(read_style("example-style-geojson.json"))
    assert map.listSources() == ["geojson"]

    map = Map(read_style("example-style-empty.json"))
    assert map.listSources() == []

    map = Map(read_style("example-style-mbtiles-vector-source.json"))
    assert map.listSources() == ["land"]


@pytest.mark.skipif(not MAPBOX_TOKEN, reason="MAPBOX_TOKEN not available")
def test_list_sources_remote_style():
    map = Map(
        "mapbox://styles/mapbox/streets-v11",
        10,
        10,
        token=MAPBOX_TOKEN,
        provider="mapbox",
    )

    # remote styles require render to load all assets
    map.renderPNG()

    assert map.listSources() == ["composite"]


def test_add_source():
    map = Map("", 10, 10)

    map.addSource(
        "geojson",
        json.dumps(
            {"type": "geojson", "data": {"type": "Point", "coordinates": [0, 0]}}
        ),
    )
    map.addSource(
        "vector_url", json.dumps({"type": "vector", "url": "mbtiles://land.mbtiles"})
    )
    map.addSource(
        "vector_tiles",
        json.dumps(
            {
                "type": "vector",
                "tiles": ["http://test/{x}/{y}/{z}.pbf"],
                "minzoom": 2,
                "maxzoom": 12,
            }
        ),
    )
    map.addSource(
        "raster_url",
        json.dumps({"type": "raster", "url": "mbtiles://geography-class-png.mbtiles"}),
    )
    map.addSource(
        "raster_tiles",
        json.dumps(
            {
                "type": "raster",
                "tiles": ["http://test/{x}/{y}/{z}.pbf"],
                "minzoom": 2,
                "maxzoom": 12,
                "tileSize": 256,
            }
        ),
    )
    map.addSource(
        "image",
        json.dumps(
            {
                "type": "image",
                "url": "https://services.arcgisonline.com/arcgis/rest/services/World_Terrain_Base/MapServer/tile/0/0/0",
                "coordinates": [[-180, 90], [180, 90], [180, -90], [-180, -90]],
            }
        ),
    )

    assert map.listSources() == [
        "geojson",
        "vector_url",
        "vector_tiles",
        "raster_url",
        "raster_tiles",
        "image",
    ]


def test_add_layer():
    map = Map("", 10, 10)

    map.addLayer(
        json.dumps(
            {
                "id": "background",
                "type": "background",
                "paint": {"background-color": "#0000FF"},
            }
        )
    )

    map.addLayer(json.dumps({"id": "circle", "source": "geojson", "type": "circle"}))

    assert map.listLayers() == ["background", "circle"]


def test_set_geojson():
    map = Map("", 10, 10)

    geoJSON = json.dumps({"type": "Point", "coordinates": [0, 0]})

    with pytest.raises(RuntimeError, match="geojson is not a valid source"):
        map.setGeoJSON("geojson", geoJSON)

    map.addSource(
        "vector_url", json.dumps({"type": "vector", "url": "mbtiles://land.mbtiles"})
    )
    with pytest.raises(RuntimeError, match="vector_url is not a GeoJSON source"):
        map.setGeoJSON("vector_url", geoJSON)

    map.addSource(
        "geojson",
        json.dumps(
            {
                "type": "geojson",
                "data": {"type": "GeometryCollection", "geometries": []},
            }
        ),
    )
    map.setGeoJSON("geojson", geoJSON)


def test_feature_state():
    map = Map(read_style("example-style-geojson-features.json"), 10, 10, 1)

    assert map.getFeatureState("geojson", "box", "0") is None

    map.load()
    assert map.getFeatureState("geojson", "box", "0") is None

    map.setFeatureState("geojson", "box", "0", '{"a": true}')
    state = map.getFeatureState("geojson", "box", "0")

    assert state is not None
    assert json.loads(state) == {"a": True}


def test_feature_state_invalid():
    map = Map(read_style("example-style-geojson-features.json"), 10, 10, 1)

    with pytest.raises(RuntimeError, match="invalid-source is not a valid source"):
        map.getFeatureState("invalid-source", "box", "0")

    with pytest.raises(RuntimeError, match="invalid-source is not a valid source"):
        map.setFeatureState("invalid-source", "box", "0", '{"a": true}')

    with pytest.raises(RuntimeError, match="invalid-source is not a valid source"):
        map.removeFeatureState("invalid-source", "box", "0", "a")

    with pytest.raises(RuntimeError, match="invalid-layer is not a valid layer"):
        map.getFeatureState("geojson", "invalid-layer", "0")

    with pytest.raises(RuntimeError, match="invalid-layer is not a valid layer"):
        map.setFeatureState("geojson", "invalid-layer", "0", '{"a": true}')

    with pytest.raises(RuntimeError, match="invalid-layer is not a valid layer"):
        map.removeFeatureState("geojson", "invalid-layer", "0", "a")
