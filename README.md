# PyMGL: Maplibre GL Native Static Renderer for Python

This package provides an interface to `mapblibre-gl-native` to render Mapbox GL
styles to PNG images.

WARNING: this package is under active development and the API may change without notice.

## Goals

This package is intended to provide a lightweight interface to `maplibre-gl-native`
for rendering Mapbox GL to PNG image data using Python. This is particularly useful for
server-side rendering of maps for use in reports.

This package provides only the Python API for interacting with `maplibre-gl-native`;
it does not provide higher-level functionality such as a web server or a CLI.

For a stand-alone service implmenting rendering functionality, see
[mbgl-renderer](https://github.com/consbio/mbgl-renderer) (implemented in NodeJS).

## Install

Right now, this package must be built manually (below).

### Supported operating systems

-   MacOS 10.15+ (Arm64 / M1 support unknown)
-   Linux Debian Bullseye and similar; Alpine Linux is not supported

Windows is not and will not be supported.

## Usage

To create a map object, you must always provide a Mapbox GL style JSON string or
URL to a well-known style hosted by Mapbox or Maptiler:

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
```

You can retrieve these values using attributes, if needed:

```Python
map.size  # (width, height)

map.center  # (longitude, latitude)

map.zoom

map.bearing

map.pitch
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

WARNING: providing a URI to tiles under the `tiles` key of a source is NOT currently supported by Maplibre GL Native;
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

### Unsupported features

PyMGL does not support alternative projections or 3D terrain.

## Developing

### PyBind11

PyBind11 is used to provide bindings for Python against a C++ class that wraps
`maplibre-gl-native` for easier rendering operations.

It is included here as a git submodule, per the [installation instructions](https://pybind11.readthedocs.io/en/stable/installing.html).

```bash
git submodule add -b stable https://github.com/pybind/pybind11.git vendor/pybind11
```

### Maplibre-gl-native

Maplibre-gl-native is included as a git submodule, and it includes many submodules
of its own.

```bash
git submodule add -b master https://github.com/maplibre/maplibre-gl-native vendor/maplibre-gl-native
```

### Git submodules

Run

```bash
git submodule update --init
```

We only need some of the submodules under maplibre-gl-native. In particular,
we do not need `maplibre-gl-js` or Android / IOS dependencies.

Run the following:

```bash
cd vendor/maplibre-gl-native

git submodule update --init --recursive \
    vendor/earcut.hpp \
    vendor/polylabel \
    vendor/protozero \
    vendor/wagyu \
    vendor/unique_resource \
    vendor/boost \
    vendor/eternal \
    vendor/googletest \
    vendor/mapbox-base

```

### Architecture

This package is composed of 2 main parts:

-   wrapper around Maplibre GL native classes to make constructing and managing
    properties of the map easier
-   Python bindings created using pybind11 against that wrapper

The wrapper is located in `src/map.cpp`.

### Build

#### C++ tests

See [tests/README](tests/README.md) for more information.

Building using CMake directly is useful when building the C++ level tests of the
Maplibre GL wrapper.

```bash
mkdir build

cd build

cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug

ctest -V
```

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

## See also

[mbgl-renderer](https://github.com/consbio/mbgl-renderer)
provides a NodeJS API, CLI, and server based on the NodeJS bindings to Mapbox GL Native.

## Credits

This project was developed with the support of the
[U.S. Fish and Wildlife Service]https://www.fws.gov/)
[Southeast Conservation Adaptation Strategy](https://secassoutheast.org/) for
use in the
[Southeast Conservation Blueprint Viewer](https://blueprint.geoplatform.gov/southeast/)
and
[South Atlantic Conservation Blueprint Simple Viewer](https://blueprint.geoplatform.gov/southatlantic/).

This project is made possible because of the
[mapbox-gl-native](https://github.com/mapbox/mapbox-gl-native/) project by
[Mapbox](https://www.mapbox.com/)
by the efforts of the Maplibre community maintaining the open-source fork of that
project at [maplibre-gl-native](https://github.com/mapbox/mapbox-gl-native/).
