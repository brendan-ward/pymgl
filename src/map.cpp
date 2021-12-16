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

    // Turn off logging
    mbgl::Log::setObserver(std::make_unique<mbgl::Log::NullObserver>());

    // TODO: remove
    std::cout << "ratio " << ratio.value_or(1) << " dims: " << frontend->getSize().width << ","
              << frontend->getSize().height << std::endl;
    std::cout << "token: " << (token.has_value() ? "set" : "not set ") << std::endl;

    // determine tile server options from provider
    // TODO: verify this works properly
    mbgl::TileServerOptions tileServerOptions = mbgl::TileServerOptions();

    if (provider.has_value() && !provider.value().empty()) {
        if (provider.value().find("mapbox") != -1) {
            tileServerOptions = mbgl::TileServerOptions::MapboxConfiguration();
            std::cout << "Set mapbox provider" << std::endl;
        } else if (provider.value().find("maptiler") != -1) {
            tileServerOptions = mbgl::TileServerOptions::MapTilerConfiguration();
            std::cout << "Set maptiler provider" << std::endl;
        } else if (provider.value().find("maplibre") != -1) {
            tileServerOptions = mbgl::TileServerOptions::MapLibreConfiguration();
            std::cout << "Set maplibre provider" << std::endl;
        } else {
            throw std::invalid_argument("invalid provider: " + provider.value());
        }
        // TODO: custom ?
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

    // create loop but immediately stop it (doesn't need to run)
    mbgl::util::RunLoop loop;
    loop.stop();

    if (style.find("{") == 0) {
        // assume json
        std::cout << "loading style JSON" << std::endl;
        map->getStyle().loadJSON(style);
        has_style = true;
    } else if (style.find("://") != -1) {
        // assume URL, like mapbox://
        std::cout << "loading style URL: " << style << std::endl;
        map->getStyle().loadURL(style);
        has_style = true;
    } else if (!style.empty()) {
        // assume simple style ID, construct URL
        std::string url
            = mbgl::util::mapbox::normalizeStyleURL(tileServerOptions, style, token.value_or(""));
        std::cout << "loading style URL: " << url << std::endl;
        map->getStyle().loadURL(url);
        has_style = true;
    } else {
        throw std::invalid_argument("style is not valid");
    }

    std::cout << "Init config, zoom: " << zoom.value_or(0) << " center: " << longitude.value_or(0)
              << ", " << latitude.value_or(0) << std::endl;

    map->jumpTo(mbgl::CameraOptions()
                    .withCenter(mbgl::LatLng{latitude.value_or(0), longitude.value_or(0)})
                    .withZoom(zoom.value_or(0))
                    .withBearing(0)
                    .withPitch(0));

    std::cout << "Map created" << std::endl;
}

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
    // check via map
    if (map->getStyle().getSources().empty() || map->getStyle().getLayers().empty()) {
        throw std::runtime_error(std::string("CHECK style is not set"));
    }

    // style must be set or render will hang
    if (!has_style) {
        throw std::runtime_error(std::string("style is not set"));
    }

    mbgl::util::RunLoop loop;
    loop.stop();

    try {
        std::string img = encodePNG(frontend->render(*map).image);
        std::cout << "rendered successfully" << std::endl;
        return img;

    } catch (std::exception &e) {
        std::cout << "ERROR: Error rendering map " << e.what() << std::endl;
        throw e;
    }
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
