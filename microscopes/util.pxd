from mixturemodel cimport mixturemodel_state
from dataview cimport dataview

cdef extern from "util.hpp" namespace "util":
    void stupid_bootstrap(mixturemodel_state &, dataview &) except +
