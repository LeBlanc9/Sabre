#pragma once
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <graphviz/gvc.h>

struct InstructionNode {
    int id;  // -1=start; -2=end; -3=measure
    std::string name = "NoName";  // 门操作的名称
    std::vector<int> pos;
    std::unordered_map<int, int> measure_pos = {};


    InstructionNode();
    InstructionNode(int label, const std::string& name);
    InstructionNode(int label, const std::string& operation, std::vector<int> pos);
};

struct EdgeProperties {
public:
    int id; // 1,2,3 for qubits 
    std::string name = "NoName";

    EdgeProperties();
    EdgeProperties(int id);
};

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    InstructionNode,
                                    EdgeProperties>;


class DAGCircuit {

public:
    Graph graph;

    DAGCircuit();
    DAGCircuit(Graph& graph);
    
    void add_node(InstructionNode node);
    void add_edge(int from, int to, EdgeProperties ep);
    int get_num_nodes(); 

    void draw_self();
};

Graph reverse_dag(Graph& graph);