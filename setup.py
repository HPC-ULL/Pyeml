#!/usr/bin/env python

import sys

# Available at setup time due to pyproject.toml
from pybind11 import get_cmake_dir
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

__version__ = "0.0.1"


setup_helper = Pybind11Extension("pyeml",
        ["src/pyeml/lib/eml.cpp"],
        # Example: passing in the version to the compiled code
        define_macros = [('VERSION_INFO', __version__)],
        )

setup_helper._add_cflags("-O3", "-shared" ,"-std=c++11" ,"-fPIC")
setup_helper._add_ldflags("-leml" , "-lconfuse", "-Wl," ,"--as-needed",  "-lnvidia-ml" ,  "-Wl," , "--no-as-needed")

ext_modules = [
 setup_helper
]

setup(
    name="pyeml",
    version=__version__,
    author="Daniel Suárez Labena",
    author_email="dsuarezl@ull.edu.es",
    url="https://github.com/dsuarezl/Pyeml",
    description="A test project using pybind11",
    long_description="",
    ext_modules=ext_modules,
    extras_require={"test": "pytest"},
    # Currently, build_ext only provides an optional "highest supported C++
    # level" feature, but in the future it may provide more features.
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)