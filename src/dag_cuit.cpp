#include "dag_cuit.h"
#include "vis.h"


//* Struct InstructionNode
InstructionNode::InstructionNode() {}
InstructionNode::InstructionNode(int id, const std::string& name) : id(id), name(name) {}
InstructionNode::InstructionNode(int id, const std::string& name, std::vector<int> pos) : id(id), name(name), pos(pos) {}

//* Class Edge
EdgeProperties::EdgeProperties() {}
EdgeProperties::EdgeProperties(int id) : id(id) {}


//* Class DAGCircuit
DAGCircuit::DAGCircuit() {}
DAGCircuit::DAGCircuit(Graph& graph) : graph(graph) {}
void DAGCircuit::add_node(InstructionNode node) { boost::add_vertex(node, graph);}
void DAGCircuit::add_edge(int from, int to, EdgeProperties ep) { boost::add_edge(from, to, ep, graph); }
int  DAGCircuit::get_num_nodes() { return boost::num_vertices(graph); }
void DAGCircuit::draw_self() { draw_graph(this->graph); }


//* Utils Function
Graph reverse_dag(Graph& graph) {
    Graph rev_graph;
    // TODO:
    return rev_graph;
}