#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <sabre.h>
#include <type.h>
#include "sabre_routing.cpp"
#include "backend.h"
#include "model.h"


PYBIND11_MODULE(sabre, m) {
    m.doc() = "For Sabre"; // optional module docstring

    py::class_<SabreRouting>(m, "SabreRouting")
        .def(py::init<>()) 
        .def("set_model", &SabreRouting::set_model, "Set Model")
        .def("get_model", &SabreRouting::get_model)
        .def("run", &SabreRouting::run );


    py::class_<Backend>(m, "Backend")
        .def(py::init<std::vector<std::tuple<int, int, float>>>())
        .def(py::init<>());
        


    py::class_<Model>(m, "Model")
        .def(py::init<Backend&>());
    }
