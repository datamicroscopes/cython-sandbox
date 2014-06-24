
cdef extern from "kernel.hpp" namespace "gibbs":
    void assign(mixturemodel_state &, dataview &)
