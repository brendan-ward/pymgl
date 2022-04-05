from io import BytesIO
import os
from pathlib import Path

from dotenv import load_dotenv
from PIL import Image, ImageChops
from pixelmatch.contrib.PIL import pixelmatch

load_dotenv()
MAPBOX_TOKEN = os.getenv("MAPBOX_TOKEN", None)
FIXTURES_PATH = Path(__file__).parent.resolve() / "fixtures"


def read_style(filename):
    """Read style file from fixtures"""
    return open(FIXTURES_PATH / filename).read()


def image_matches(img_data, expected_filename, tolerance=0):
    """Compare image bytes to expected image file

    Parameters
    ----------
    img_data : bytes
    expected_filename : str
    tolerance : int
        number of pixels that are allowed to be different

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

    diff = pixelmatch(actual, expected, includeAA=False, threshold=0.1285)
    matches = diff <= tolerance

    if not matches:
        print(f"{expected_filename} differs by {diff} pixels")

    return matches
