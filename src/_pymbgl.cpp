#include <iostream>
#include <optional>

#include <mbgl/gfx/headless_frontend.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/map/map_observer.hpp>
#include <mbgl/map/map_options.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace pybind11::literals;

// TODO: connect this to output to raise errors for Python
// struct PyMapObserver : public mbgl::MapObserver {
//     void onDidFailLoadingMap(mbgl::MapLoadError, const std::string &) final;
// };

// TODO: (from node_map)
// mbgl::FileSourceManager::get()->registerFileSourceFactory(
//     mbgl::FileSourceType::ResourceLoader, [](const mbgl::ResourceOptions& resourceOptions) {
//         return std::make_unique<node_mbgl::NodeFileSource>(
//             reinterpret_cast<node_mbgl::NodeMap*>(resourceOptions.platformContext()));
//     });

class PyMap {
public:
    PyMap(const uint32_t width,
          const uint32_t height,
          const float ratio,
          std::optional<std::string> token)
        // TODO: : mapObserver(PyMapObserver())
        : ratio(ratio)
        , frontend(std::make_unique<mbgl::HeadlessFrontend>(mbgl::Size{width, height}, ratio))
        , map(std::make_unique<mbgl::Map>(*frontend,
                                          // TODO: mapObserver,
                                          mbgl::MapObserver::nullObserver(),
                                          mbgl::MapOptions()
                                              .withMapMode(mbgl::MapMode::Static)
                                              .withSize(frontend->getSize())
                                              .withPixelRatio(ratio),

                                          // TODO: cache and assets dir
                                          mbgl::ResourceOptions().withAccessToken(
                                              token.has_value() ? token.value() : std::string())

                                              )) {

        std::cout << "token: " << (token.has_value() ? token.value() : "not set ") << std::endl;

        // TODO: can pass in style as URL or JSON string
        // map->getStyle().loadURL(str) or map->getStyle().loadJSON(str)


    }

    // ~PyMap() {} // TODO: ?

    void Render() {
        std::cout << "ratio " << ratio << " dims: " << frontend->getSize().width << ","
                  << frontend->getSize().height << std::endl;

        // TODO: (from render.cpp)
            // map.jumpTo(mbgl::CameraOptions()
            //        .withCenter(mbgl::LatLng { lat, lon })
            //        .withZoom(zoom)
            //        .withBearing(bearing)
            //        .withPitch(pitch));


    } // TODO: return array
    void Cancel() { } // TODO:
    void Release() { } // TODO:

private:
    std::unique_ptr<mbgl::HeadlessFrontend> frontend;
    std::unique_ptr<mbgl::Map> map;
    // PyMapObserver mapObserver;

    std::exception_ptr error;
    // mbgl::PremultipliedImage image;
    // std::unique_ptr<RenderRequest> req;

    // pixel ratio
    float ratio = 1;

    bool loaded = false;
};

int add(int i, int j) { return i + j; }

PYBIND11_MODULE(_pymbgl, m) {
    py::class_<PyMap>(m, "Map")
        .def(py::init<const uint32_t, const uint32_t, const float, std::optional<std::string>>(),
             "width"_a  = 256,
             "height"_a = 256,
             "ratio"_a  = 1,
             // see std::optional allow None as input here:
             // https://github.com/pybind/pybind11/issues/1953
             "token"_a = py::none())
        .def("render", &PyMap::Render)

        ;
}

// TODO: remove
    //   static PyMap New(float ratio) {
    //     // TODO: options: request, ratio
    //     // TODO: file source manager

    //     std::cout << "Pixel ratio is " << ratio;

    //     return PyMap(ratio);
    //   }