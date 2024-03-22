#include "dag_cuit.h"
#include "vis.h"


//* Struct InstructionNode
InstructionNode::InstructionNode() {}
InstructionNode::InstructionNode(const std::string& name) : name(name) {}
InstructionNode::InstructionNode(const std::string& name, std::vector<int> qubit_pos) : name(name), qubit_pos(qubit_pos) {}

//* Struct Edge
EdgeProperties::EdgeProperties() {}
EdgeProperties::EdgeProperties(int id) : id(id) {}


//* Class DAGCircuit
DAGCircuit::DAGCircuit() {}
DAGCircuit::DAGCircuit(DagGraph& graph) : graph(graph) {}
void DAGCircuit::add_node(InstructionNode node) { boost::add_vertex(node, graph);}
void DAGCircuit::add_edge(int from, int to, EdgeProperties ep) { boost::add_edge(from, to, ep, graph); }
int  DAGCircuit::get_num_nodes() { return boost::num_vertices(graph); }
void DAGCircuit::draw_self() { draw_dot(graph_to_dot(graph)); }


//* Utils Function
DagGraph reverse_dag(DagGraph& graph) {
    DagGraph rev_graph;
    // TODO:
    return rev_graph;
}