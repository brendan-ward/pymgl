#include <iostream>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

#include "map.h"

namespace py = pybind11;
using namespace pybind11::literals;
using namespace mgl_wrapper;

PYBIND11_MODULE(_pymgl, m) {
    m.doc() = "MapLibre GL native static renderer";

    py::class_<Map>(m, "Map")
        .def(py::init<const std::string &,
                      const std::optional<uint32_t> &,
                      const std::optional<uint32_t> &,
                      const std::optional<float> &,
                      const std::optional<double> &,
                      const std::optional<double> &,
                      const std::optional<double> &,
                      const std::optional<std::string> &,
                      const std::optional<std::string> &>(),
             R"pbdoc(
            Create Maplibre GL map instance.

            Parameters
            ----------
            style : str
                Mapbox GL style object as json-encoded string.
            width : int, optional (default: 256)
                Width of output map.
            height : int, optional (default: 256)
                Height of output map.
            ratio : float, optional (default: 1)
                Pixel ratio of output map.
            longitude : float, optional (default: 0)
                Map center longitude.
            latitude : float, optional (default: 0)
                Map center latitude.
            zoom : float, optional (default 0)
                Map zoom level, between 0 and 24.
            token : str, optional
                Token, if required for provider.
            provider : str, one of {'mapbox', 'maptiler', 'maplibre', None}
                Map resource provider, if required for sources listed in the style.
        )pbdoc",
             py::arg("style"),
             py::arg("width")     = 256,
             py::arg("height")    = 256,
             py::arg("ratio")     = 1,
             py::arg("longitude") = 0,
             py::arg("latitude")  = 0,
             py::arg("zoom")      = 0,
             py::arg("token")     = py::none(),
             py::arg("provider")  = py::none())
        .def("__str__",
             [](Map &self) {
                 std::ostringstream os;
                 os << "pymgl." << self;
                 return os.str();
             })
        .def("__repr__",
             [](Map &self) {
                 std::ostringstream os;
                 os << "pymgl." << self;
                 return os.str();
             })
        .def("addImage",
             &Map::addImage,
             R"pbdoc(
            Add an image to the map to be used as an icon or pattern.

            Parameters
            ----------
            name : str
                Name or ID of the image.  Used when referencing
                this image in the style.
            image : bytes
                Raw pixels of image data.  PNG and other images must
                be decoded to raw pixel values.
            width : int
                Width of image.
            height : int
                Height of image.
            ratio : float
                Pixel ratio of image to use when rendering the image.
            make_sdf : bool
                If True, image will be converted to an SDF format.  See
                the Mapbox Style Specification for more information about
                SDF images.
        )pbdoc",
             py::arg("name"),
             py::arg("image"),
             py::arg("width"),
             py::arg("height"),
             py::arg("ratio"),
             py::arg("make_sdf"))
        .def_property_readonly("bearing", &Map::getBearing)
        .def_property_readonly("center", &Map::getCenter)
        .def_property_readonly("pitch", &Map::getPitch)
        .def_property_readonly("size", &Map::getSize)
        .def_property_readonly("zoom", &Map::getZoom)
        .def(
            "renderPNG",
            [](Map &self) -> py::bytes { return py::bytes(self.renderPNG()); },
            py::call_guard<py::gil_scoped_release>(),
            R"pbdoc(
                Render the map to PNG bytes.
            )pbdoc")
        .def(
            "renderBuffer",
            [](Map &self) -> py::array_t<uint8_t> {
                std::pair<uint32_t, uint32_t> size = self.getSize();

                // always returns width * height * 4 (RGBA)
                size_t bytes = size.first * size.second * 4;

                // have to hold a reference until we are done
                auto img = self.renderBuffer();
                auto buf = img.get();

                // Alternative copy version
                // allocate array to receive the data, then copy in memory
                //  auto result                = py::array_t<uint8_t>(bytes);
                //  py::buffer_info result_buf = result.request();
                //  auto ptr                   = static_cast<uint8_t *>(result_buf.ptr);
                //  std::memcpy(ptr, buf, bytes);
                //  return result;

                // return a view of the data instead, using capsule to handle delete of underlying
                // memory
                // ref: https://github.com/pybind/pybind11/issues/323#issuecomment-575717041
                auto capsule = py::capsule(buf, [](void *p) {
                    std::unique_ptr<uint8_t>(reinterpret_cast<uint8_t *>(p));
                });
                img.release();
                auto arr = py::array(size.first * size.second * 4, buf, capsule);
                return arr;
            },
            R"pbdoc(
                Render the map to an array of uint8 pixel values.
            )pbdoc")
        .def("setBearing",
             &Map::setBearing,
             R"pbdoc(
                Set the bearing of the map.

                Parameters
                ----------
                bearing : float
                    Map bearing in degrees, between 0 and 360.
            )pbdoc",
             py::arg("bearing"))
        .def("setBounds",
             &Map::setBounds,
             R"pbdoc(
                Fit the map to the bounds, given an optional inset padding in pixels.

                Parameters
                ----------
                xmin : float
                ymin : float
                xmax : float
                ymax : float
                padding : int, optional (default: 0)
            )pbdoc",
             py::arg("xmin"),
             py::arg("ymin"),
             py::arg("xmax"),
             py::arg("ymax"),
             py::arg("padding") = 0)
        .def("setCenter",
             &Map::setCenter,
             R"pbdoc(
                Set the center of the map.

                Parameters
                ----------
                longitude : float
                latitude : float
            )pbdoc",
             py::arg("longitude"),
             py::arg("latitude"))
        .def("setPitch",
             &Map::setPitch,
             R"pbdoc(
                Set the pitch of the map.

                Parameters
                ----------
                pitch : float
                    Map pitch in degrees, between 0 and 85.
            )pbdoc",
             py::arg("pitch"))
        .def("setZoom",
             &Map::setZoom,
             R"pbdoc(
                Set the zoom level of the map.

                Parameters
                ----------
                zoom : float
                    Map zoom between 0 and 24
            )pbdoc",
             py::arg("zoom"))
        .def("setSize",
             &Map::setSize,
             R"pbdoc(
                Set the width and height of the map.

                Parameters
                ----------
                width : int
                height : int
            )pbdoc",
             py::arg("width"),
             py::arg("height"));
}