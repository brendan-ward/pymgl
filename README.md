# Maplibre GL Native Static Renderer for Python

This package provides an interface to `mapblibre-gl-native` to render Mapbox GL
styles to PNG images.

WARNING: this package is under active development and the API may change without notice.

## Goals

This package is intended to provide a lightweight interface to `maplibre-gl-native`
for rendering Mapbox GL to PNG image data. This is particularly useful for
server-side rendering of maps for use in reports.

## Install

Right now, this package must be built manually (below).

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

git submodule update --init --recursive --remote \
    vendor/earcut.hpp \
    vendor/polylabel \
    vendor/protozero \
    vendor/wagyu \
    vendor/unique_resource \
    vendor/boost \
    vendor/eternal

git submodule update --init --recursive vendor/mapbox-base
```

### Build

#### Build tests using CMake / Ninja

```bash
mkdir build

cd build

cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug

```

#### Build Python extension

from project root directory:

```bash

python setup.py build_ext --inplace
```

## See also

[https://github.com/consbio/mbgl-renderer](https://github.com/consbio/mbgl-renderer)
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
