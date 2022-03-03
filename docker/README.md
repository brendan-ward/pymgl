# PyMGL Docker

## Development container

Build the development container (using source files on host):

```bash
docker build -f docker/Dockerfile.dev -t pymgl-dev .
```

Run the container mounted to the host filesystem for source files:

```bash
docker run -it -v "$PWD/:/app" pymgl-dev /bin/bash
```

Then from within the container, run:

```bash
python3.9 setup.py develop

# tests require virtual display
Xvfb ${DISPLAY} -screen 0 "1024x768x24" -ac +extension GLX +render -noreset  -nolisten tcp  &

# run C++ test suite
build/temp.linux-x86_64-3.9/tests/pymgl_test

# run Python test suite
pytest pymgl/tests
```

NOTE: this appears to build very slowly when using source files mounted on the host.

TODO: test setting env var to build in a local folder rather than against host; update setup.py.

To copy rendered PNG files from the running container:

```bash
docker cp <containerID>:/tmp /tmp/images
```

NOTE: it is normal for images rendered in MacOS to be slightly different than Linux, particularly
those rendered from vector tiles.
