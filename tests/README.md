# C++ tests

The test suite is compiled by default using the CMake configuration in the root
of this project.

Unit test files are contained in `tests/*Test.cpp` using the
[GoogleTest](https://google.github.io/googletest/) framework.

The `MAPBOX_TOKEN` environment variable must be set in order for the tests to run.

From within the `build` directory, after compiling everything, run:

```bash

ctest -V
```

To run an individual tests:

```bash
TODO:
```

To run with Valgrind (Linux only):

```bash
ctest --output-on-failure --overwrite MemoryCheckCommandOptions="--leak-check=full --error-exitcode=100" -C Valgrind -T memcheck
```
