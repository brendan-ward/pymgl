# PyMGL Docker

## Development container

Build the development container (using source files on host):

```bash
docker build -f docker/Dockerfile.ubuntu22.04 -t pymgl-dev-ubuntu22.04 .
```

Run the container mounted to the host filesystem for source files:

```bash
docker run -it -v "$PWD/:/app" pymgl-dev-ubuntu22.04 /bin/bash
```

Then from within the container, run:

```bash
python3 setup.py build_ext --debug --inplace
python3 setup.py develop

# tests require virtual display
Xvfb ${DISPLAY} -screen 0 "1024x768x24" -ac +render -noreset -nolisten tcp  &

# or this if using GLX:
# Xvfb ${DISPLAY} -screen 0 "1024x768x24" -ac +extension GLX +render -noreset  -nolisten tcp  &

export MAPBOX_TOKEN=<your token>

# run C++ test suite
/tmp/build/tests/pymgl_test

# run Python test suite
pytest pymgl/tests
```

You can also run the tests using `xvfb-run` to sidestep the need to start Xvfb:

```bash
xvfb-run -a --server-args="-screen 0 1024x768x24 -ac +render -noreset" /tmp/build/tests/pymgl_test
```

To copy rendered PNG files from the running container:

```bash
docker cp <containerID>:/tmp /tmp/images
```

NOTE: it is normal for images rendered in MacOS to be slightly different than Linux, particularly
those rendered from vector tiles.

To test for memory leaks using the C++ tests, run:

```bash
valgrind --show-leak-kinds=definite --track-origins=yes --log-file=/tmp/valgrind-output /tmp/build/tests/pymgl_test
```

To run a specific test:

```bash
valgrind -s --leak-check=full --track-origins=yes --log-file=/tmp/valgrind-output /tmp/build/tests/pymgl_test --gtest_filter="Wrapper.WidthHeightConstructor"
```

Then copy to host:

```bash
docker cp <containerID>:/tmp/valgrind-output /tmp/
```

To test for memory leaks using the Python tests, run:

```bash
valgrind --show-leak-kinds=definite --log-file=/tmp/valgrind-output python3 -m pytest pymgl/tests -vv --valgrind --valgrind-log=/tmp/valgrind-output > valgrind.log
```
