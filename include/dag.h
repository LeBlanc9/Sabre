#pragma once
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/graphviz.hpp>
#include <graphviz/gvc.h>

struct InstructionNode {
    std::string name = "NoName";  // 门操作的名称
    std::vector<int> qubit_pos;
    //std::unordered_map<int, int> measure_pos = {};
    std::vector<int> classic_pos;


    InstructionNode();
    InstructionNode(const std::string& name);
    InstructionNode(const std::string& operation, std::vector<int> pos);
};

struct EdgeProperties {
public:
    int qubit_id; // 1,2,3 for qubits 
    std::string name = "NoName";

    EdgeProperties();
    EdgeProperties(int qubit_id);
};

using DagGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, InstructionNode, EdgeProperties, boost::bidirectionalS>;


class DAGCircuit {

public:
    DagGraph graph;

    DAGCircuit();
    DAGCircuit(DagGraph& graph);
    
    void add_node(InstructionNode node);
    void add_edge(int from, int to, EdgeProperties ep);
    int get_num_nodes();    // return num of gates.
    int get_qubits_used();

    void draw_self();
};

DagGraph reverse_DagGraph(const DagGraph& graph);