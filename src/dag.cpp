#include <set>
#include "dag.h"
#include "vis.h"
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_utility.hpp>


//* Struct InstructionNode
InstructionNode::InstructionNode() {}
InstructionNode::InstructionNode(const std::string& name) : name(name) {}
InstructionNode::InstructionNode(const std::string& name, std::vector<int> qubit_pos) : name(name), qubit_pos(qubit_pos) {}

//* Struct Edge
EdgeProperties::EdgeProperties() : qubit_id(-1) {}
EdgeProperties::EdgeProperties(int qubit_id) : qubit_id(qubit_id) {}


//* Class DAGCircuit
DAGCircuit::DAGCircuit() {}
DAGCircuit::DAGCircuit(DagGraph& graph) : graph(graph) {}
void DAGCircuit::add_node(InstructionNode node) { boost::add_vertex(node.name, graph);}
void DAGCircuit::add_edge(int from, int to, EdgeProperties ep) { boost::add_edge(from, to, ep, graph); }
int  DAGCircuit::get_num_nodes() const { return boost::num_vertices(graph); }
int  DAGCircuit::get_qubits_used() const {
    int qubits_used = 0;
    // 遍历所有边
    std::set<int> qubits_id_set;      
    DagGraph::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei)
        qubits_id_set.insert(graph[*ei].qubit_id);

    return qubits_id_set.size();
}

void DAGCircuit::draw_self() const { draw_dot(graph_to_dot(graph)); }
void DAGCircuit::print_self() const { print_graph(graph); }

//* Utils Function
DagGraph reverse_DagGraph(const DagGraph& graph) {
    DagGraph rev_graph;
    boost::copy_graph(boost::make_reverse_graph(graph), rev_graph);
    return rev_graph;
}