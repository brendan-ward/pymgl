#include <exception>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include <zlib.h>

#include <mbgl/map/map_observer.hpp>
#include <mbgl/map/map_options.hpp>
#include <mbgl/style/conversion/filter.hpp>
#include <mbgl/style/conversion/json.hpp>
#include <mbgl/style/style.hpp>
#include <mbgl/util/image.hpp>
#include <mbgl/util/mapbox.hpp>
#include <mbgl/util/premultiply.hpp>

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "map.h"
#include "spng.h"

namespace mgl_wrapper {

// helper function to parse filter expr from JSON string to Expression
mbgl::style::Filter parseFilter(const std::string &expression) {
    mbgl::style::conversion::Error error;
    return *mbgl::style::conversion::convertJSON<mbgl::style::Filter>(expression, error);
}

// helper function to write JSON from mbgl::Value
// adapted from mbgl-native-gl::/expression-test/expression_test_parser.cpp
void writeJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer,
               const mbgl::Value &value) {
    value.match([&](const mbgl::NullValue &) { writer.Null(); },
                [&](bool b) { writer.Bool(b); },
                [&](uint64_t u) { writer.Uint64(u); },
                [&](int64_t i) { writer.Int64(i); },
                [&](double d) {
                    d == std::floor(d) ? writer.Int64(static_cast<int64_t>(d)) : writer.Double(d);
                },
                [&](const std::string &s) { writer.String(s); },
                [&](const std::vector<mbgl::Value> &arr) {
                    writer.StartArray();
                    for (const auto &item : arr) {
                        writeJSON(writer, item);
                    }
                    writer.EndArray();
                },
                [&](const std::unordered_map<std::string, mbgl::Value> &obj) {
                    writer.StartObject();
                    for (const auto &entry : obj) {
                        writer.Key(entry.first.c_str());
                        writeJSON(writer, entry.second);
                    }
                    writer.EndObject();
                });
}

Map::Map(const std::string &style,
         const std::optional<uint32_t> &width,
         const std::optional<uint32_t> &height,
         const std::optional<float> &ratio,
         const std::optional<double> &longitude,
         const std::optional<double> &latitude,
         const std::optional<double> &zoom,
         const std::optional<std::string> &token,
         const std::optional<std::string> &provider) {

    // loop must be created before frontend
    loop     = std::make_unique<mbgl::util::RunLoop>();
    frontend = std::make_unique<mbgl::HeadlessFrontend>(
        mbgl::Size{width.value_or(256), height.value_or(256)}, ratio.value_or(1));
    observer = std::make_unique<MapObserver>();

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
                                      *observer,
                                      mbgl::MapOptions()
                                          .withMapMode(mbgl::MapMode::Static)
                                          .withSize(frontend->getSize())
                                          .withPixelRatio(ratio.value_or(1)),
                                      resourceOptions.withTileServerOptions(tileServerOptions));

    if (style.find("{") == 0) {
        observer->didFailLoadingMapCallback
            = [&](mbgl::MapLoadError type, const std::string &description) {
                  throw std::runtime_error(description);
              };

        // assume content is json
        map->getStyle().loadJSON(style);
    } else if (style.find("://") != -1) {
        // otherwise must be URL-like reference, like "mapbox://styles/mapbox/streets-v11"

        map->getStyle().loadURL(style);

        // run the looop until the style is loaded
        observer->didFinishLoadingStyleCallback = [&]() { loop->stop(); };

        observer->didFailLoadingMapCallback
            = [&](mbgl::MapLoadError type, const std::string &description) {
                  loop->stop();
                  throw std::runtime_error(description);
              };

        loop->run();
    } else {
        throw std::invalid_argument("style is not valid");
    }

    map->jumpTo(mbgl::CameraOptions()
                    .withCenter(mbgl::LatLng{latitude.value_or(0), longitude.value_or(0)})
                    .withZoom(zoom.value_or(0))
                    .withBearing(0)
                    .withPitch(0));
}

Map::~Map() {
    if (map) {
        release();
    }
}

void Map::addImage(const std::string &name,
                   const std::string &image,
                   uint32_t width,
                   uint32_t height,
                   float ratio   = 1.0,
                   bool make_sdf = false) {

    if (width > 1024 || height > 1024) {
        throw std::invalid_argument("width and height must be less than 1024");
    }

    if (image.length() != width * height * 4) {
        throw std::invalid_argument("length of image bytes must be width * height * 4");
    }

    // Construct premultiplied image from string
    mbgl::UnassociatedImage cImage(
        {width, height}, reinterpret_cast<const uint8_t *>(image.c_str()), image.length());
    mbgl::PremultipliedImage cPremultipliedImage = mbgl::util::premultiply(std::move(cImage));

    map->getStyle().addImage(std::make_unique<mbgl::style::Image>(
        name, std::move(cPremultipliedImage), ratio, make_sdf));
}

const double Map::getBearing() { return std::abs(map->getCameraOptions().bearing.value_or(0)); }

const std::pair<double, double> Map::getCenter() {
    mbgl::LatLng center = map->getCameraOptions().center.value_or(mbgl::LatLng(0, 0));
    return std::pair<double, double>(center.longitude(), center.latitude());
}

const std::optional<std::string> Map::getLayerFilter(const std::string &id) {
    auto layer = map->getStyle().getLayer(id);
    if (layer == nullptr) {
        throw std::runtime_error(id + " is not a valid layer id in map");
    }
    auto filter = layer->getFilter();
    if (!filter) {
        return std::optional<std::string>();
    }

    // adapted from expression_test_parser.cpp
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
    writeJSON(writer, filter.serialize());

    return buffer.GetString();
}

const std::optional<std::string> Map::getLayerJSON(const std::string &id) {
    auto layer = map->getStyle().getLayer(id);
    if (layer == nullptr) {
        throw std::runtime_error(id + " is not a valid layer id in map");
    }

    // adapted from expression_test_parser.cpp
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
    writer.SetIndent(' ', 2);
    writeJSON(writer, layer->serialize());

    return buffer.GetString();
}

const bool Map::getLayerVisibility(const std::string &id) {
    auto layer = map->getStyle().getLayer(id);
    if (layer == nullptr) {
        throw std::runtime_error(id + " is not a valid layer id in map");
    }
    return layer->getVisibility() == mbgl::style::VisibilityType::Visible;
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

const std::vector<std::string> Map::listLayers() {
    auto layers = map->getStyle().getLayers();

    std::vector<std::string> layerIds;
    layerIds.reserve(layers.size());

    for (auto &layer : layers) {
        auto layerId = layer->getID();

        // ignore builtin layer
        if (layerId == "com.mapbox.annotations.points") {
            continue;
        }

        layerIds.push_back(layerId);
    }
    return layerIds;
}

const std::vector<std::string> Map::listSources() {
    auto sources = map->getStyle().getSources();

    std::vector<std::string> sourceIds;
    sourceIds.reserve(sources.size());

    for (auto &source : sources) {
        auto sourceId = source->getID();

        // ignore builtin source
        if (sourceId == "com.mapbox.annotations") {
            continue;
        }

        sourceIds.push_back(sourceId);
    }
    return sourceIds;
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

void Map::setLayerFilter(const std::string &id, const std::optional<std::string> &expression) {
    auto layer = map->getStyle().getLayer(id);
    if (layer == nullptr) {
        throw std::runtime_error(id + " is not a valid layer id in map");
    }

    if (!expression.has_value() || expression.value().empty()) {
        layer->setFilter(mbgl::style::Filter());
    } else {
        auto filter = parseFilter(expression.value());
        layer->setFilter(filter);
    }
}

void Map::setLayerVisibility(const std::string &id, bool visible) {
    auto layer = map->getStyle().getLayer(id);
    if (layer == nullptr) {
        throw std::runtime_error(id + " is not a valid layer id in map");
    }
    layer->setVisibility(visible ? mbgl::style::VisibilityType::Visible
                                 : mbgl::style::VisibilityType::None);
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

const std::string Map::renderPNG() {
    // render produces premultiplied image; unpremultiply it
    auto image = mbgl::util::unpremultiply(frontend->render(*map).image);

    struct spng_ihdr ihdr = {0};
    ihdr.width            = image.size.width;
    ihdr.height           = image.size.height;
    ihdr.bit_depth        = 8;
    ihdr.color_type       = SPNG_COLOR_TYPE_TRUECOLOR_ALPHA;

    spng_ctx *ctx = spng_ctx_new(SPNG_CTX_ENCODER);
    spng_set_ihdr(ctx, &ihdr);
    spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);
    spng_set_option(ctx, SPNG_FILTER_CHOICE, SPNG_FILTER_CHOICE_NONE);
    spng_set_option(ctx, SPNG_IMG_COMPRESSION_LEVEL, 3);

    int ret = spng_encode_image(ctx,
                                static_cast<const void *>(image.data.get()),
                                image.bytes(),
                                SPNG_FMT_PNG,
                                SPNG_ENCODE_FINALIZE);

    if (ret) {
        spng_ctx_free(ctx);
        throw std::runtime_error("could not encode image, error: "
                                 + std::string(spng_strerror(ret)));
    }

    size_t png_size;
    auto buf = static_cast<unsigned char *>(spng_get_png_buffer(ctx, &png_size, &ret));

    if (buf == NULL) {
        spng_ctx_free(ctx);
        throw std::runtime_error("could not get encoded image, error: "
                                 + std::string(spng_strerror(ret)));
    }

    std::string out = std::string(buf, buf + png_size);

    free(buf);
    spng_ctx_free(ctx);

    return out;
}

const std::unique_ptr<uint8_t[]> Map::renderBuffer() {
    // render produces premultiplied image; unpremultiply it
    auto image = mbgl::util::unpremultiply(frontend->render(*map).image);

    return std::move(image.data);
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
    // match mapbox gl JS max zoom
    if (zoom > 24) {
        throw std::domain_error("zoom must be no greater than 24");
    }
}

void Map::release() {
    if (!map) {
        return;
    }
    map.reset();
    frontend.reset();
    loop.reset();
}

std::ostream &operator<<(std::ostream &os, Map &m) {
    os << std::setprecision(2) << "Map(size: (" << m.getSize().first << ", " << m.getSize().second
       << "), center: (" << m.getCenter().first << ", " << m.getCenter().second
       << "), zoom: " << m.getZoom() << ", bearing: " << m.getBearing()
       << ", pitch: " << m.getPitch() << ")";
    return os;
}

} // namespace mgl_wrapper