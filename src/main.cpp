#include <mbgl/map/map.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;

// TODO: destructor
class PyMap {
public:
    static PyMap create() {
        return PyMap();
        // return Example(a);

    }
};






int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(pymbgl, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function which adds two numbers");

    py::class_<PyMap>(m, "Map")
        .def(py::init(&PyMap::create))

    ;


}


