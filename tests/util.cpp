#include <fstream>
#include <iostream>
#include <sstream>

#include <mapbox/pixelmatch.hpp>
#include <mbgl/util/image.hpp>

namespace testing {

using namespace std;
using namespace mbgl;

// relative to where tests are run: build/tests
const string style_dir    = "../../tests/fixtures/";
const string expected_dir = "../../tests/fixtures/";
const string actual_dir   = "/tmp/";

const string get_token() {
    // MAPBOX_TOKEN Must always be defined
    char *token = getenv("MAPBOX_TOKEN");
    if (token == NULL) {
        cout << "ERROR: MAPBOX_TOKEN env var must be set" << endl;
        throw invalid_argument("MAPBOX_TOKEN env var must be set");
    }
    return token;
}

const string read_style(const string &filename) {
    string styleFilename = style_dir + filename;
    ifstream jsonFile(styleFilename);
    if (!jsonFile) {
        throw invalid_argument("style json file does not exist: " + styleFilename);
    }
    stringstream buffer;
    buffer << jsonFile.rdbuf();
    jsonFile.close();

    if (buffer.str().empty()) {
        throw invalid_argument("style JSON must be non-empty");
    }

    return buffer.str();
}

PremultipliedImage read_image(const string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.good()) {
        throw invalid_argument("file not found: " + filename);
    }
    std::stringstream data;
    data << file.rdbuf();
    return decodeImage(data.str());
}

void write_image(const string &img, const string &filename) {
    std::ofstream out(filename, std::ios::binary);
    out << img;
    out.close();
}

void write_test_image(const string &img, const string &filename, bool is_expected) {
    if (is_expected) {
        write_image(img, expected_dir + filename);
    } else {
        write_image(img, actual_dir + filename);
    }
}

bool image_matches(const string &filename, uint64_t tolerance) {

    auto actual   = read_image(actual_dir + filename);
    auto expected = read_image(expected_dir + filename);

    // using threshold defined in Mapbox GL JS
    auto diff = mapbox::pixelmatch(actual.data.get(),
                                   expected.data.get(),
                                   expected.size.width,
                                   expected.size.height,
                                   nullptr,
                                   0.1285);

    bool matches = diff <= tolerance;
    if (diff > tolerance) {
        cout << filename << " differs by " << diff << " pixels\n";
    }

    return matches;
}

} // namespace testing