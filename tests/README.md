# PyMGL tests

## C++ tests

Building using CMake directly is useful when building the C++ level tests of the
Maplibre Native wrapper.

To build the tests directly:

```bash
mkdir build

cmake . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
cmake --build build

build/tests/pymgl_test -v
```

Alternatively, the test suite is automatically compiled as part of building the
python extension, and will be located in `build/<subdir>/tests/pymgl_test`,
where `<subdir>` is something like `temp.macosx-12.6-arm64-cpython-310`.

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

## Manual network tests

To test network-related issues issues, in one terminal do the following:

```
pip install poorconn[full]

python -m poorconn close_upon_acceptance
```

Given a style JSON:

Then in another terminal, run this as needed across different combinations of poorconn settings:

```
python -c "from pymgl import Map; Map(open('pymgl/tests/fixtures/example-style-bad-remote.json').read()).renderPNG()"
```

This will error with a connection lost.

In the first terminal, run the following:

```bash
python -m poorconn delay_before_sending_upon_acceptance --t=1 --length=1024
```

This should not fail.

To make it really slow:

```
python -m poorconn delay_before_sending_upon_acceptance --t=1 --length=10
```

This should have an error for request timing out.
