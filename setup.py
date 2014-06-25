#!/usr/bin/env python

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import numpy
import sys

clang = False
if sys.platform.lower().startswith('darwin'):
    clang = True

extra_compile_args = ['-std=c++0x']
if clang:
    extra_compile_args.extend([
        '-mmacosx-version-min=10.7',  # for anaconda
        '-std=c++0x',
        '-stdlib=libc++',
    ])

setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = [Extension("microscopes.hodgepodge",
                             sources=["microscopes/hodgepodge.pyx"],
                             libraries=["microscopes"],
                             language="c++",
                             include_dirs=[numpy.get_include(), 'include', 'microscopes'],
                             extra_compile_args=extra_compile_args)],
)
