import os
import sys
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

# A CMakeExtension needs a sourcedir instead of a file list.
# The name must be the _single_ output extension from the CMake build.
# If you need multiple extensions, see scikit-build.
class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def build_extension(self, ext):
        if self.compiler.compiler_type == "msvc":
            raise NotImplementedError("pymgl is not supported on Windows")

        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))

        # required for auto-detection of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep

        cfg = "Debug" if self.debug else "Release"
        print(f"Build mode: {cfg}")

        cmake_args = [
            "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={}".format(extdir),
            "-DPYTHON_EXECUTABLE={}".format(sys.executable),
            "-DVERSION_INFO={}".format(self.distribution.get_version()),
            "-DCMAKE_BUILD_TYPE={}".format(cfg),
        ]
        build_args = []

        cmake_generator = os.environ.get("CMAKE_GENERATOR", "")
        if not cmake_generator:
            cmake_args += ["-GNinja"]

        # Set CMAKE_BUILD_PARALLEL_LEVEL to control the parallel build level
        # across all generators.
        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            # self.parallel is a Python 3 only way to set parallel jobs by hand
            # using -j in the build_ext call, not supported by pip or PyPA-build.
            if hasattr(self, "parallel") and self.parallel:
                build_args += ["-j{}".format(self.parallel)]

        tmp_dir = os.environ.get("BUILD_TEMP_DIR", self.build_temp)

        if not os.path.exists(tmp_dir):
            os.makedirs(tmp_dir)

        subprocess.check_call(["cmake", ext.sourcedir] + cmake_args, cwd=tmp_dir)
        subprocess.check_call(["cmake", "--build", "."] + build_args, cwd=tmp_dir)


setup(
    name="pymgl",
    version="0.1.0",
    packages=["pymgl"],
    url="https://github.com/brendan-ward/pymgl",
    license="MIT",
    author="Brendan C. Ward",
    author_email="bcward@astutespruce.com",
    description="Python wrapper for MapLibre GL native",
    long_description_content_type="text/markdown",
    long_description=open("README.md").read(),
    python_requires=">=3.8",
    ext_modules=[CMakeExtension("pymgl._pymgl")],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    extras_require={
        "test": [
            "pytest",
            "pytest-benchmark",
            "pytest-cov",
            "Pillow",
            "python-dotenv",
            "numpy",
            "requests",
            "pixelmatch",
        ]
    },
)
