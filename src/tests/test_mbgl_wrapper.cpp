#include <fstream>
#include <iostream>
#include <sstream>

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
#include <mbgl/util/run_loop.hpp>

#include "map.hpp"

using namespace mbgl_wrapper;

int main(int argc, char *argv[]) {
    std::chrono::steady_clock clock;
    auto start   = clock.now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - start);

    const float pixelRatio          = 1;
    const uint32_t width            = 256;
    const uint32_t height           = 256;
    const std::string styleFilename = "../tests/fixtures/example-style-geojson.json";
    const double lon                = -119.81689453125;
    const double lat                = 39.52099229357195;
    const double zoom               = 1;
    const double bearing            = 0;
    const double pitch              = 0;

    // TODO: may be able to have map fit bounds instead, or call the same thing to figure out
    // center / zoom it uses
    const double xmin       = -100;
    const double ymin       = -10;
    const double xmax       = 0;
    const double ymax       = 50;
    const double pad_pixels = 0;

    try {
        // MAPBOX_TOKEN Must always be defined
        char *rawToken = std::getenv("MAPBOX_TOKEN");
        if (rawToken == NULL) {
            std::cout << "ERROR: MAPBOX_TOKEN env var must be set" << std::endl;
            exit(1);
        }
        // const std::string token = rawToken;

        std::cout << "Read style file: " << styleFilename << std::endl;
        std::ifstream jsonFile(styleFilename);
        if (!jsonFile) {
            throw std::invalid_argument("style json file does not exist: " + styleFilename);
        }
        std::stringstream buffer;
        buffer << jsonFile.rdbuf();
        jsonFile.close();

        if (buffer.str().empty()) {
            throw std::invalid_argument("style JSON must be non-empty");
        }

        const std::string style = buffer.str();

        std::cout << "#### Create map 1" << std::endl;
        Map map = Map(style, 256, 256, 1, -100, 20, 1);
        // map.setSize(200, 400);
        // map.setBearing(20);
        // map.setPitch(50);

        auto img = map.render();
        std::ofstream out("/tmp/test1.png", std::ios::binary);
        out << img;
        out.close();

        // std::cout << "#### Create map 2" << std::endl;
        // map.setBounds(-100, 0, -20, 40);
        // img = map.render();
        // out = std::ofstream("/tmp/test2.png", std::ios::binary);
        // out << img;
        // out.close();

    } catch (std::exception &e) {
        std::cout << "ERROR: " << e.what() << std::endl;
        abort();
        // exit(1);
        // throw e;
    }

    std::cout << "Done!" << std::endl;

    return 0;
}
