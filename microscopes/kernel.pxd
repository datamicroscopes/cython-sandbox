from mixturemodel cimport mixturemodel_state
from dataview cimport dataview
from random_fwd cimport rng_t

cdef extern from "kernel.hpp" namespace "gibbs":
    void assign(mixturemodel_state &, dataview &, rng_t &) except +
    void bootstrap(mixturemodel_state &, dataview &, rng_t &) except +
