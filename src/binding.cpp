#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "backend.h"
#include "vis.h"
#include "model.h"
#include "coupling.h"
#include "sabre_layout.h"
#include "sabre_routing.h"
#include "dag.h"
#include "layout.h"


namespace py = pybind11;

PYBIND11_MODULE(sabre, m) {
    m.doc() = "For Sabre"; // optional module docstring

    py::class_<SabreLayout>(m, "SabreLayout")
        .def(py::init<const CouplingCircuit&>()) 
        .def("run", &SabreLayout::run );

     py::class_<SabreRouting>(m, "SabreRouting")
        .def(py::init<const CouplingCircuit&>()) 
        .def("set_model", &SabreRouting::set_model)
        .def("get_model", &SabreRouting::get_model)
        .def("run", &SabreRouting::run)
        .def_readwrite("modify_dag", &SabreRouting::modify_dag);

    py::class_<Backend>(m, "Backend")
        .def(py::init<CouplingList>())
        .def(py::init<>());

    py::class_<Model>(m, "Model")
        .def(py::init<>())
        .def(py::init<Backend>())
        .def_readwrite("init_layout", &Model::init_layout);


    py::class_<CouplingCircuit>(m, "CouplingCircuit")
        .def(py::init<CouplingList>())
        .def("update_num_qubits", &CouplingCircuit::update_num_qubits)
        .def("get_distance_matrix", &CouplingCircuit::get_distance_matrix)
        .def("draw_self", &CouplingCircuit::draw_self)
        .def("print_self", &CouplingCircuit::print_self);

    py::class_<Layout>(m, "Layout")
        .def(py::init<>())
        .def(py::init<LayoutStructure>())
        .def("__getitem__", &Layout::operator[]);

    py::class_<InstructionNode>(m, "InstructionNode")
        .def(py::init<>())
        .def(py::init<const std::string>())
        .def_readwrite("name", &InstructionNode::name)
        .def_readwrite("qubit_pos", &InstructionNode::qubit_pos)
        .def_readwrite("classic_pos", &InstructionNode::classic_pos);

    py::class_<EdgeProperties>(m, "EdgeProperties")
        .def(py::init<>())
        .def(py::init<int>())
        .def_readwrite("id", &EdgeProperties::qubit_id);

    py::class_<DAGCircuit>(m, "DAGCircuit")
        .def(py::init<>())
        .def("add_node", &DAGCircuit::add_node)
        .def("add_edge", &DAGCircuit::add_edge)
        .def("get_num_nodes", &DAGCircuit::get_num_nodes)
        .def("draw_self", &DAGCircuit::draw_self)
        .def("print_self", &DAGCircuit::print_self)
        .def_readwrite("graph", &DAGCircuit::graph);

    //m.def("draw_graph", &draw_graph, "Draw a Graph object");
}
