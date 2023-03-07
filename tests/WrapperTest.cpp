#include <algorithm>
#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include <mbgl/util/rapidjson.hpp>

#include "map.h"
#include "util.h"

using namespace mgl_wrapper;
using namespace testing;
using namespace std;

// Tests are named TEST(<group name>, <test name>)

TEST(Wrapper, WidthHeightConstructor) {
    const string style = read_style("example-style-empty.json");

    // test constructor with minimum required values
    Map map   = Map(style, 10, 20);
    auto size = map.getSize();
    EXPECT_EQ(size.first, 10);
    EXPECT_EQ(size.second, 20);

    auto center = map.getCenter();
    EXPECT_NEAR(center.first, 0, 1e-6);
    EXPECT_NEAR(center.second, 0, 1e-6);

    EXPECT_NEAR(map.getZoom(), 0, 1e-6);
}

TEST(Wrapper, InvalidWidthHeightConstructor) {
    const string style = read_style("example-style-empty.json");

    // invalid dimensions should fail
    EXPECT_THROW(Map(style, 0, 0), std::domain_error);
    EXPECT_THROW(Map(style, 0, 10), std::domain_error);
    EXPECT_THROW(Map(style, 10, 0), std::domain_error);
}

TEST(Wrapper, RatioConstructor) {
    const string style = read_style("example-style-empty.json");

    // ratio doesn't have a getter, just be sure map doesn't fail
    EXPECT_NO_THROW(Map(style, 10, 10, 2));
}

TEST(Wrapper, InvalidRatioConstructor) {
    const string style = read_style("example-style-empty.json");

    // invalid ratio should fail
    EXPECT_THROW(Map(style, 10, 10, 0), std::domain_error);
}

TEST(Wrapper, CenterConstructor) {
    const string style = read_style("example-style-empty.json");

    // ratio doesn't have a getter, just be sure map doesn't fail
    Map map     = Map(style, 10, 10, 1, -180.0, -10.0);
    auto center = map.getCenter();
    EXPECT_NEAR(center.first, -180.0, 1e-6);
    EXPECT_NEAR(center.second, -10.0, 1e-6);
}

TEST(Wrapper, ZoomConstructor) {
    const string style = read_style("example-style-empty.json");

    Map map = Map(style, 10, 10, 1, {}, {}, 2);
    EXPECT_NEAR(map.getZoom(), 2, 1e-6);
}

TEST(Wrapper, InvalidZoomConstructor) {
    const string style = read_style("example-style-empty.json");

    EXPECT_THROW(Map(style, 10, 10, 1, {}, {}, -2), std::domain_error);
    EXPECT_THROW(Map(style, 10, 10, 1, {}, {}, 100), std::domain_error);
}

TEST(Wrapper, RequiredTokenProvider) {
    const string style = read_style("example-style-empty.json");

    EXPECT_THROW(Map(style, 10, 10, 1, {}, {}, {}, {}, "mapbox"), std::invalid_argument);
    EXPECT_THROW(Map(style, 10, 10, 1, {}, {}, {}, {}, "maptiler"), std::invalid_argument);

    // token is not valid for provider but is present so should not fail here
    EXPECT_NO_THROW(Map(style, 10, 10, 1, {}, {}, {}, "token", "mapbox"));
}

TEST(Wrapper, SetCenter) {
    const string style = read_style("example-style-empty.json");

    Map map = Map(style, 10, 10);

    map.setCenter(-180, -10);
    EXPECT_NEAR(map.getCenter().first, -180, 1e-6);
    EXPECT_NEAR(map.getCenter().second, -10, 1e-6);

    map.setCenter(160, 10);
    EXPECT_NEAR(map.getCenter().first, 160, 1e-6);
    EXPECT_NEAR(map.getCenter().second, 10, 1e-6);
}

TEST(Wrapper, SetZoom) {
    const string style = read_style("example-style-empty.json");

    Map map = Map(style, 10, 10);
    EXPECT_NEAR(map.getZoom(), 0, 1e-6);

    map.setZoom(2);
    EXPECT_NEAR(map.getZoom(), 2, 1e-6);

    EXPECT_THROW(map.setZoom(-2), std::domain_error);
}

TEST(Wrapper, SetBounds) {
    const string style = read_style("example-style-empty.json");

    Map map = Map(style, 512, 512);

    map.setBounds(-1, -1, 0, 0);
    EXPECT_NEAR(map.getCenter().first, -0.5, 1e-2);
    EXPECT_NEAR(map.getCenter().second, -0.5, 1e-2);
    EXPECT_NEAR(map.getZoom(), 8.492, 1e-2);
}

TEST(Wrapper, LayerFilter) {
    Map map = Map(read_style("example-style-geojson.json"), 10, 10);
    map.setLayerFilter("box", R"(["==", "foo", "bar"])");

    EXPECT_TRUE(map.getLayerFilter("box").has_value());
    EXPECT_EQ(map.getLayerFilter("box").value(), R"(["==", "foo", "bar"])");

    map.setLayerFilter("box", "");
    EXPECT_FALSE(map.getLayerFilter("box").has_value());

    map.setLayerFilter("box", std::optional<std::string>());
    EXPECT_FALSE(map.getLayerFilter("box").has_value());

    // empty style has no layers to set; should throw errors
    Map map2 = Map(read_style("example-style-empty.json"), 10, 10);
    EXPECT_THROW(map2.getLayerFilter("any_layer"), std::runtime_error);
    EXPECT_THROW(map2.setLayerFilter("any_layer", R"(["==", "foo", "bar"])"), std::runtime_error);
    EXPECT_THROW(map2.setLayerFilter("any_layer", ""), std::runtime_error);
    EXPECT_THROW(map2.setLayerFilter("any_layer", std::optional<std::string>()),
                 std::runtime_error);
}

TEST(Wrapper, LayerJSON) {
    Map map = Map(read_style("example-style-geojson.json"), 10, 10);

    auto actualJSON = map.getLayerJSON("box").value();

    const std::string expectedJSON = R"""({
  "source": "geojson",
  "type": "fill",
  "paint": {
    "fill-opacity": 0.5,
    "fill-color": ["rgba", 255, 0, 0, 1]
  },
  "id": "box"
})""";

    // WARNING: order of keys varies between platforms, so we have to parse the
    // JSON here and compare individual fields
    mbgl::JSDocument actual;
    actual.Parse(actualJSON.c_str());

    mbgl::JSDocument expected;
    expected.Parse(expectedJSON.c_str());

    EXPECT_EQ(actual["id"], expected["id"]);
    EXPECT_EQ(actual["source"], expected["source"]);
    EXPECT_EQ(actual["type"], expected["type"]);
    EXPECT_EQ(actual["paint"]["fill-color"], expected["paint"]["fill-color"]);
    EXPECT_EQ(actual["paint"]["fill-opacity"], expected["paint"]["fill-opacity"]);

    // empty style has no layers; should throw errors
    Map map2 = Map(read_style("example-style-empty.json"), 10, 10);
    EXPECT_THROW(map2.getLayerJSON("any_layer"), std::runtime_error);
}

TEST(Wrapper, LayerVisibility) {
    Map map = Map(read_style("example-style-geojson.json"), 10, 10);
    map.setLayerVisibility("box", true);
    EXPECT_EQ(map.getLayerVisibility("box"), true);

    map.setLayerVisibility("box", false);
    EXPECT_EQ(map.getLayerVisibility("box"), false);

    // empty style has no layers to set; should throw errors
    Map map2 = Map(read_style("example-style-empty.json"), 10, 10);
    EXPECT_THROW(map2.setLayerVisibility("any_layer", true), std::runtime_error);
    EXPECT_THROW(map2.setLayerVisibility("any_layer", false), std::runtime_error);
    EXPECT_THROW(map2.getLayerVisibility("any_layer"), std::runtime_error);
}

TEST(Wrapper, SetPitch) {
    const string style = read_style("example-style-empty.json");

    Map map = Map(style, 10, 10);

    map.setPitch(10);
    EXPECT_NEAR(map.getPitch(), 10, 1e-6);

    map.setPitch(40);
    EXPECT_NEAR(map.getPitch(), 40, 1e-6);

    EXPECT_THROW(map.setPitch(-1), std::domain_error);
    EXPECT_THROW(map.setPitch(90), std::domain_error);
}

TEST(Wrapper, SetBearing) {
    const string style = read_style("example-style-empty.json");

    Map map = Map(style, 10, 10);

    map.setBearing(10);
    EXPECT_NEAR(map.getBearing(), 10, 1e-6);

    map.setBearing(180);
    EXPECT_NEAR(map.getBearing(), 180, 1e-6);

    EXPECT_THROW(map.setPitch(-1), std::domain_error);
    EXPECT_THROW(map.setPitch(361), std::domain_error);
}

TEST(Wrapper, SetSize) {
    const string style = read_style("example-style-empty.json");

    Map map = Map(style, 10, 10);

    map.setSize(20, 30);
    EXPECT_EQ(map.getSize().first, 20);
    EXPECT_EQ(map.getSize().second, 30);

    map.setSize(1000, 2000);
    EXPECT_EQ(map.getSize().first, 1000);
    EXPECT_EQ(map.getSize().second, 2000);

    EXPECT_THROW(map.setSize(0, 0), std::domain_error);
    EXPECT_THROW(map.setSize(0, 10), std::domain_error);
    EXPECT_THROW(map.setSize(10, 0), std::domain_error);
}

TEST(Wrapper, RepeatedRender) {
    const string style = read_style("example-style-empty.json");

    Map map = Map(style, 255, 255);

    for (int i = 0; i < 5; i++) {
        map.renderPNG();
    }
}

TEST(Wrapper, MultipleMapInstances) {
    const string style = read_style("example-style-empty.json");

    for (int i = 0; i < 5; i++) {
        Map map = Map(style, 255, 255);
    }
}

TEST(Wrapper, MultipleMapRenders) {
    const string style = read_style("example-style-empty.json");

    for (int i = 0; i < 5; i++) {
        Map(style, 255, 255).renderPNG();
    }
}

TEST(Wrapper, ListLayers) {
    auto layers = Map(read_style("example-style-geojson.json"), 10, 10).listLayers();
    EXPECT_EQ(layers.size(), 2);
    EXPECT_EQ(layers[0], "box");
    EXPECT_EQ(layers[1], "box-outline");

    EXPECT_EQ(Map(read_style("example-style-empty.json"), 10, 10).listLayers().size(), 0);
}

TEST(Wrapper, ListSources) {
    auto sources = Map(read_style("example-style-geojson.json"), 10, 10).listSources();
    EXPECT_EQ(sources.size(), 1);
    EXPECT_EQ(sources[0], "geojson");

    EXPECT_EQ(Map(read_style("example-style-empty.json"), 10, 10).listSources().size(), 0);

    sources = Map(read_style("example-style-mbtiles-vector-source.json"), 10, 10).listSources();
    EXPECT_EQ(sources.size(), 1);
    EXPECT_EQ(sources[0], "land");
}