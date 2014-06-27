from libcpp.vector cimport vector
from libcpp.string cimport string
from libc.stdint import size_t

from random_fwd cimport rng_t
from type_info cimport runtime_type_info
from _models cimport hyperparam_bag_t, model
from dataview cimport row_accessor, row_mutator
from shared_ptr cimport shared_ptr

cdef extern from "microscopes/mixturemodel.hpp" namespace "microscopes":
    cdef cppclass mixturemodel_state:
        mixturemodel_state(size_t, vector[shared_ptr[model]] &) except +

        hyperparam_bag_t get_hp() except +
        void set_hp(hyperparam_bag_t &) except +
        hyperparam_bag_t get_feature_hp(size_t) except +
        void set_feature_hp(size_t, hyperparam_bag_t &) except +

        void ensure_k_empty_groups(size_t, rng_t &) except +
        void sample_post_pred(row_accessor &, row_mutator &, rng_t &) except +	
        vector[runtime_type_info] get_runtime_type_info() except +
