# Python Mapbox GL Wrapper

## Setup

Create a sibling directory structure to this project that contains the mapbox sources:
`../lib/mapbox/...`

Into this folder download or clone:

-   [mapbox gl native](https://github.com/mapbox/mapbox-gl-native)
-   [mapbox geometry headers](https://github.com/mapbox/geometry.hpp)
-   [mapbox variant headers](https://github.com/mapbox/variant)

## Library include notes

Note: there is a `mbgl/util/includes` directory but do not include it, it causes conflicts with `std::experimental::optional` definitions.

## Notes:

see example renderer at: /usr/local/bin/mbgl-render
