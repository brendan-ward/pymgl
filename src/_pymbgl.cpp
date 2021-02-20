#include "_pymbgl.hpp"

#include <exception>
#include <iostream>
#include <fstream>
#include <optional>

#include <mbgl/gfx/headless_frontend.hpp>
#include <mbgl/util/image.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/map/map_observer.hpp>
#include <mbgl/map/map_options.hpp>
#include <mbgl/style/style.hpp>
#include <mbgl/util/async_request.hpp>
#include <mbgl/util/run_loop.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace pybind11::literals;

namespace py_mbgl {

void PyMapObserver::onDidFailLoadingMap(mbgl::MapLoadError error, const std::string &description) {
    std::cout << "ERROR:" << std::endl;
    // switch (error) {
    //     // case mbgl::MapLoadError::StyleParseError:
    //     //     // TODO: properly throw errors to pybind
    //     //     std::cout << "ERROR: failed to parse style: " << description.c_str() <<
    //     std::endl;
    //     //     break;
    //     default:
    //         std::cout << "ERROR: failed to load map: " << description.c_str() << std::endl;
    // }
}

// std::unique_ptr<mbgl::AsyncRequest> PyFileSource::request(const mbgl::Resource& resource,
// mbgl::FileSource::Callback callback_) {
//     assert(pyMap);

//     // Nan::HandleScope scope;
//     // Because this method may be called while this NodeMap is already eligible for garbage
//     collection,
//     // we need to explicitly hold onto our own handle here so that GC during a v8 call doesn't
//     destroy
//     // *this while we're still executing code.
//     pyMap->handle();

//     auto asyncRequest = std::make_unique<py_mbgl::NodeAsyncRequest>();

//     v8::Local<v8::Value> argv[] = {
//         Nan::New<v8::External>(nodeMap),
//         Nan::New<v8::External>(&callback_),
//         Nan::New<v8::External>(asyncRequest.get()),
//         Nan::New(resource.url).ToLocalChecked(),
//         Nan::New<v8::Integer>(resource.kind)
//     };

//     Nan::NewInstance(Nan::New(node_mbgl::NodeRequest::constructor), 5, argv).ToLocalChecked();

//     return asyncRequest;
// }

// bool PyFileSource::canRequest(const mbgl::Resource&) const {
//     return true;
// }

PyMap::PyMap(const uint32_t width,
             const uint32_t height,
             const float ratio,
             std::optional<std::string> token,
             std::optional<std::string> style_url,
             std::optional<std::string> style_json)
    : mapObserver(PyMapObserver())
    , frontend(std::make_unique<mbgl::HeadlessFrontend>(mbgl::Size{width, height}, ratio)) {

    std::cout << "ratio " << ratio << " dims: " << frontend->getSize().width << ","
              << frontend->getSize().height << std::endl;
    std::cout << "token: " << (token.has_value() ? token.value() : "not set ") << std::endl;
    std::cout << "style_url: " << (style_url.has_value() ? style_url.value() : "not set ")
              << std::endl;
    // std::cout << "style_json: " << (style_json.has_value() ? style_json.value() : "not set ")
    //           << std::endl;
    std::cout << "style_json: " << (style_json.has_value() ? "{...}" : "not set ") << std::endl;

    // TODO: how do we connect this up to the instance??
    // mbgl::FileSourceManager::get()->registerFileSourceFactory(
    //     mbgl::FileSourceType::ResourceLoader, [](const mbgl::ResourceOptions& resourceOptions) {
    //         return std::make_unique<node_mbgl::NodeFileSource>(
    //             reinterpret_cast<node_mbgl::NodeMap*>(resourceOptions.platformContext()));
    //     });

    // init map - could be above instead but we get setfaults with observer that way
    map = std::make_unique<mbgl::Map>(*frontend,
                                      mapObserver,
                                      mbgl::MapOptions()
                                          .withMapMode(mbgl::MapMode::Static)
                                          .withSize(frontend->getSize())
                                          .withPixelRatio(ratio),

                                      // TODO: cache and assets dir
                                      mbgl::ResourceOptions().withAccessToken(
                                          token.has_value() ? token.value() : std::string()));

    try {
        if (style_url.has_value()) {
            // TODO: this doesn't work, maybe because we don't have a local assets folder set, or
            // something else with a file source loader?
            // immediately segfaults

            // how do we know if the file exists?
            map->getStyle();
            std::cout << "got map style: " << map->getStyle().getName() << std::endl;
            map->getStyle().loadURL(style_url.value());
        } else if (style_json.has_value()) {
            map->getStyle().loadJSON(style_json.value());
            std::cout << "got map style: " << map->getStyle().getName() << std::endl;
        }

    } catch (std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    std::cout << "after set style" << std::endl;
}

std::string PyMap::Render(float zoom,
                   float longitude,
                   float latitude,
                   float bearing,
                   float pitch) { // TODO: return array
    std::cout << " center: " << longitude << "," << latitude << " bearing: " << bearing
              << " pitch: " << pitch << std::endl;

    // TODO: (from render.cpp)
    map->jumpTo(mbgl::CameraOptions()
                    .withCenter(mbgl::LatLng{latitude, longitude})
                    .withZoom(zoom)
                    .withBearing(bearing)
                    .withPitch(pitch));

    // adapted from render.cpp
    mbgl::util::RunLoop loop;

    try {
        std::string img = encodePNG(frontend->render(*map).image);
        std::cout << "rendered successfully" << std::endl;
        return img;

    } catch(std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return std::string(); // empty buffer
    }
}

PYBIND11_MODULE(_pymbgl, m) {
    py::class_<PyMap>(m, "Map")
        .def(py::init<const uint32_t,
                      const uint32_t,
                      const float,
                      std::optional<std::string>,
                      std::optional<std::string>,
                      std::optional<std::string>>(),
             "width"_a  = 256,
             "height"_a = 256,
             "ratio"_a  = 1,
             // see std::optional allow None as input here:
             // https://github.com/pybind/pybind11/issues/1953
             "token"_a      = py::none(),
             "style_url"_a  = py::none(),
             "style_json"_a = py::none())
        .def("render", [](PyMap& self, float zoom, float longitude, float latitude, float bearing, float pitch) -> py::bytes {
                return py::bytes(self.Render(zoom, longitude, latitude, bearing, pitch));
            },
            "Render the map to PNG data",
             "zoom"_a      = 0,
             "longitude"_a = 0,
             "latitude"_a  = 0,
             "bearing"_a   = 0,
             "pitch"_a     = 0
            )

        ;
}

} // namespace py_mbgl
