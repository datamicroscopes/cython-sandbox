#!/usr/bin/env python

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import numpy

setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = [Extension("microscopes.hodgepodge",
                             sources=["microscopes/hodgepodge.pyx"],
                             libraries=["microscopes"],
                             language="c++",
                             include_dirs=[numpy.get_include(), 'include', 'microscopes'],
                             extra_compile_args=['-std=c++0x'])],
)
