#include <sstream>

namespace testing {

using namespace std;

const string get_token();
const string read_style(const string &filename);
void write_test_image(const string &img, const string &filename, bool is_expected);
bool image_matches(const string &filename, uint64_t tolerance);

} // namespace testing
