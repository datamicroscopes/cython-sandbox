#!/usr/bin/env python

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

import numpy
import sys

clang = False
if sys.platform.lower().startswith('darwin'):
    clang = True

import os
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

non_exported_module_sources = [
    #'microscopes/cxx/_models.pyx',
    #'microscopes/cxx/common/_dataview.pyx',
    #'microscopes/cxx/common/_rng.pyx',
    #'microscopes/cxx/mixture/_model.pyx',
]

def make_extension(name):
    name = ['microscopes'] + name
    module_name = '.'.join(name)
    module_source = '/'.join(name) + '.pyx'
    module_pxd = '/'.join(name) + '.pxd'
    return _make_extension(module_name, [module_source] + non_exported_module_sources)

def _make_extension(name, sources):
    return Extension(
        name,
        sources=sources,
        libraries=["microscopes", "protobuf", "distributions_shared"],
        language="c++",
        include_dirs=[numpy.get_include(), 'include', 'microscopes', distributions_inc],
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args)

extensions = cythonize([
        make_extension(['cxx', 'models']),
        make_extension(['cxx', '_models']),
        make_extension(['cxx', 'common', 'dataview']),
        make_extension(['cxx', 'common', '_dataview']),
        make_extension(['cxx', 'common', 'rng']),
        make_extension(['cxx', 'common', '_rng']),
        make_extension(['cxx', 'mixture', 'model']),
        make_extension(['cxx', 'mixture', '_model']),
    ])

setup(
    #cmdclass = {'build_ext': build_ext},
    ext_modules = extensions,
)
