#!/bin/bash

# this runs on a MacOS host; wheels do not need to be delocated

for PYTHON_VERSION in 3.9 3.10 3.11 3.12 3.13
do
    uv build --wheel --python $PYTHON_VERSION --out-dir dist
    echo
    echo "===================== DONE BUILDING $PYTHON_VERSION WHEELS ==================="
    echo
    echo
done
