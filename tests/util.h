#pragma once

#include <filesystem>
#include <sstream>

#include <mbgl/util/image.hpp>

namespace testing {

using namespace std;
namespace fs = std::filesystem;

const string FIXTURES_PATH = fs::absolute("tests/fixtures/").string();

const string get_token();
mbgl::PremultipliedImage read_image(const string &filename);
const string read_style(const string &filename);
void write_test_image(const string &img, const string &filename, bool is_expected);
bool image_matches(const string &filename, uint64_t tolerance);

} // namespace testing
