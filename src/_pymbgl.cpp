#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

#include "map.hpp"

namespace py = pybind11;
using namespace pybind11::literals;
using namespace py_mbgl;

PYBIND11_MODULE(_pymbgl, m) {
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
                 os << "pymbgl." << self;
                 return os.str();
             })
        .def("__repr__",
             [](Map &self) {
                 std::ostringstream os;
                 os << "pymbgl." << self;
                 return os.str();
             })
        .def_property_readonly("bearing", &Map::getBearing)
        .def_property_readonly("center", &Map::getCenter)
        .def_property_readonly("pitch", &Map::getPitch)
        .def_property_readonly("size", &Map::getSize)
        .def_property_readonly("zoom", &Map::getZoom)
        .def(
            "render",
            [](Map &self) -> py::bytes { return py::bytes(self.render()); },
            py::call_guard<py::gil_scoped_release>(),
            "Render the map to PNG data")
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