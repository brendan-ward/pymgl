#!/bin/bash

# this runs inside a manylinux container and produces manylinux wheels in dist on host

for PYTHON_VERSION in 3.9
# 3.10 3.11 3.12 3.13
do
    uv venv /uv/$PYTHON_VERSION.venv --python $PYTHON_VERSION
    . /uv/$PYTHON_VERSION.venv/bin/activate
    uv pip install setuptools versioneer auditwheel tomli
    uv build --wheel --python $PYTHON_VERSION --out-dir /wheels
    deactivate
    rm -rf /uv/*
    echo
    echo "===================== DONE BUILDING $PYTHON_VERSION WHEELS ==================="
    echo
    echo
done

auditwheel repair /wheels/* -w dist