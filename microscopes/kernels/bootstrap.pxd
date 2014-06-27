from mixturemodel cimport mixturemodel_state
from dataview cimport dataview
from random_fwd cimport rng_t

cdef extern from "microscopes/kernels.hpp" namespace "microscopes::bootstrap":
    void likelihood(mixturemodel_state &, dataview &, rng_t &) except +
