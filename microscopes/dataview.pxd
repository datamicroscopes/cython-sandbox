from libcpp.vector cimport vector
from libc.stdint cimport uint8_t
cimport type_info as ti

cdef extern from "dataview.hpp":
    cdef cppclass row_accessor:
        row_accessor()
        row_accessor(uint8_t *, vector[ti.runtime_type_info] *, vector[size_t] *) except +

    cdef cppclass dataview:
        pass

    cdef cppclass row_major_dataview(dataview):
        row_major_dataview(uint8_t *, size_t, vector[ti.runtime_type_info] &) except +
