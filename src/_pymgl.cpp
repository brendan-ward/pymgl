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
             "Class that provides static rendering using MapLibre GL native",
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
        .def("addImage", &Map::addImage, "add an image to the map")
        .def_property_readonly("bearing", &Map::getBearing)
        .def_property_readonly("center", &Map::getCenter)
        .def_property_readonly("pitch", &Map::getPitch)
        .def_property_readonly("size", &Map::getSize)
        .def_property_readonly("zoom", &Map::getZoom)
        .def(
            "renderPNG",
            [](Map &self) -> py::bytes {
                return py::bytes(self.renderPNG());
            },
            py::call_guard<py::gil_scoped_release>(),
            "Render the map to PNG data")
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
            //   py::call_guard<py::gil_scoped_release>(),
            "Render the map to a uint8 numpy array")
        .def("setBearing", &Map::setBearing, "Set the bearing of the map")
        .def("setBounds",
             &Map::setBounds,
             "Fit the map to the bounds, given an optional inset padding in pixels",
             py::arg("xmin"),
             py::arg("ymin"),
             py::arg("xmax"),
             py::arg("ymax"),
             py::arg("padding") = 0)
        .def("setCenter",
             &Map::setCenter,
             "Set the center longitude and latitude of the map",
             py::arg("longitude"),
             py::arg("latitude"))
        .def("setPitch", &Map::setPitch, "Set the pitch of the map")
        .def("setZoom", &Map::setZoom, "Set the zoom of the map")
        .def("setSize",
             &Map::setSize,
             "Set the width and height of the map",
             py::arg("width"),
             py::arg("height"));
}