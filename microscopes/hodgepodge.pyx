from libcpp.vector cimport vector
from libcpp.utility cimport pair
from libcpp cimport bool as cbool
from libcpp.string cimport string
from libc.stdint cimport uint8_t 
from libc.stddef cimport size_t

from dataview cimport dataview, row_major_dataview, row_accessor, row_mutator
from _models cimport hyperparam_bag_t, suffstats_bag_t, model, _c_model
from random_fwd cimport rng_t as _rng_t
from mixturemodel cimport mixturemodel_state
from kernels.gibbs cimport assign as _gibbs_assign
from kernels.bootstrap cimport likelihood as _bootstrap_likelihood
from shared_ptr cimport shared_ptr
from type_helper cimport GetOffsetsAndSize

import numpy as np
cimport numpy as np
cimport type_info as ti

cdef class rng_t:
    cdef _rng_t *_thisptr
    def __cinit__(self, seed=12345):
        self._thisptr = new _rng_t(seed)
    def __dealloc__(self):
        del self._thisptr

def _get_c_type(tpe):
    if tpe in (bool, np.bool):
        return ti.TYPE_INFO_B
    if tpe == np.int8:
        return ti.TYPE_INFO_I8
    if tpe == np.int16:
        return ti.TYPE_INFO_I16
    if tpe == np.int32:
        return ti.TYPE_INFO_I32
    if tpe in (int, np.int, np.int64):
        return ti.TYPE_INFO_I64
    if tpe == np.float32:
        return ti.TYPE_INFO_F32
    if tpe in (float, np.float, np.float64):
        return ti.TYPE_INFO_F64
    raise Exception("Unknown type: " + tpe)

def _get_np_type(tpe):
    if tpe == ti.TYPE_INFO_B:
        return np.bool
    if tpe == ti.TYPE_INFO_I8:
        return np.int8
    if tpe == ti.TYPE_INFO_I16:
        return np.int16
    if tpe == ti.TYPE_INFO_I32:
        return np.int32
    if tpe == ti.TYPE_INFO_I64:
        return np.int64
    if tpe == ti.TYPE_INFO_F32:
        return np.float32
    if tpe == ti.TYPE_INFO_F64:
        return np.float64
    raise Exception("unknown type: " + tpe)

cdef class abstract_dataview:
    cdef dataview *_thisptr
    def __cinit__(self):
        pass
    def __dealloc__(self):
        del self._thisptr

cdef vector[ti.runtime_type_info] _get_c_types(np.ndarray npd):
    cdef vector[ti.runtime_type_info] ctypes
    l = len(npd.dtype)
    for i in xrange(l):
        ctypes.push_back(_get_c_type(npd.dtype[i]))
    return ctypes

cdef class numpy_dataview(abstract_dataview):
    cdef np.ndarray _npd
    def __cinit__(self, np.ndarray npd):
        self._npd = np.ascontiguousarray(npd) # bump refcount
        n = npd.shape[0]
        cdef vector[ti.runtime_type_info] ctypes
        ctypes = _get_c_types(npd)
        cdef np.ndarray npd_mask
        if hasattr(npd, 'mask'):
            npd_mask = np.ascontiguousarray(npd.mask)
            self._thisptr = new row_major_dataview( <uint8_t *> npd.data, <cbool *> npd_mask.data, n, ctypes)
        else:
            self._thisptr = new row_major_dataview( <uint8_t *> npd.data, NULL, n, ctypes)
    def permute(self, rng_t rng):
        (<row_major_dataview *>self._thisptr)[0].permute(rng._thisptr[0])
        return self
    def reset_permutation(self):
        (<row_major_dataview *>self._thisptr)[0].reset_permutation()

cdef class mixturemodel:
    cdef mixturemodel_state *_thisptr
    def __cinit__(self, n, list models):
        cdef vector[shared_ptr[model]] cmodels
        for py_m, c_m in models:
            cmodels.push_back((<_c_model>c_m).new_cmodel())
        self._thisptr = new mixturemodel_state(n, cmodels)
    def __dealloc__(self):
        del self._thisptr

    def sample_post_pred(self, np.ndarray inp, rng_t rng, size=1):
        ret = [self._sample_post_pred_one(inp, rng) for _ in xrange(size)]
        return np.hstack(ret)

    def _sample_post_pred_one(self, np.ndarray inp, rng_t rng):
        cdef np.ndarray inp_data = None
        cdef np.ndarray inp_mask = None
        cdef vector[ti.runtime_type_info] inp_ctypes
        cdef vector[ti.runtime_type_info] out_ctypes

        if hasattr(inp, 'mask'):
            inp_data = np.ascontiguousarray(inp.data)
            inp_mask = np.ascontiguousarray(inp.mask)
        else:
            inp_data = np.ascontiguousarray(inp.data)

        inp_ctypes = _get_c_types(inp)
        cdef pair[vector[size_t], size_t] inp_ret     
        inp_ret = GetOffsetsAndSize(inp_ctypes)
        cdef vector[size_t] *inp_offsets = &inp_ret.first

        # build row_accessor
        # XXX: can we stack allocate?
        cdef row_accessor *acc = new row_accessor( 
            <uint8_t *> inp_data.data, 
            <cbool *> inp_mask.data if inp_mask is not None else NULL,
            &inp_ctypes,
            inp_offsets)

        # ensure the state has 1 empty group
        self._thisptr[0].ensure_k_empty_groups(1, rng._thisptr[0])     

        out_ctypes = self._thisptr[0].get_runtime_type_info()
        out_dtype = []
        for t in out_ctypes:
            out_dtype.append(('', _get_np_type(t)))

        # build an appropriate numpy array to store the output
        cdef np.ndarray out_npd = np.zeros(1, dtype=out_dtype)

        # construct the output offsets
        cdef pair[vector[size_t], size_t] out_ret     
        out_ret = GetOffsetsAndSize(out_ctypes)
        cdef vector[size_t] *out_offsets = &out_ret.first

        cdef row_mutator *mut = new row_mutator(
            <uint8_t *> out_npd.data,
            &out_ctypes,
            out_offsets)

        self._thisptr[0].sample_post_pred(acc[0], mut[0], rng._thisptr[0])

        del acc
        del mut
        return out_npd

def bootstrap_likelihood(mixturemodel mm, abstract_dataview view, rng_t rng):
    _bootstrap_likelihood(mm._thisptr[0], view._thisptr[0], rng._thisptr[0])

def gibbs_assign(mixturemodel mm, abstract_dataview view, rng_t rng):
    _gibbs_assign(mm._thisptr[0], view._thisptr[0], rng._thisptr[0])
