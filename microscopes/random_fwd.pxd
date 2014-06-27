cdef extern from "<random>" namespace "std":
    cdef cppclass default_random_engine:
        default_random_engine(int) except +
        void seed(int) except +

cdef extern from "microscopes/random_fwd.hpp" namespace "microscopes":
    ctypedef default_random_engine rng_t
