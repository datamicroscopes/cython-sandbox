from libcpp.map cimport map
from libcpp.string cimport string

cdef extern from "component.hpp":
    ctypedef map[string, float] hyperparam_t
