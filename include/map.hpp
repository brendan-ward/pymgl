#pragma once

#include <exception>
#include <iostream>
#include <optional>

#include <mbgl/gfx/headless_frontend.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/map/map_observer.hpp>
#include <mbgl/map/map_options.hpp>
#include <mbgl/storage/file_source.hpp>
#include <mbgl/style/style.hpp>
#include <mbgl/util/async_request.hpp>
#include <mbgl/util/image.hpp>
#include <mbgl/util/run_loop.hpp>

namespace py_mbgl {

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
    ~Map()           = default;

    std::string render();

    const double getBearing();
    const std::pair<double, double> getCenter();
    const double getPitch();
    const std::pair<uint32_t, uint32_t> getSize();
    const double getZoom();

    void setBearing(const double &bearing);
    void setCenter(const double &longitude, const double &latitude);
    void setBounds(const double &xmin,
                   const double &ymin,
                   const double &xmax,
                   const double &ymax,
                   const double &padding = 0);
    void setPitch(const double &pitch);
    void setZoom(const double &zoom);
    void setSize(const uint32_t &width, const uint32_t &height);

private:
    std::unique_ptr<mbgl::HeadlessFrontend> frontend;
    std::unique_ptr<mbgl::Map> map;
    mbgl::util::RunLoop loop;

    void validateBearing(const double &bearing);
    void validateDimension(const uint32_t &value, const std::string dimType);
    void validatePitch(const double &pitch);
    void validatePixelRatio(const float &ratio);
    void validateZoom(const double &zoom);
};

} // namespace py_mbgl