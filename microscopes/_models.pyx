from _models cimport _c_model, distributions_factory
from _models cimport BetaBernoulli as c_bb
from shared_ptr cimport shared_ptr

cdef class _c_model:
    cdef shared_ptr[model] new_cmodel(self):
        # XXX: cython does not support virtual abstract classes :(
        raise Exception("Abstract class")

cdef class _bb_c_model(_c_model):
    cdef shared_ptr[model] new_cmodel(self):
        cdef distributions_factory[c_bb] factory = distributions_factory[c_bb]()
        return factory.new_instance()
