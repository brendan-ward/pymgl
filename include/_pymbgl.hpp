#pragma once

#include <exception>
#include <iostream>
#include <optional>

#include <mbgl/gfx/headless_frontend.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/map/map_observer.hpp>
#include <mbgl/map/map_options.hpp>
#include <mbgl/style/style.hpp>
#include <mbgl/storage/file_source.hpp>
#include <mbgl/util/async_request.hpp>
#include <mbgl/util/image.hpp>

#include <pybind11/stl.h>



namespace py_mbgl {

struct PyMapObserver : public mbgl::MapObserver {
    void onDidFailLoadingMap(mbgl::MapLoadError, const std::string &) final;
};

// struct PyFileSource : public mbgl::FileSource {
//     PyFileSource(PyMap* pyMap_) : pyMap(pyMap_) {}
//     ~PyFileSource() override = default;
//     std::unique_ptr<mbgl::AsyncRequest> request(const mbgl::Resource&, mbgl::FileSource::Callback) final;
//     bool canRequest(const mbgl::Resource&) const override;
//     PyMap* pyMap;
// };


// class RenderRequest;


class PyMap {
public:
    PyMap(const uint32_t width,
          const uint32_t height,
          const float ratio,
          std::optional<std::string> token,
          std::optional<std::string> style_url,
          std::optional<std::string> style_json);

    // ~PyMap(); // TODO: ?

    std::string Render(float zoom, float latitude, float longitude, float bearing, float pitch);     // TODO: return array
    void Cancel();
    void Release();

private:
    std::unique_ptr<mbgl::HeadlessFrontend> frontend;
    std::unique_ptr<mbgl::Map> map;
    PyMapObserver mapObserver;

    std::exception_ptr error;

    // don't need these unless we do something async
    // mbgl::PremultipliedImage image;
    // std::unique_ptr<RenderRequest> req;

    // Async for delivering the notifications of render completion.
    // uv_async_t *async;

};

} // namespace py_mbgl