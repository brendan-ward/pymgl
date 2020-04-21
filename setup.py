import os
from pathlib import Path
import shutil
import subprocess
import sys

from Cython.Build import cythonize
from Cython.Distutils import build_ext
from setuptools import setup
from setuptools.command.build_ext import build_ext as _build_ext
from setuptools.extension import Extension


# TODO: make env vars
mbgl = Path("../lib/mapbox/mapbox-gl-native").resolve()
geometryhpp_include = Path("../lib/mapbox/geometry.hpp/include").resolve()
varianthpp_include = Path("../lib/mapbox/variant/include").resolve()

mbgl_include = mbgl / "include"
mbgl_vendor_include = mbgl / "vendor"


ext_options = {
    # not all internal includes within mbgl use full paths
    "include_dirs": [
        str(mbgl_include),
        str(mbgl_include / "mbgl"),
        str(mbgl_vendor_include / "mapbox-base/include"),
        str(mbgl_vendor_include / "mapbox-base/deps/optional"),
        str(geometryhpp_include),
        str(varianthpp_include),
    ],
    "library_dirs": [str(mbgl / "build")],
    "libraries": ["mbgl-core"],
    "extra_compile_args": ["-std=c++14"],
    "extra_link_args": [],
    "language": "c++",
}


# setuptools clean does not cleanup Cython artifacts
if "clean" in sys.argv:
    if os.path.exists("build"):
        shutil.rmtree("build")

    root = Path(".")
    for ext in ["*.so", "*.pyc", "*.c", "*.cpp"]:
        for entry in root.rglob(ext):
            entry.unlink()

setup(
    name="pymbgl",
    version="0.1.0",
    packages=["pymbgl"],
    url="https://github.com/brendan-ward/pymbgl",
    license="MIT",
    author="Brendan C. Ward",
    author_email="bcward@astutespruce.com",
    description="Python wrapper for Mapbox GL native",
    long_description_content_type="text/markdown",
    long_description=open("README.md").read(),
    python_requires=">=3.8",
    install_requires=[],
    extras_require={"dev": ["Cython", "pytest", "pytest-cov", "pytest-benchmark",],},
    include_package_data=True,
    ext_modules=cythonize(
        [Extension("pymbgl._map", ["pymbgl/_map.pyx"], **ext_options),],
        compiler_directives={"language_level": "3"},
    ),
)
