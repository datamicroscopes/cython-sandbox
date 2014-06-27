from microscopes.cxx._models_h cimport distributions_factory
from microscopes.cxx._models_h cimport BetaBernoulli as c_bb

cdef class factory:
    cdef shared_ptr[model] new_cmodel(self):
        # XXX: cython does not support virtual abstract classes :(
        raise Exception("Abstract class")

cdef class bb_factory(factory):
    cdef shared_ptr[model] new_cmodel(self):
        cdef distributions_factory[c_bb] f = distributions_factory[c_bb]()
        return f.new_instance()

