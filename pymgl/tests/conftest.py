import pytest

from .common import read_style


@pytest.fixture(scope="session")
def empty_style():
    return read_style("example-style-empty.json")
