cdef extern from "<random>" namespace "std":
    cdef cppclass default_random_engine:
        default_random_engine(int) except +
        void seed(int) except +

cdef extern from "random_fwd.hpp":
    ctypedef default_random_engine rng_t
