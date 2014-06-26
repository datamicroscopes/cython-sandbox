from libcpp.vector cimport vector
from libcpp.string cimport string
from libc.stdint import size_t

from random_fwd cimport rng_t
from type_info cimport runtime_type_info
from component cimport hyperparam_bag_t
from dataview cimport row_accessor, row_mutator

cdef extern from "mixturemodel.hpp":
    cdef cppclass mixturemodel_state:
        mixturemodel_state(size_t, hyperparam_bag_t, vector[string] &, vector[hyperparam_bag_t] &) except +
        void ensure_k_empty_groups(size_t, rng_t &) except +
        vector[runtime_type_info] get_runtime_type_info() except +
        void sample_post_pred(row_accessor &, row_mutator &, rng_t &) except +	
