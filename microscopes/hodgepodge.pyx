from libcpp.vector cimport vector
from libcpp.string cimport string
from libc.stdint cimport uint8_t

from dataview cimport dataview, row_major_dataview
from component cimport hyperparam_t
from mixturemodel cimport mixturemodel_state
from kernel cimport assign
from kernel cimport bootstrap as _bootstrap

cimport numpy as np
cimport type_info as ti

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
        return ti.TYPE_INFO_I16
    if tpe == np.float32:
        return ti.TYPE_INFO_F32
    if tpe in (float, np.float, np.float64):
        return ti.TYPE_INFO_F64
    raise Exception("Unknown type: " + tpe)

cdef class abstract_dataview:
    cdef dataview *_thisptr
    def __cinit__(self):
        pass
    def __dealloc__(self):
        del self._thisptr

cdef class numpy_dataview(abstract_dataview):
    cdef np.ndarray _npd
    def __cinit__(self, np.ndarray npd):
        self._npd = npd # bump refcount
        n = npd.shape[0]
        cdef vector[ti.runtime_type_info] ctypes
        l = len(npd.dtype)
        for i in xrange(l):
            ctypes.push_back(_get_c_type(npd.dtype[i]))
        self._thisptr = new row_major_dataview(<uint8_t *> npd.data, n, ctypes)

cdef hyperparam_t _get_c_hyperparam(hp):
    cdef hyperparam_t ret
    for k, v in hp.iteritems():
        ret[k] = float(v)
    return ret

cdef class mixturemodel:
    cdef mixturemodel_state *_thisptr
    def __cinit__(self, n, clusterhp, models, hps):
        cdef vector[string] factories 
        cdef vector[hyperparam_t] hyperparams
        for m in models:
            factories.push_back(str(m))
        for hp in hps:
            hyperparams.push_back(_get_c_hyperparam(hp))
        self._thisptr = new mixturemodel_state(n, _get_c_hyperparam(clusterhp), factories, hyperparams)
    def __dealloc__(self):
        del self._thisptr

def bootstrap(mixturemodel mm, abstract_dataview view):
    _bootstrap(mm._thisptr[0], view._thisptr[0])

def gibbs_assign(mixturemodel mm, abstract_dataview view):
    assign(mm._thisptr[0], view._thisptr[0])
