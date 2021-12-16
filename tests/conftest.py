import os

from pathlib import Path

import pytest
from dotenv import load_dotenv

load_dotenv()


styles_dir = Path("tests/fixtures")


@pytest.fixture(scope="session")
def mapbox_token():
    # Must be on defined or contained a .env file in root directory
    token = os.getenv("MAPBOX_TOKEN", None)
    if token is None:
        raise ValueError("MAPBOX_TOKEN environment variable must be defined")

    return token


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
