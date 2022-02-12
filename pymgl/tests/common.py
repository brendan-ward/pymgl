from io import BytesIO
import os
from pathlib import Path

from dotenv import load_dotenv
from PIL import Image, ImageChops


load_dotenv()
MAPBOX_TOKEN = os.getenv("MAPBOX_TOKEN", None)
if not MAPBOX_TOKEN:
    raise RuntimeError("MAPBOX_TOKEN env var must be set")


FIXTURES_PATH = Path("tests/fixtures").absolute()


def read_style(filename):
    """Read style file from fixtures and trim extra space"""
    return open(FIXTURES_PATH / filename).read()


def image_matches(img_data, expected_filename):
    """Compare image bytes to expected image file

    Parameters
    ----------
    img_data : bytes
    expected_filename : str

    Returns
    -------
    True if images match exactly, False otherwise
    """
    buffer = BytesIO(img_data)
    actual = Image.open(buffer)
    expected = Image.open(FIXTURES_PATH / expected_filename)

    actual.save(f"/tmp/{expected_filename}")

    if actual.size != expected.size:
        return False

    diff = ImageChops.difference(actual, expected)
    if diff.getbbox():
        return False

    return True
