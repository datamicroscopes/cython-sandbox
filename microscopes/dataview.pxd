from libcpp.vector cimport vector
from libcpp cimport bool
from libc.stdint cimport uint8_t
from libc.stddef cimport size_t

from random_fwd cimport rng_t
from type_info cimport runtime_type_info

cdef extern from "dataview.hpp":
    cdef cppclass row_accessor:
        row_accessor(uint8_t *, bool *, vector[runtime_type_info] *, vector[size_t] *)

    cdef cppclass row_mutator:
        row_mutator(uint8_t *, vector[runtime_type_info] *, vector[size_t] *)

    cdef cppclass dataview:
        pass

    cdef cppclass row_major_dataview(dataview):
        row_major_dataview(uint8_t *, bool *, size_t, vector[runtime_type_info] &) except +
        void permute(rng_t &) except +
        void reset_permutation() 
