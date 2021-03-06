#pragma once

#include <iomanip>
#include <optional>
#include <ostream>

#include <mbgl/gfx/headless_frontend.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/util/run_loop.hpp>

namespace mgl_wrapper {

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

    const std::string renderPNG();
    const std::unique_ptr<uint8_t[]> renderBuffer();

    const double getBearing();
    const std::pair<double, double> getCenter();
    const double getPitch();
    const std::pair<uint32_t, uint32_t> getSize();
    const double getZoom();

    void addImage(const std::string &name,
                  const std::string &image,
                  uint32_t width,
                  uint32_t height,
                  float ratio,
                  bool make_sdf);

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

    void release();

    friend std::ostream &operator<<(std::ostream &os, Map &m);

private:
    std::unique_ptr<mbgl::HeadlessFrontend> frontend;
    std::unique_ptr<mbgl::Map> map;

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