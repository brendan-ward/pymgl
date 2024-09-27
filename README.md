# PyMGL: Maplibre GL Native Static Renderer for Python

This package provides an interface to [Mapblibre Native](https://github.com/maplibre/maplibre-native)
to render Mapbox GL / Maplibre GL styles to PNG images.

WARNING: this package is under active development and the API may change without notice.

## Goals

This package is intended to provide a lightweight interface to `maplibre-native`
for rendering Mapbox GL / Maplibre GL styles to PNG image data using Python.
This is particularly useful for server-side rendering of maps for use in reports.

This package provides only the Python API for interacting with `maplibre-native`;
it does not provide higher-level functionality such as a web server or a CLI.

## Install

### Supported operating systems

#### MacOS 12+, Ubuntu 18+, Debian 10+, Fedora 29+, RHEL 8+, Alma Linux 8+

x86_64 and arm64 wheels are available on PyPI:

```bash
pip install pymgl
```

NOTE: x86_64 wheels are not currently available for MacOS.

To verify that pymgl installed correctly, install with the test dependencies and
run the included test suite:

```bash
pip install pymgl[test]
pytest --pyargs pymgl -v
```

#### Windows

Windows is not and will not be supported.

## Usage

To create a map object, you must always provide a Mapbox GL / Maplibre GL style
JSON string or URL to a well-known style hosted by Mapbox or Maptiler:

```Python
from pymgl import Map

style = """{
    "version": 8,
    "sources": {
        "basemap": {
            "type": "raster",
            "tiles": ["https://services.arcgisonline.com/arcgis/rest/services/Ocean/World_Ocean_Base/MapServer/tile/{z}/{y}/{x}"],
            "tileSize": 256
        }
    },
    "layers": [
        { "id": "basemap", "source": "basemap", "type": "raster" }
    ]
}"""

map = Map(style, <height=256>, <width=256>, <ratio=1>, <longitude=0>, <latitude=0>, <zoom=0>, <token=None>, <provider=None>)
```

See the [styles](#styles) section for more information about map styles.

Other than style, all other parameters are optional with default values.

NOTE: `style` and `ratio` cannot be changed once the instance is constructed.

You can use a well-known style instead of providing a style JSON string, but
you must also provide a token and identify the correct provider:

```Python
map = Map("mapbox://styles/mapbox/streets-v11", token=<mapbox token>, provider="mapbox")
```

Valid providers are `mapbox`, `maptiler`, and `maplibre`.

### Map properties

You can set additional properties on the map instance after it is created:

```Python
map.setCenter(longitude, latitude)

map.setZoom(zoom)

map.setSize(width, height)

map.setBearing(bearing)  # map bearing in degrees

map.setPitch(pitch)  # map pitch in degrees

map.setFilter(layerId, filterJSON or None)

map.setPaintProperty(layerId, property, value)

map.setVisibility(layerId, True / False)
```

You can retrieve these values using attributes, if needed:

```Python
map.size  # (width, height)

map.center  # (longitude, latitude)

map.zoom

map.bearing

map.pitch
```

You can also retrive information about the map's style or a specific layer:

```Python
map.listLayers()  # [<layerId1>, ...]

map.listSources()  # [<sourceId1>, ...]

map.getFilter(<layerId>)  # returns JSON value or None

map.getPaintProperty(<layerId>, <property>)  # returns JSON value or None

map.getLayerJSON(<layerId>)  # returns JSON describing layer
```

NOTE: paint properties may be decoded to their internal representation. For
example, a CSS color string `#FF0000` will be returned as `["rgba", 255, 0, 0, 1]`.

IMPORTANT: if you are using a remotely-hosted style, you need to force the map
to load - which loads all underying assets - before listing the style's layers,
sources, or other properties.

```Python
map = Map("mapbox://styles/mapbox/streets-v11", token=<mapbox token>, provider="mapbox")

map.listLayers()  # []
map.load()
map.listLayers()  # [<layerId1>, ...]
```

Alternatively, you can download the style yourself and provide that as input to
the Map, and it will show all layers without requiring a render first. However, not all assets will be loaded until the first render.

```Python
from urllib.request import urlopen

url = f"https://api.mapbox.com/styles/v1/mapbox/streets-v11?access_token={MAPBOX_TOKEN}"

with urlopen(url) as r:
    style = r.read()

map = Map(style.decode("UTF-8") token=<mapbox token>, provider="mapbox")
map.listLayers()  # [<layerId1>, ...]
```

You can auto-fit the map to bounds instead of using center longitude / lantitude
and zoom:

```Python
map.setBounds(xmin, ymin, xmax, ymax, <padding=0>)
```

You can register an image for use with your style by providing an ID,
raw image bytes, width, height, pixel ratio, and indicate if it should be
interpreted as SDF:

```
map.addImage("id", img_bytes, width, height, <ratio=1>, <make_SDF=False>)
```

See the [SDF image docs](https://docs.mapbox.com/help/troubleshooting/using-recolorable-images-in-mapbox-maps/) for more information about using SDF
images.

### Rendering

You can render the map to PNG bytes:

```Python
img_bytes = map.renderPNG()
```

This returns `bytes` containing the RGBA PNG data.

You can render the map to a raw buffer as a numpy array (`uint8` dtype):

```Python
array = map.renderBuffer()
```

The array is a sequence of RGBA values for each pixel in the image.

This may be useful if you are going to immediately read the image data into
another package such as `Pillow` or `pyvips` to combine with other image
operations.

### Map instances

WARNING: you must manually delete the map instance if you assign a new map
instance to that variable, or this package will segfault (not yet sure why).
This problem does not occur if separate instances are assigned to separate
variables.

```Python

map = Map(<style>, <width>, <height>)

del map  # must manually delete BEFORE creating a new instance assigned to this

map = Map(<style>, <width>, <height>)
```

For this reason, you should consider using a context manager:

```Python
with Map(<style>, <width>, <height>) as map:
    map.renderPNG()
```

You can also use the map instance to directly render to PNG, if you don't need
to set other properties on the map instance:

```
Map(<style>, <width>, <height>).renderPNG()
```

## Styles

PyMGL should support basic styles as of Mapbox GL JS 1.13.

### Remote tilesets, sources, and assets

Remote tilesets, tile sources, and assets (glyphs, sprites) should be well-supported.
These are loaded by the underlying C++ library outside our control. Invalid
URLs will generally raise errors. However, network timeouts or incorrect formats
may cause the process to crash.

### Local mbtiles

Local MBTiles are supported, but must be provided using an absolute path to the
mbtiles file as the source `url` of a tileset; it must resolve to an actual file.

Local MBTiles are denoted with a `mbtiles://` URI prefix.

Example:

```json
{
    "sources": {
        "source_id": {
            "url": "mbtiles:///<pymgl_root_dir>/tests/fixtures/geography-class-png.mbtiles",
            ...
        }
    },
    "layers": [...],
    ...
}
```

### Local files

GeoJSON files and other local file assets are supported, but must be provided
using an absolute path to the file source.

Example:

```json
{
    "sources": {
        "geojson": {
            "type": "geojson",
            "data": "file:///<pymgl_root_dir>/tests/fixtures/test.geojson"
        }
    },
    "layers": [...],
    ...
}

```

WARNING: providing a URI to tiles under the `tiles` key of a source is NOT currently supported by Maplibre Native;
attempting to do so will fail.

### Images

You must register the image with the map instance before rendering the map. See
`map.addImage()` above.

```json
{
    "sources": {...},
    "layers": [
        {
            ...,
            "paint": {
                "fill-pattern": "pattern"
            }
        },
    ]
}
```

You can use map images as fill patterns or icon images.

### Adding sources and layers after construction

You can add sources and layers dynamically after constructing the map instance:

```Python
import json

map = Map("")  # construct with empty style

map.addSource("my_id", json.dumps({
    "type": "geojson",
    "data": {"type": "Point", "coordinates": [0, 0]}
}))

map.addLayer(json.dumps({
    "id": "geojson-point",
    "source": "geojson",
    "type": "circle",
    "paint": { ... }
}))
```

### Feature state

You can get, set, and remove feature state after the map has been loaded.

```Python

map = Map(<style with source "exampleSource" and layer "exampleLayer">, ...)

map.load()
map.getFeatureState("exampleSource", "exampleLayer", "0")  # returns None
map.setFeatureState("exampleSource", "exampleLayer", "0", "{\"a\": true}")
map.getFeatureState("exampleSource", "exampleLayer", "0")  # returns "{\"a\": true}"

# remove the state value for key "a"
map.removeFeatureState("exampleSource", "exampleLayer", "0", "a")
map.render()
map.getFeatureState("exampleSource", "exampleLayer", "0")  # returns None
```

NOTE: features must already have a unique, numeric ID set on each feature. There
is currently no support for `promoteId` like in MapLibre GL JS.

IMPORTANT: the map must be loaded before getting or setting feature state. You
must manually force a render in order for the map to update feature state after
removing a state key

### Unsupported features

PyMGL does not support alternative projections or 3D terrain.

## Developing

### Dependencies:

#### MacOS:

Developing on MacOS requires the following binary libraries to be installed
via `homebrew`:

-   cmake
-   ninja

#### Developing on Ubuntu requires the following binary libraries:

-   cmake
-   ninja-build
-   build-essential
-   libcurl4-openssl-dev
-   libicu-dev
-   libpng-dev
-   libwebp-dev
-   libprotobuf-dev
-   libjpeg-turbo8-dev
-   libx11-dev
-   libegl-dev
-   libopengl-dev
-   xvfb

To run on Linux, XVFB must also be running; otherwise the process will segfault.

See [`docker/README.md`](./docker/README.md) for more information.

### nanobind

`nanonbind` is used to provide bindings for Python against a C++ class that wraps
`maplibre-native` for easier rendering operations.

It is included here as a git submodule, per the
[installation instructions]().

```bash
git submodule add https://github.com/wjakob/nanobind vendor/nanobind
cd vendor/nanobind
git submodule update --init --recursive
```

Then to upgrade to a specific version of nanobind for development, if needed:

```bash
cd vendor/nanobind
git checkout <version tag>
```

### Maplibre Native

Maplibre Native is included as a git submodule, and it includes many submodules
of its own.

```bash
git submodule add -b main https://github.com/maplibre/maplibre-native vendor/maplibre-native
```

### Git submodules

Run

```bash
git submodule update --init
```

We only need some of the submodules under maplibre-native. In particular,
we do not need `maplibre-gl-js` or Android / IOS dependencies.

Run the following:

```bash
cd vendor/maplibre-native

git submodule update --init --recursive \
    vendor/boost \
    vendor/cpp-httplib \
    vendor/earcut.hpp \
    vendor/eternal \
    vendor/googletest \
    vendor/metal-cpp \
    vendor/polylabel \
    vendor/protozero \
    vendor/mapbox-base \
    vendor/unique_resource \
    vendor/unordered_dense \
    vendor/vector-tile \
    vendor/wagyu \
    vendor/zip-archive
```

To later update `maplibre-native`:

```bash
cd vendor/maplibre-native
git checkout main
git pull origin

cd ../..
git commit -am "update maplibre-native" to latest
```

### Architecture

This package is composed of 2 main parts:

-   wrapper around Maplibre Native classes to make constructing and managing
    properties of the map easier
-   Python bindings created using nanobind against that wrapper

The wrapper is located in `src/map.cpp`.

### Build

#### C++ tests

See [tests/README](tests/README.md) for more information.

#### Build Python extension

The Python `setup.py` script manages building the library and extension using
CMake.

From project root directory:

```bash
python setup.py build_ext --inplace
```

#### Docstrings / type information

Docstrings are maintained in both `src/_pymgl.cpp` and `pymgl/__init__.pyi`.

Python-friendly type annotations are maintained in `pymgl/__init__.pyi`.

Note: `pymgl/__init__.pyi` is necessary to support autocompletion and tooltips
in VSCode.

##### Building wheels

Most wheels are automatically built by Github when pushing a new version tag.
Linux Arm64 wheels must be built locally on an Arm64 machine (e.g., MacOS host).

These are created using the manylinux_2_28 Docker container.

```bash
docker build -f docker/Dockerfile.manylinux_2_28_aarch64 -t pymgl-manylinux_2_28_aarch64 .
docker run -v "$PWD/:/app" pymgl-manylinux_2_28_aarch64 ci/build_linux_wheels.sh
```

This will create aarch64 wheels in `dist` that can be uploaded directly to PyPI.

## See also

[mbgl-renderer](https://github.com/consbio/mbgl-renderer)
provides a NodeJS API, CLI, and server based on the NodeJS bindings to Mapbox GL Native.

## Credits

This project was developed with the support of the
[U.S. Fish and Wildlife Service](https://www.fws.gov/)
[Southeast Conservation Adaptation Strategy](https://secassoutheast.org/) for
use in the
[Southeast Conservation Blueprint Viewer](https://blueprint.geoplatform.gov/southeast/).

This project is made possible because of the
[mapbox-gl-native](https://github.com/mapbox/mapbox-gl-native/) project by
[Mapbox](https://www.mapbox.com/)
by the efforts of the Maplibre community maintaining the open-source fork of that
project at [maplibre-native](https://github.com/mapbox/mapbox-native/).
