from libcpp.vector cimport vector
from libcpp.utility cimport pair
from libc.stddef cimport size_t

from type_info cimport runtime_type_info

cdef extern from "microscopes/type_helper.hpp" namespace "microscopes::runtime_type_traits":
    pair[vector[size_t], size_t] GetOffsetsAndSize(vector[runtime_type_info] &) except +
