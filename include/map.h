#pragma once

#include <iomanip>
#include <optional>
#include <ostream>

#include <mbgl/gfx/headless_frontend.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/util/run_loop.hpp>

namespace mgl_wrapper {

// adapted from mbgl/test/stub_map_observer.hpp to implement only those callbacks
// that are used below
class MapObserver : public mbgl::MapObserver {
public:
    void onDidFinishLoadingStyle() final {
        if (didFinishLoadingStyleCallback) {
            didFinishLoadingStyleCallback();
        }
    }

    void onDidFailLoadingMap(mbgl::MapLoadError type, const std::string &description) final {
        if (didFailLoadingMapCallback) {
            didFailLoadingMapCallback(type, description);
        }
    }

    std::function<void()> didFinishLoadingStyleCallback;
    std::function<void(mbgl::MapLoadError, const std::string &)> didFailLoadingMapCallback;
};

class Map {
public:
    Map(const std::string &style,
        const std::optional<uint32_t> &width       = {},
        const std::optional<uint32_t> &height      = {},
        const std::optional<float> &ratio          = {},
        const std::optional<double> &longitude     = {},
        const std::optional<double> &latitude      = {},
        const std::optional<double> &zoom          = {},
        const std::optional<std::string> &token    = {},
        const std::optional<std::string> &provider = {});

    // Underlying constructs do not support easy copy, so prevent them here
    Map(const Map &) = delete;
    ~Map();

    void render();
    const std::string renderPNG();
    const std::unique_ptr<uint8_t[]> renderBuffer();

    const double getBearing();
    const std::pair<double, double> getCenter();
    const std::optional<std::string> getFeatureState(const std::string &sourceID,
                                                     const std::string &layerID,
                                                     const std::string &featureID);
    const std::optional<std::string> getLayerFilter(const std::string &id);
    const std::optional<std::string> getLayerPaintProperty(const std::string &id,
                                                           const std::string &property);
    const std::optional<std::string> getLayerJSON(const std::string &id);
    const bool getLayerVisibility(const std::string &id);
    const double getPitch();
    const std::pair<uint32_t, uint32_t> getSize();
    const double getZoom();

    const std::vector<std::string> listLayers();
    const std::vector<std::string> listSources();

    void load();

    void addImage(const std::string &name,
                  const std::string &image,
                  uint32_t width,
                  uint32_t height,
                  float ratio,
                  bool make_sdf);

    void addSource(const std::string &id, const std::string &options);
    void addLayer(const std::string &options);

    void removeFeatureState(const std::string &sourceID,
                            const std::string &layerID,
                            const std::string &featureID,
                            const std::string &stateKey);

    void setBearing(const double &bearing);
    void setCenter(const double &longitude, const double &latitude);
    void setBounds(const double &xmin,
                   const double &ymin,
                   const double &xmax,
                   const double &ymax,
                   const double &padding = 0);
    void setGeoJSON(const std::string &sourceID, const std::string &geoJSON);
    void setFeatureState(const std::string &sourceID,
                         const std::string &layerID,
                         const std::string &featureID,
                         const std::string &state);
    void setLayerFilter(const std::string &id, const std::optional<std::string> &expression);
    void setLayerPaintProperty(const std::string &id,
                               const std::string &property,
                               const std::string &value);
    void setLayerVisibility(const std::string &id, bool visible);
    void setPitch(const double &pitch);
    void setZoom(const double &zoom);
    void setSize(const uint32_t &width, const uint32_t &height);

    void release();

    friend std::ostream &operator<<(std::ostream &os, Map &m);

private:
    std::unique_ptr<mbgl::HeadlessFrontend> frontend;
    std::unique_ptr<mbgl::Map> map;

    // std::unique_ptr<LogObserver> logObserver;
    std::unique_ptr<MapObserver> observer;

    // loop must be defined on the instance or we get segfaults, but we don't
    // need to stop it (stopping works fine on MacOS, but causes things to hang
    // on Linux)
    std::unique_ptr<mbgl::util::RunLoop> loop;

    void validateBearing(const double &bearing);
    void validateDimension(const uint32_t &value, const std::string dimType);
    void validatePitch(const double &pitch);
    void validatePixelRatio(const float &ratio);
    void validateZoom(const double &zoom);
};

} // namespace mgl_wrapper