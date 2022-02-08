#include <fstream>
#include <iostream>
#include <sstream>

namespace testing {

// relative to where tests are run: build/tests
const std::string style_dir = "../../tests/fixtures/";

const std::string get_token() {
    // MAPBOX_TOKEN Must always be defined
    char *token = std::getenv("MAPBOX_TOKEN");
    if (token == NULL) {
        std::cout << "ERROR: MAPBOX_TOKEN env var must be set" << std::endl;
        throw std::invalid_argument("MAPBOX_TOKEN env var must be set");
    }
    return token;
}

const std::string read_style(std::string filename) {
    std::string styleFilename = style_dir + filename;
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

    return buffer.str();
}

} // namespace testing