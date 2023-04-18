#include <iostream>
#include <regex>
#include <sstream>

#include <gtest/gtest.h>
#include <mbgl/util/image.hpp>

#include "map.h"
#include "util.h"

using namespace mgl_wrapper;
using namespace testing;
using namespace std;

// Tests are named TEST(<group name>, <test name>)

TEST(Style, Empty) {
    const string test  = "example-style-empty";
    const string style = read_style(test + ".json");

    Map map  = Map(style, 10, 10, 1, 0, 0);
    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 0));
}

TEST(Style, GeoJSON) {
    const string test  = "example-style-geojson";
    const string style = read_style(test + ".json");

    Map map = Map(style, 100, 100, 1);
    map.setBounds(-125, 37.5, -115, 42.5);
    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Style, FileGeoJSON) {
    const string test = "example-style-file-geojson";
    string style      = read_style(test + ".json");

    // update style from relative to absolute
    style = regex_replace(style, regex("file://"), "file://" + FIXTURES_PATH);

    Map map = Map(style, 100, 100, 1);
    map.setBounds(-125, 37.5, -115, 42.5);
    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Style, RemoteRaster) {
    const string test  = "example-style-remote-raster";
    const string style = read_style(test + ".json");

    Map map  = Map(style, 256, 256, 1);
    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Style, RemoteImageSource) {
    // remote image will render over top of raster
    const string test  = "example-style-remote-image-source";
    const string style = read_style(test + ".json");

    Map map  = Map(style, 256, 256, 1);
    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Style, MapboxSource) {
    const string test  = "example-style-mapbox-source";
    const string style = read_style(test + ".json");

    const string token = get_token(false);
    if (token == "") {
        GTEST_SKIP_("Missing Mapbox Token");
    }

    Map map  = Map(style, 256, 256, 1, 0, 0, 0, token, "mapbox");
    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Style, MapboxStyle) {
    const string token = get_token(false);
    if (token == "") {
        GTEST_SKIP_("Missing Mapbox Token");
    }

    Map map  = Map("mapbox://styles/mapbox/streets-v11", 256, 256, 1, 0, 0, 0, token, "mapbox");
    auto img = map.renderPNG();

    const string img_filename = "mapbox-streets-v11.png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 100));
}

TEST(Style, BadRemoteStyleURL) {
    EXPECT_THROW(Map("https://google.com/bogus_style", 10, 10), std::runtime_error);
}

TEST(Style, Labels) {
    // should render text labels
    const string test  = "example-style-geojson-labels";
    const string style = read_style(test + ".json");

    Map map = Map(style, 256, 256, 1);
    map.setBounds(-125, 37.5, -115, 42.5);
    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Style, LocalMBtilesRasterSource) {
    const string test = "example-style-mbtiles-raster-source";
    string style      = read_style(test + ".json");

    // update style from relative to mbtiles_path to absolute
    style = regex_replace(style, regex("mbtiles://"), "mbtiles://" + FIXTURES_PATH);

    Map map  = Map(style, 256, 256, 1);
    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Style, LocalMBtilesVectorSource) {
    const string test = "example-style-mbtiles-vector-source";
    string style      = read_style(test + ".json");

    // update style from relative to mbtiles_path to absolute
    style = regex_replace(style, regex("mbtiles://"), "mbtiles://" + FIXTURES_PATH);

    Map map  = Map(style, 256, 256, 1);
    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 100));
}

TEST(Style, LocalMBtilesVectorSourceX2) {
    const string test = "example-style-mbtiles-vector-source";
    string style      = read_style(test + ".json");

    // update style from relative to mbtiles_path to absolute
    style = regex_replace(style, regex("mbtiles://"), "mbtiles://" + FIXTURES_PATH);

    // actual image will by 512 x 512
    Map map  = Map(style, 256, 256, 2);
    auto img = map.renderPNG();

    const string img_filename = test + "@2x.png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 100));
}

TEST(Style, InvalidLocalMBtilesRasterSource) {
    const string test = "example-style-mbtiles-raster-source";
    string style      = read_style(test + ".json");

    style = regex_replace(style, regex("mbtiles://"), "mbtiles:///invalid/");

    Map map = Map(style, 256, 256, 1);

    EXPECT_THROW(
        {
            try {
                map.renderPNG();
            } catch (const std::exception &e) {
                EXPECT_NE(std::string(e.what()).find("path not found"), std::string::npos);
                throw;
            }
        },
        std::exception);
}

TEST(Style, ImagePattern) {
    const string test = "example-style-image-pattern";
    string style      = read_style(test + ".json");

    Map map = Map(style, 256, 256, 1);
    map.setBounds(-125, 37.5, -115, 42.5);

    // read and decode a PNG image, and convert the uint8_t[] data to string
    mbgl::PremultipliedImage image = read_image(FIXTURES_PATH + "example-pattern.png");
    std::ostringstream image_str;
    for (size_t i = 0; i < image.bytes(); i++) {
        image_str << (uint8_t) image.data[i];
    }

    map.addImage("pattern", image_str.str(), image.size.width, image.size.height, 1, false);

    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 350));
}

// Tests of bad inputs
TEST(Style, BadSource) {
    // bad tile source will not render, but GeoJSON features will
    const string test  = "example-style-bad-source";
    const string style = read_style(test + ".json");

    Map map = Map(style, 100, 100, 1);
    map.setBounds(-79.98, 32.64, -79.84, 32.79);
    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Style, BadGlyphs) {
    // bad glyphs will make render fail with an exception
    const string test  = "example-style-bad-glyphs";
    const string style = read_style(test + ".json");

    Map map = Map(style, 100, 100, 1);
    map.setBounds(-125, 37.5, -115, 42.5);

    EXPECT_ANY_THROW(map.renderPNG());
}

TEST(Style, LayerVisibility) {
    const string test  = "example-style-geojson-hidden-box";
    const string style = read_style(test + ".json");

    Map map = Map(style, 100, 100, 1);
    map.setBounds(-125, 37.5, -115, 42.5);
    // toggle visible box hidden
    map.setLayerVisibility("box", false);
    EXPECT_EQ(map.getLayerVisibility("box"), false);

    // toggle hidden box visible
    map.setLayerVisibility("box2", true);
    EXPECT_EQ(map.getLayerVisibility("box2"), true);

    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Style, LayerFilter) {
    const string test  = "example-style-geojson-filter";
    const string style = read_style(test + ".json");

    Map map = Map(style, 100, 100, 1);
    map.setBounds(-125, 37.5, -115, 42.5);
    map.setLayerFilter("box", R"(["==", "id", 2])");

    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Style, AddBackgroundLayer) {
    const string test = "add-background";

    Map map = Map("", 100, 100, 1);
    map.addBackgroundLayer("background", "#0000FF");

    auto img = map.renderPNG();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}