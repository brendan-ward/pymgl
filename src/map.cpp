#include <csignal>
#include <exception>
#include <fstream>
#include <iostream>
#include <optional>

#include <mbgl/gfx/headless_frontend.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/map/map_observer.hpp>
#include <mbgl/map/map_options.hpp>
#include <mbgl/storage/file_source_manager.hpp>
#include <mbgl/storage/network_status.hpp>
#include <mbgl/style/style.hpp>
#include <mbgl/util/exception.hpp>
#include <mbgl/util/image.hpp>
#include <mbgl/util/logging.hpp>
#include <mbgl/util/mapbox.hpp>
#include <mbgl/util/run_loop.hpp>

#include "map.hpp"

namespace py_mbgl {

Map::Map(const std::string &style,
         const std::optional<uint32_t> &width,
         const std::optional<uint32_t> &height,
         const std::optional<float> &ratio,
         const std::optional<double> &longitude,
         const std::optional<double> &latitude,
         const std::optional<double> &zoom,
         const std::optional<std::string> &token,
         const std::optional<std::string> &provider)
    : frontend(std::make_unique<mbgl::HeadlessFrontend>(
        mbgl::Size{width.value_or(256), height.value_or(256)}, ratio.value_or(1))) {

    // immediately stop loop (doesn't need to run)
    loop.stop();

    // Turn off logging
    mbgl::Log::setObserver(std::make_unique<mbgl::Log::NullObserver>());

    // determine tile server options from provider
    mbgl::TileServerOptions tileServerOptions = mbgl::TileServerOptions();

    if (provider.has_value() && !provider.value().empty()) {
        if (provider.value().find("mapbox") != -1) {
            tileServerOptions = mbgl::TileServerOptions::MapboxConfiguration();
        } else if (provider.value().find("maptiler") != -1) {
            tileServerOptions = mbgl::TileServerOptions::MapTilerConfiguration();
        } else if (provider.value().find("maplibre") != -1) {
            tileServerOptions = mbgl::TileServerOptions::MapLibreConfiguration();
        } else {
            throw std::invalid_argument("invalid provider: " + provider.value());
        }
        // TODO: custom ?
    }

    if (tileServerOptions.requiresApiKey() && (!token.has_value() || token.value().empty())) {
        throw std::invalid_argument("provider '" + provider.value_or("") + "' requires a token");
    }

    // validate parameters
    if (width.has_value()) {
        validateDimension(width.value(), "width");
    }
    if (height.has_value()) {
        validateDimension(height.value(), "height");
    }
    if (ratio.has_value()) {
        validatePixelRatio(ratio.value());
    }
    if (zoom.has_value()) {
        validateZoom(zoom.value());
    }

    // create resource options
    mbgl::ResourceOptions resourceOptions;
    if (token.has_value()) {
        resourceOptions.withApiKey(token.value());
    }

    map = std::make_unique<mbgl::Map>(*frontend,
                                      mbgl::MapObserver::nullObserver(),
                                      mbgl::MapOptions()
                                          .withMapMode(mbgl::MapMode::Static)
                                          .withSize(frontend->getSize())
                                          .withPixelRatio(ratio.value_or(1)),
                                      resourceOptions.withTileServerOptions(tileServerOptions));

    if (style.find("{") == 0) {
        // assume json
        map->getStyle().loadJSON(style);
    } else if (style.find("://") != -1) {
        // otherwise must be URL-like reference, like "mapbox://styles/mapbox/streets-v11"
        map->getStyle().loadURL(style);
    } else {
        throw std::invalid_argument("style is not valid");
    }

    map->jumpTo(mbgl::CameraOptions()
                    .withCenter(mbgl::LatLng{latitude.value_or(0), longitude.value_or(0)})
                    .withZoom(zoom.value_or(0))
                    .withBearing(0)
                    .withPitch(0));
}

const double Map::getBearing() { return map->getCameraOptions().bearing.value_or(0); }

const std::pair<double, double> Map::getCenter() {
    mbgl::LatLng center = map->getCameraOptions().center.value_or(mbgl::LatLng(0, 0));
    return std::pair<double, double>(center.longitude(), center.latitude());
}

const double Map::getPitch() { return map->getCameraOptions().pitch.value_or(0); }

const std::pair<uint32_t, uint32_t> Map::getSize() {
    return std::pair<uint32_t, uint32_t>(frontend->getSize().width, frontend->getSize().height);
}

const double Map::getZoom() { return map->getCameraOptions().zoom.value_or(0); }

void Map::setBearing(const double &bearing) {
    validateBearing(bearing);
    map->jumpTo(mbgl::CameraOptions().withBearing(bearing));
}

void Map::setCenter(const double &longitude, const double &latitude) {
    map->jumpTo(mbgl::CameraOptions().withCenter(mbgl::LatLng{latitude, longitude}));
}

void Map::setBounds(const double &xmin,
                    const double &ymin,
                    const double &xmax,
                    const double &ymax,
                    const double &padding) {
    map->jumpTo(map->cameraForLatLngBounds(
        mbgl::LatLngBounds::hull(mbgl::LatLng{ymin, xmin}, mbgl::LatLng{ymax, xmax}),
        {padding, padding, padding, padding},
        {},
        {}));
}

void Map::setPitch(const double &pitch) {
    validatePitch(pitch);
    map->jumpTo(mbgl::CameraOptions().withPitch(pitch));
}

void Map::setSize(const uint32_t &width, const uint32_t &height) {
    validateDimension(width, "width");
    validateDimension(height, "height");
    frontend->setSize(mbgl::Size{width, height});
    map->setSize(mbgl::Size{width, height});
}

void Map::setZoom(const double &zoom) {
    validateZoom(zoom);
    map->jumpTo(mbgl::CameraOptions().withZoom(zoom));
}

std::string Map::render() {
    std::string img = encodePNG(frontend->render(*map).image);
    return img;
}

// private:

void Map::validateBearing(const double &bearing) {
    if (bearing < 0) {
        throw std::domain_error("bearing must be at least 0");
    }
    if (bearing > 360) {
        throw std::domain_error("bearing must be no greater than 360");
    }
}

void Map::validateDimension(const uint32_t &value, const std::string dimType) {
    if (value <= 0) {
        throw std::domain_error(dimType + " must be greater than 0");
    }
}

void Map::validatePitch(const double &pitch) {
    if (pitch < 0) {
        throw std::domain_error("pitch must be at least 0");
    }
    // match mapbox gl JS
    if (pitch > 85) {
        throw std::domain_error("pitch must be no greater than 85");
    }
}

void Map::validatePixelRatio(const float &ratio) {
    if (ratio <= 0) {
        throw std::domain_error("ratio must be greater than 0");
    }
    // arbitrary cutoff
    if (ratio > 85) {
        throw std::domain_error("ratio must be no greater than 8");
    }
}

void Map::validateZoom(const double &zoom) {
    if (zoom < 0) {
        throw std::domain_error("zoom must be greater than 0");
    }
    // match mapbox gl JS
    if (zoom > 24) {
        throw std::domain_error("zoom must be no greater than 24");
    }
}

} // namespace py_mbgl
