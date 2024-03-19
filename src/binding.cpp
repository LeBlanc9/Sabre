#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "backend.h"
#include "vis.h"
#include "model.h"
#include "type.h"
#include "sabre_layout.h"
#include "sabre_routing.h"
#include "dag_cuit.h"


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


    py::class_<InstructionNode>(m, "InstructionNode")
        .def(py::init<>())
        .def(py::init<int ,const std::string>())
        .def_readwrite("name", &InstructionNode::name)
        .def_readwrite("id", &InstructionNode::id)
        .def_readwrite("pos", &InstructionNode::pos)
        .def_readwrite("measure_pos", &InstructionNode::measure_pos);

    py::class_<EdgeProperties>(m, "EdgeProperties")
        .def(py::init<>())
        .def(py::init<int>())
        .def_readwrite("id", &EdgeProperties::id);

    py::class_<DAGCircuit>(m, "DAGCircuit")
        .def(py::init<>())
        .def("add_node", &DAGCircuit::add_node)
        .def("add_edge", &DAGCircuit::add_edge)
        .def("get_num_nodes", &DAGCircuit::get_num_nodes)
        .def("draw_self", &DAGCircuit::draw_self)
        .def_readwrite("graph", &DAGCircuit::graph);

    //m.def("draw_graph", &draw_graph, "Draw a Graph object");
}
