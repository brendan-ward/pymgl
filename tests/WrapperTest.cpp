#include <fstream>
#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

// #include <mbgl/gfx/headless_frontend.hpp>
// #include <mbgl/map/map.hpp>
// #include <mbgl/map/map_observer.hpp>
// #include <mbgl/map/map_options.hpp>
// #include <mbgl/storage/file_source_manager.hpp>
// #include <mbgl/storage/network_status.hpp>
// #include <mbgl/style/style.hpp>
// #include <mbgl/util/default_style.hpp>
// #include <mbgl/util/exception.hpp>
// #include <mbgl/util/image.hpp>
// #include <mbgl/util/logging.hpp>

#include "map.h"
#include "util.h"

using namespace mgl_wrapper;
using namespace testing;
using namespace std;

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
    assert(image_matches(img_filename, 0) == true);
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
    assert(image_matches(img_filename, 10) == true);
}