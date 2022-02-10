#include <iostream>

#include <gtest/gtest.h>

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
