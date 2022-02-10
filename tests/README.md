## C++ tests

The test suite is compiled by default using the CMake configuration in the root
of this project. This creates an executable in `build/<subdir>/tests/pymgl_test`.

`<subdir>` is only present if you used python to do the compilation via something
like `python setup.py build_ext --inplace`. It will be something like
`temp.macosx-10.15-x86_64-3.9`.

If you used CMake directly, the executable will be at `build/tests/pymgl_test`.

Unit test files are contained in `tests/*Test.cpp` files using the
[GoogleTest](https://google.github.io/googletest/) framework.

The `MAPBOX_TOKEN` environment variable must be set in order for the tests to run.

Running tests requires a network connection, as the tests fetch and render
remote map tiles and other assets.

To run all tests:

```bash
<executable_dir>/pymgl_test -v
```

To run an individual tests:

```bash
<executable_dir>/pymgl_test --gtest_filter="Wrapper.SetBounds" -v
```

Where `Wrapper` is the name of the test suite, and `SetBounds` is the name of a specific test case.

## Python tests

After compiling the package using `python setup.py build_ext --inplace` or similar,
from the root directory run all tests using:

```bash
pytest pymgl/tests
```
