#!/usr/bin/env python

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import numpy
import sys

clang = False
if sys.platform.lower().startswith('darwin'):
    clang = True

from os.path import expanduser
distributions_inc = expanduser("~/distributions/include")
distributions_lib = expanduser("~/distributions-bin/lib")

extra_compile_args = ['-std=c++0x']
if clang:
    extra_compile_args.extend([
        '-mmacosx-version-min=10.7',  # for anaconda
        '-std=c++0x',
        '-stdlib=libc++',
    ])

extra_link_args = [
        '-L' + distributions_lib,
        '-Wl,-rpath,' + distributions_lib
    ]

def make_extension(name):
    return Extension(
        "microscopes." + name,
        sources=["microscopes/" + name + ".pyx"],
        libraries=["microscopes", "protobuf", "distributions_shared"],
        language="c++",
        include_dirs=[numpy.get_include(), 'include', 'microscopes', distributions_inc],
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args)

setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = [
        make_extension("hodgepodge"),
        make_extension("_models"),
    ],
)
