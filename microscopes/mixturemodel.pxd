from libcpp.vector cimport vector
from libcpp.string cimport string

from component cimport hyperparam_t

cdef extern from "mixturemodel.hpp":
    cdef cppclass mixturemodel_state:
        mixturemodel_state(size_t, vector[string] &, vector[hyperparam_t] &) except +
