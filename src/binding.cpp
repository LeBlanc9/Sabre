#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <sabre.h>
#include "sabre_routing.cpp"

namespace py = pybind11;

PYBIND11_MODULE(sabre, m) {
    m.doc() = "For Sabre"; // optional module docstring

    py::class_<SabreRouting>(m, "SabreRouting")
        .def(py::init<Model>()) 
        .def("set_model", &SabreRouting::set_model)
        .def("get_model", &SabreRouting::get_model)
        .def("run", &SabreRouting::run );


    py::class_<Backend>(m, "Backend")
        .def(py::init<CouplingList>())
        .def(py::init<>());
        

    py::class_<Model>(m, "Model")
        .def(py::init<Backend>());
    }
