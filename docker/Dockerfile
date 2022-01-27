# docker build -f docker/Dockerfile -t pymgl .
# docker run -it pymgl /bin/bash
# docker run -it -v "$PWD/:/app" pymgl /bin/bash
FROM debian:bullseye-slim

WORKDIR /app

RUN apt-get update && apt-get -y install \
    git \
    # unzip \
    # build tools
    build-essential \
    cmake \
    ninja-build \
    # required libs
    # curl \ ?
    # libcairo2-dev \ ?
    python3 \
    python3-dev \
    python3-pip \
    libcurl4-openssl-dev \
    libicu-dev \

    # GL / display libs
    libgles2-mesa-dev \
    libgbm-dev \
    # TODO: EGL
    # for EGL - not sure which are needed
    # libegl1 \
    # libegl-dev \
    # libegl-mesa0 \
    # libegl1-mesa-dev \

    libjpeg62-turbo-dev \
    libllvm11 \
    libprotobuf-dev \
    libuv1-dev \
    libxxf86vm-dev \
    xvfb \
    x11-utils


# TODO: only init the submodules we need
# Download snapshot of maplibre-gl-native
RUN git clone --depth 1 -b issue32 https://github.com/brendan-ward/maplibre-gl-native.git /maplibre-gl-native && \
    cd /maplibre-gl-native && \
    git submodule update --init --recursive --remote

# COPY . /app/


#  Install test deps
RUN pip install pytest python-dotenv Pillow numpy

RUN python3 setup.py develop

# Xvfb must be running or things fail
# export DISPLAY=:99
# entrypoint:
# Xvfb ${DISPLAY} -screen 0 "1024x768x24" -ac +extension GLX +render -noreset  -nolisten tcp  &


# TODO: remove build-essential, cmake, etc
# remove contents of build directory
# TODO: once statically compiled, are libs needed anymore?