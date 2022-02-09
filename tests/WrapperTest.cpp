#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include <gtest/gtest.h>
#include <mbgl/storage/sqlite3.hpp>

#include "map.h"
#include "util.h"

namespace fs = std::filesystem;
using namespace mgl_wrapper;
using namespace testing;
using namespace std;

const string fixtures_dir = fs::absolute("../../tests/fixtures/").string();

// Tests are named TEST(<group name>, <test name>)

TEST(Wrapper, Empty) {
    const string test  = "example-style-empty";
    const string style = read_style(test + ".json");

    Map map  = Map(style, 10, 10, 1, 0, 0);
    auto img = map.render();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 0));
}

TEST(Wrapper, GeoJSON) {
    const string test  = "example-style-geojson";
    const string style = read_style(test + ".json");

    Map map = Map(style, 100, 100, 1);
    map.setBounds(-125, 37.5, -115, 42.5);
    auto img = map.render();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Wrapper, FileGeoJSON) {
    const string test = "example-style-file-geojson";
    string style      = read_style(test + ".json");

    // update style from relative to absolute
    style = regex_replace(style, regex("file://"), "file://" + fixtures_dir);
    cout << style << endl;

    Map map = Map(style, 100, 100, 1);
    map.setBounds(-125, 37.5, -115, 42.5);
    auto img = map.render();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Wrapper, RemoteRaster) {
    const string test  = "example-style-remote-raster";
    const string style = read_style(test + ".json");

    Map map  = Map(style, 256, 256, 1);
    auto img = map.render();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Wrapper, RemoteImageSource) {
    // remote image will render over top of raster
    const string test  = "example-style-remote-image-source";
    const string style = read_style(test + ".json");

    Map map  = Map(style, 256, 256, 1);
    auto img = map.render();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Wrapper, MapboxSource) {
    const string test  = "example-style-mapbox-source";
    const string style = read_style(test + ".json");

    const string token = get_token();

    Map map  = Map(style, 256, 256, 1, 0, 0, 0, token, "mapbox");
    auto img = map.render();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Wrapper, Labels) {
    // should render text labels
    const string test  = "example-style-geojson-labels";
    const string style = read_style(test + ".json");

    Map map = Map(style, 256, 256, 1, {}, {}, {}, {}, "maplibre");
    map.setBounds(-125, 37.5, -115, 42.5);
    auto img = map.render();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Wrapper, LocalMBtilesRasterSource) {
    const string test = "example-style-mbtiles-raster-source";
    string style      = read_style(test + ".json");

    // update style from relative to mbtiles_path to absolute
    style = regex_replace(style, regex("mbtiles://"), "mbtiles://" + fixtures_dir);

    Map map  = Map(style, 256, 256, 1);
    auto img = map.render();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Wrapper, LocalMBtilesVectorSource) {
    const string test = "example-style-mbtiles-vector-source";
    string style      = read_style(test + ".json");

    // update style from relative to mbtiles_path to absolute
    style = regex_replace(style, regex("mbtiles://"), "mbtiles://" + fixtures_dir);

    Map map  = Map(style, 256, 256, 1);
    auto img = map.render();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Wrapper, BadSource) {
    // bad tile source will not render, but GeoJSON features will
    const string test  = "example-style-bad-source";
    const string style = read_style(test + ".json");

    Map map = Map(style, 100, 100, 1);
    map.setBounds(-79.98, 32.64, -79.84, 32.79);
    auto img = map.render();

    const string img_filename = test + ".png";

    // to write out expected image, uncomment
    // write_test_image(img, img_filename, true);

    write_test_image(img, img_filename, false);
    EXPECT_TRUE(image_matches(img_filename, 10));
}

TEST(Wrapper, BadGlyphs) {
    // bad glyphs will make render fail with an exception
    const string test  = "example-style-bad-glyphs";
    const string style = read_style(test + ".json");

    Map map = Map(style, 100, 100, 1);
    map.setBounds(-125, 37.5, -115, 42.5);

    EXPECT_ANY_THROW(map.render());
}