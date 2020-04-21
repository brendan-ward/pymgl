cdef extern from "mbgl/map/map.hpp" namespace "mbgl":
    cdef cppclass Map:
        Map(void *, void *, void *, void *)
        setStyle(void *)
        renderStill(void *, void *, void*)

cdef class PyMap:
    cdef Map *_ptr