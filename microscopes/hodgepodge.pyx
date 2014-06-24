from dataview cimport row_major_dataview

cdef class numpy_dataview:
    cdef row_major_dataview *thisptr
    def __cinit__(self, np.ndarray npd):
        self._npd = npd # bump refcount
    def __dealloc__(self):
        del self.thisptr
