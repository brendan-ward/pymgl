import os

from pathlib import Path

import pytest


styles_dir = Path("tests/fixtures")


@pytest.fixture(scope="session")
def geojson_style():
    return (
        open(styles_dir / "example-style-geojson.json")
        .read()
        .replace(" ", "")
        .replace("\n", " ")
    )


@pytest.fixture(scope="session")
def local_vector_style():
    # tiles are served at http://localhost:8001/services/states/tiles/{z}/{x}/{y}.pbf
    return open(styles_dir / "example-style-local-vector.json").read()
