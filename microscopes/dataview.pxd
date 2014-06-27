from libcpp.vector cimport vector
from libcpp cimport bool as cbool
from libc.stdint cimport uint8_t
from libc.stddef cimport size_t

from random_fwd cimport rng_t
from type_info cimport runtime_type_info

cdef extern from "microscopes/dataview.hpp" namespace "microscopes":
    cdef cppclass row_accessor:
        row_accessor(uint8_t *, cbool *, vector[runtime_type_info] *, vector[size_t] *)

    cdef cppclass row_mutator:
        row_mutator(uint8_t *, vector[runtime_type_info] *, vector[size_t] *)

    cdef cppclass dataview:
        pass

    cdef cppclass row_major_dataview(dataview):
        row_major_dataview(uint8_t *, cbool *, size_t, vector[runtime_type_info] &) except +
        void permute(rng_t &) except +
        void reset_permutation() 
