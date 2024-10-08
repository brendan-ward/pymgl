#include <iostream>
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <sstream>

#include "log_observer.h"
#include "map.h"

namespace nb = nanobind;
using namespace nanobind::literals;
using namespace mgl_wrapper;

NB_MODULE(_pymgl, m) {
    // Setup logging when module is imported
    // TODO: pass errors / warnings back to Python
    mbgl::Log::setObserver(std::make_unique<LogObserver>());

    m.doc() = "MapLibre Native static renderer";

    nb::class_<Map>(m, "Map")
        .def(nb::init<const std::string &,
                      const std::optional<uint32_t> &,
                      const std::optional<uint32_t> &,
                      const std::optional<float> &,
                      const std::optional<double> &,
                      const std::optional<double> &,
                      const std::optional<double> &,
                      const std::optional<std::string> &,
                      const std::optional<std::string> &>(),
             R"pbdoc(
            Create Maplibre Native map instance.

            Parameters
            ----------
            style : str
                Mapbox GL / Maplibre GL style object as json-encoded string.
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
             nb::arg("style"),
             nb::arg("width")     = 256,
             nb::arg("height")    = 256,
             nb::arg("ratio")     = 1,
             nb::arg("longitude") = 0,
             nb::arg("latitude")  = 0,
             nb::arg("zoom")      = 0,
             nb::arg("token")     = nb::none(),
             nb::arg("provider")  = nb::none())
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
        .def("__enter__", [&](Map &self) { return &self; })
        .def(
            "__exit__",
            [&](Map &self,
                nb::object exc_type  = nb::none(),
                nb::object exc_value = nb::none(),
                nb::object traceback = nb::none()) { self.release(); },
            nb::arg("exc_type").none(),
            nb::arg("exc_value").none(),
            nb::arg("traceback").none())
        .def(
            "addImage",
            [](Map &self,
               const std::string &name,
               nb::bytes &image,
               uint32_t width,
               uint32_t height,
               float ratio   = 1,
               bool make_sdf = false) {
                // convert bytes to std::string
                const std::string imageStr(image.c_str(), image.size());
                self.addImage(name, imageStr, width, height, ratio, make_sdf);
            },
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
        )pbdoc")
        .def("addSource",
             &Map::addSource,
             R"pbdoc(
                Add a source to the map.

                Parameters
                ---------
                id : str
                    ID of the source.  Used when referencing this source in the style.
                options : str
                    JSON-encoded source options.  Fields are specific to the source type.
            )pbdoc",
             nb::arg("id"),
             nb::arg("options"))
        .def("addLayer",
             &Map::addLayer,
             R"pbdoc(
                Add a layer to the map.

                Parameters
                ----------
                options : str
                    JSON-encoded layer options.  Fields are specific to the layer type.
            )pbdoc",
             nb::arg("options"))
        .def_prop_ro("bearing", &Map::getBearing)
        .def_prop_ro("center", &Map::getCenter)
        .def_prop_ro("pitch", &Map::getPitch)
        .def_prop_ro("size", &Map::getSize)
        .def_prop_ro("zoom", &Map::getZoom)
        .def("getFeatureState",
             &Map::getFeatureState,
             R"pbdoc(
                Get the current feature state of a feature

                NOTE: map must be loaded first.

                Parameters
                ----------
                sourceID : str
                    source ID
                layerID : str
                    layer ID
                featureID : str
                    feature ID

                Returns
                -------
                JSON str or None
            )pbdoc",
             nb::arg("sourceID"),
             nb::arg("layerID"),
             nb::arg("featureID"))
        .def("getFilter",
             &Map::getFilter,
             R"pbdoc(
                Get the filter of a layer in the map

                Parameters
                ----------
                layerID : str
                    id of layer in map

                Returns
                -------
                JSON str or None
            )pbdoc",
             nb::arg("layerID"))
        .def("getPaintProperty",
             &Map::getPaintProperty,
             R"pbdoc(
                Get the value of a layer paint property in the map

                Parameters
                ----------
                layerID : str
                    id of layer in map
                property : str
                    name of the paint property

                Returns
                -------
                JSON str or None
            )pbdoc",
             nb::arg("layerID"),
             nb::arg("property"))
        .def("getLayerJSON",
             &Map::getLayerJSON,
             R"pbdoc(
                Get JSON that describes a layer

                Parameters
                ----------
                layerID : str
                    id of layer in map

                Returns
                -------
                str
            )pbdoc",
             nb::arg("layerID"))
        .def("getVisibility",
             &Map::getVisibility,
             R"pbdoc(
                Get the visibility of a layer in the map

                Parameters
                ----------
                layerID : str
                    id of layer in map

                Returns
                -------
                bool
            )pbdoc",
             nb::arg("layerID"))
        .def("listLayers", &Map::listLayers)
        .def("listSources", &Map::listSources)
        .def("load", &Map::load)
        .def("removeFeatureState",
             &Map::removeFeatureState,
             R"pbdoc(
                Removes the feature state for a single state key of a feature.

                NOTE: map must be loaded first and map must be rendered after
                calling removeFeatureState to update state.

                Parameters
                ----------
                sourceID : str
                    source ID
                layerID : str
                    layer ID
                featureID : str
                    feature ID
                stateKey : str
                    key in feature state to remove
            )pbdoc",
             nb::arg("sourceID"),
             nb::arg("layerID"),
             nb::arg("featureID"),
             nb::arg("stateKey"))
        .def(
            "render",
            [](Map &self) {
                // release the GIL while rendering
                nb::gil_scoped_release release;
                self.render();
            },
            R"pbdoc(
                Force the map to render in order to load assets and update state.
            )pbdoc")
        .def(
            "renderPNG",
            [](Map &self) -> nb::bytes {
                // release the GIL while rendering to PNG but reacquire before
                // returning Python objects
                nb::gil_scoped_release release;
                const std::string png = self.renderPNG();
                nb::gil_scoped_acquire acquire;

                return nb::bytes(png.c_str(), png.size());
            },
            R"pbdoc(
                Render the map to PNG bytes.
            )pbdoc")
        .def(
            "renderBuffer",
            [](Map &self) {
                // release the GIL while rendering to PNG but reacquire before
                // returning Python objects
                nb::gil_scoped_release release;

                // always returns width * height * 4 (RGBA)
                std::pair<uint32_t, uint32_t> size = self.getSize();
                size_t shape[1]                    = {size.first * size.second * 4};

                // have to hold a reference until we are done
                auto img = self.renderBuffer();
                auto buf = img.get();

                nb::gil_scoped_acquire acquire;

                // return a view of the data instead, using capsule to handle
                // delete of underlying memory
                // (see: https://nanobind.readthedocs.io/en/latest/ndarray.html)
                nb::capsule owner(buf, [](void *p) noexcept {
                    std::unique_ptr<uint8_t>(reinterpret_cast<uint8_t *>(p));
                });
                img.release();

                return nb::ndarray<nb::numpy, uint8_t, nb::shape<1>>(buf, 1, shape, owner);
            },
            R"pbdoc(
                Render the map to a numpy array of uint8 pixel values.
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
             nb::arg("bearing"))
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
             nb::arg("xmin"),
             nb::arg("ymin"),
             nb::arg("xmax"),
             nb::arg("ymax"),
             nb::arg("padding") = 0)
        .def("setCenter",
             &Map::setCenter,
             R"pbdoc(
                Set the center of the map.

                Parameters
                ----------
                longitude : float
                latitude : float
            )pbdoc",
             nb::arg("longitude"),
             nb::arg("latitude"))
        .def("setFeatureState",
             &Map::setFeatureState,
             R"pbdoc(
                Sets the current feature state of a feature.

                NOTE: map must be loaded first.

                Parameters
                ----------
                sourceID : str
                    source ID
                layerID : str
                    layer ID
                featureID : str
                    feature ID
                state : str
                    JSON-encoded feature state
            )pbdoc",
             nb::arg("sourceID"),
             nb::arg("layerID"),
             nb::arg("featureID"),
             nb::arg("state"))
        .def("setGeoJSON",
             &Map::setGeoJSON,
             R"pbdoc(
                Set GeoJSON data on a GeoJSON source in the map.

                Parameters
                ----------
                sourceID : str
                    ID of the source, which must already exist in the map
                geoJSON : str
                    JSON-encoded GeoJSON data
             )pbdoc",
             nb::arg("sourceID"),
             nb::arg("geoJSON"))
        .def("setFilter",
             &Map::setFilter,
             R"pbdoc(
                Set the filter of a layer in the map

                Parameters
                ----------
                layerID : str
                    id of layer in map
                visible : str, optional (default None)
                    JSON string or None / empty string to clear filter
            )pbdoc",
             nb::arg("layerID"),
             nb::arg("filter") = nb::none())
        .def("setPaintProperty",
             &Map::setPaintProperty,
             R"pbdoc(
                Set a paint property of a layer in the map

                Parameters
                ----------
                id : str
                    id of layer in map
                property : str
                    name of the paint property
                value : str
                    JSON string of paint property value
            )pbdoc",
             nb::arg("layerID"),
             nb::arg("property"),
             nb::arg("value"))
        .def("setVisibility",
             &Map::setVisibility,
             R"pbdoc(
                Set the visibility of a layer in the map

                Parameters
                ----------
                layerID : str
                    id of layer in map
                visible : bool
                    set to True to make layer visible
            )pbdoc",
             nb::arg("layerID"),
             nb::arg("visible"))
        .def("setPitch",
             &Map::setPitch,
             R"pbdoc(
                Set the pitch of the map.

                Parameters
                ----------
                pitch : float
                    Map pitch in degrees, between 0 and 85.
            )pbdoc",
             nb::arg("pitch"))
        .def("setZoom",
             &Map::setZoom,
             R"pbdoc(
                Set the zoom level of the map.

                Parameters
                ----------
                zoom : float
                    Map zoom between 0 and 24
            )pbdoc",
             nb::arg("zoom"))
        .def("setSize",
             &Map::setSize,
             R"pbdoc(
                Set the width and height of the map.

                Parameters
                ----------
                width : int
                height : int
            )pbdoc",
             nb::arg("width"),
             nb::arg("height"));
}