# CHANGELOG

## 0.4.0 (6/12/2023)

### Improvements

-   migrated bindings from `pybind11` to `nanobind` for smaller, faster bindings (#11)
-   upgraded `maplibre-gl-native` to `02c196a`
-   added several methods to describe layer and map style state (#12):
    -   added `getFilter` to retrieve a layer's filter and and
        `setFilter` to set it
    -   added `getVisibility` to retrieve a layer's visibility and
        `setVisibility` to set it
    -   added `getLayerJSON` to serialize a layer's properties to JSON
    -   added `listLayers` to list layers in the map's style, and `listSources`
        to list sources
-   added `addSource` to add a source to the map and `addLayer` to add a layer (#14)
-   added `setGeoJSON` to set GeoJSON data for a GeoJSON source in the map (#14)
-   allow constructing map with an empty style `Map("")` in order to allow adding
    sources and layers after construction (#14)
-   add support for getting and setting paint property of a layer:
    `getPaintProperty`, `setPaintProperty` (#16)
-   add support for getting, setting, and removing feature state (#17)

### Breaking changes

-   raise exceptions on certain types of invalid styles passed in as JSON (#13);
    not all invalid styles are detected, and remote styles are not checked for
    errors at map initialization due to other issues (see #15). Formerly, these
    errors were not easily available from the style parsing steps of
    `maplibre-gl-native`.

## 0.3.1 (1/10/2023)

### Improvements

-   updated Ubuntu wheels to use native platform name instead of custom platform
    tag for easier installation

## 0.3 (1/6/2023)

### Improvements

-   upgraded `maplibre-gl-native`
-   added wheels for Ubuntu 22.04 (WARNING: these used a custom platform tag,
    avoid using these)

## 0.2.2 (5/30/2022)

### Bug fixes

-   upgraded `maplibre-gl-native` to avoid an abort when tilesets are not found, and
    log as a runtime exception instead

## 0.2.1 (4/19/2022)

### Improvements

-   upgraded `maplibre-gl-native`

## 0.2 (4/5/2022)

Initial release of PyMGL with wheels available for installation (limited platforms).
