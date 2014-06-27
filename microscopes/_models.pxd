from libcpp.string cimport string
from libcpp.vector cimport vector
from type_info cimport runtime_type_info
from shared_ptr cimport shared_ptr

cdef extern from "microscopes/models.hpp" namespace "microscopes":
    ctypedef string hyperparam_bag_t 
    ctypedef string suffstats_bag_t

    cdef cppclass model:
        hyperparam_bag_t get_hp() except +
        void set_hp(hyperparam_bag_t &) except +
        vector[runtime_type_info] get_runtime_type_info() except +

    cdef cppclass distributions_factory[T]:
        distributions_factory()
        shared_ptr[model] new_instance() except +

cdef class _c_model:
    cdef shared_ptr[model] new_cmodel(self)

cdef extern from "distributions/models/bb.hpp" namespace "distributions":
    cdef cppclass BetaBernoulli:
        pass

cdef extern from "distributions/models/bnb.hpp" namespace "distributions":
    cdef cppclass BetaNegativeBinomial:
        pass

cdef extern from "distributions/models/gp.hpp" namespace "distributions":
    cdef cppclass GammaPoisson:
        pass

cdef extern from "distributions/models/nich.hpp" namespace "distributions":
    cdef cppclass NormalInverseChiSq:
        pass
