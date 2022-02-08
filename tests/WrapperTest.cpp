#include <fstream>
#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

#include <mbgl/gfx/headless_frontend.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/map/map_observer.hpp>
#include <mbgl/map/map_options.hpp>
#include <mbgl/storage/file_source_manager.hpp>
#include <mbgl/storage/network_status.hpp>
#include <mbgl/style/style.hpp>
#include <mbgl/util/default_style.hpp>
#include <mbgl/util/exception.hpp>
#include <mbgl/util/image.hpp>
#include <mbgl/util/logging.hpp>
// #include <mbgl/util/run_loop.hpp>

#include "map.h"
#include "util.h"

using namespace mgl_wrapper;
using namespace testing;

// Tests are named TEST(<group name>, <test name>)

TEST(Wrapper, Empty) {
    const std::string style = read_style("example-style-empty.json");

    Map map  = Map(style, 256, 256, 1, 0, 0);
    auto img = map.render();
    std::ofstream out("/tmp/example-style-empty.png", std::ios::binary);
    out << img;
    out.close();
}

TEST(Wrapper, GeoJSON) {
    const std::string style = read_style("example-style-geojson.json");

    Map map = Map(style, 512, 512, 1);
    map.setBounds(-125, 37.5, -115, 42.5);
    auto img = map.render();
    std::ofstream out("/tmp/example-style-geojson.png", std::ios::binary);
    out << img;
    out.close();
}