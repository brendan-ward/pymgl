# CHANGELOG

## 0.4 (in progress)

### Improvements

-   migrated bindings from `pybind11` to `nanobind` for smaller, faster bindings (#11)
-   upgraded `maplibre-gl-native` to `02c196a`

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
