from libcpp.vector cimport vector
from libcpp cimport bool
from libc.stdint cimport uint8_t
cimport type_info as ti

cdef extern from "dataview.hpp":
    cdef cppclass dataview:
        pass

    cdef cppclass row_major_dataview(dataview):
        row_major_dataview(uint8_t *, bool *, size_t, vector[ti.runtime_type_info] &) except +
