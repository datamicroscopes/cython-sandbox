from mixturemodel cimport mixturemodel_state
from dataview cimport dataview

cdef extern from "kernel.hpp" namespace "gibbs":
    void assign(mixturemodel_state &, dataview &)
