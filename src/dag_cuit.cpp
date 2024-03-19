#include "dag_cuit.h"
#include "vis.h"


//* Struct InstructionNode
InstructionNode::InstructionNode() {}
InstructionNode::InstructionNode(int label, const std::string& name) : label(label), name(name) {}
InstructionNode::InstructionNode(int label, const std::string& name, std::vector<int> pos) : label(label), name(name), pos(pos) {}

//* Class Edge
Edge::Edge() {}


//* Class DAGCircuit
DAGCircuit::DAGCircuit() {}
DAGCircuit::DAGCircuit(Graph& graph) : graph(graph) {}
void DAGCircuit::add_node(InstructionNode node) { boost::add_vertex(node, graph);}
void DAGCircuit::add_edge(int from, int to) { boost::add_edge(from, to, graph); }
int  DAGCircuit::get_num_vertices() { return boost::num_vertices(graph); }
void DAGCircuit::draw_self() { draw_graph(this->graph); }


//* Utils Function
Graph reverse_dag(Graph& graph) {
    Graph rev_graph;
    // TODO:
    return rev_graph;
}