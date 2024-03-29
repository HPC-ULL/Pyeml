#!/usr/bin/env python


from pybind11.setup_helpers import Pybind11Extension, build_ext, intree_extensions
from setuptools import setup

__version__ = "0.0.1"

setup_helper = Pybind11Extension(
        "eml",
        ["src/pyeml/lib/eml.cpp"],
        )


#, "-Wl,--as-needed",  "-lnvidia-ml" ,  "-Wl,--no-as-needed"

setup_helper._add_cflags(["-shared"])
setup_helper._add_ldflags(["-L/usr/local/lib", "-leml" ,"-lconfuse", "-std=c++11"])


setup(
    name="pyeml",
    version=__version__,
    author="Daniel Suárez Labena",
    author_email="dsuarezl@ull.edu.es",
    url="https://github.com/dsuarezl/Pyeml",
    description="Tool to measure energy consumption of python code",
    long_description="",
    ext_modules=[setup_helper],
    packages = ["pyeml",
                "pyeml.devices",
                "pyeml.units"
    ],
    package_dir={'pyeml': 'src/pyeml',
    'pyeml.devices': 'src/pyeml/devices',
    'pyeml.units': 'src/pyeml/units',
    },
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)