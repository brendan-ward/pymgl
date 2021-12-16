# Python Mapbox GL Wrapper

## Setup

### New

Add submodules:

```bash

git submodule add -b master https://github.com/mapbox/mapbox-gl-native.git vendor/mapbox-gl-native

git submodule add -b stable https://github.com/pybind/pybind11.git vendor/pybind11
```

Edit the `.gitmodules` in `mapbox-gl-native` to set branch for
`mapbox-gl-js` and `mapbox-core` to `main` instead of `master.

Init submodules:
```bash
git submodule init
git submodule update --init --recursive --remote # FIXME: maybe handled automatically by mapbox-gl-native make?
```


FIXME: this pulls in a ton of stuff we likely don't need to build mapbox-gl-native;
can we just pull in the stuff we need?  Specifically don't need to build tests or GLFW
backend.

Set options:
https://stackoverflow.com/questions/20239334/cmake-set-subdirectory-options/20239564

Use externalproject?
https://cmake.org/cmake/help/v2.8.11/cmake.html#module%3aExternalProject

See mapbox-gl-native/platform/macos/macos.cmake for example

### Coding notes

see `platform/glfw/main.cpp` for examples of setting options
















TODO: not used
build mapbox-gl-core
```bash
cd vendor/mapbox-gl-native
cmake . -B build -G Ninja
```



build:
```
mkdir build && cd build
cmake .. -G Ninja

```









Create a sibling directory structure to this project that contains the mapbox sources:
`../lib/mapbox/...`

Into this folder download or clone:

-   [mapbox gl native](https://github.com/mapbox/mapbox-gl-native)
-   [mapbox geometry headers](https://github.com/mapbox/geometry.hpp)
-   [mapbox variant headers](https://github.com/mapbox/variant)

### Build Mapbox GL

in Mapbox GL directory:

```
git submodule update --init --recursive
cmake . -B build
cmake --build build --verbose

```

## Library include notes

Note: there is a `mbgl/util/includes` directory but do not include it, it causes conflicts with `std::experimental::optional` definitions.

## Notes:

see example renderer at: /usr/local/bin/mbgl-render

Based on the verbose mode, look at what things get linked in by the link step.

On MacOS, this includes the following for the `render-test` library:

```
g++ -O3 -DNDEBUG -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk -Wl,-search_paths_first -Wl,-headerpad_max_install_names  CMakeFiles/mbgl-render-test-runner.dir/platform/default/src/mbgl/render-test/main.cpp.o  -o mbgl-render-test-runner render-test/libmbgl-render-test.a libmbgl-core.a libmbgl-vendor-csscolorparser.a libmbgl-vendor-parsedate.a /Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/OpenGL.framework/OpenGL.tbd -framework AppKit -framework CoreGraphics -framework CoreLocation -framework SystemConfiguration libmbgl-vendor-icu.a -lsqlite3 -lz
```

To discover linking errors, run the link command emitted by Cython and replace `dynamic_lookup` with `error`:

```
g++ -bundle -undefined error -L/Users/bcward/opt/miniconda3/envs/sarp_species/lib -arch x86_64 -L/Users/bcward/opt/miniconda3/envs/sarp_species/lib -arch x86_64 -framework AppKit -framework CoreGraphics -framework CoreLocation -framework SystemConfiguration -arch x86_64 build/temp.macosx-10.9-x86_64-3.8/pymbgl/_map.o -L/Users/bcward/projects/lib/mapbox/mapbox-gl-native/build -lmbgl-core -lmbgl-vendor-icu -lmbgl-vendor-parsedate -lmbgl-vendor-csscolorparser -lsqlite3 -lz -o build/lib.macosx-10.9-x86_64-3.8/pymbgl/_map.cpython-38-darwin.so
```
