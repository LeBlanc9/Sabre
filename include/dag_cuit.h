#pragma once
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <graphviz/gvc.h>

struct InstructionNode {
    int label;  // -1=start; -2=end; -3=measure
    std::string name = "name";  // 门操作的名称
    std::vector<int> pos;
    std::unordered_map<int, int> measure_pos = {};


    InstructionNode();
    InstructionNode(int label, const std::string& name);
    InstructionNode(int label, const std::string& operation, std::vector<int> pos);
};

class Edge {
public:
    std::string name = "NoName";

    Edge();
};

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    InstructionNode>;


class DAGCircuit {

public:
    Graph graph;

    DAGCircuit();
    DAGCircuit(Graph& graph);
    
    void add_node(InstructionNode node);
    void add_edge(int from, int to); 
    int get_num_vertices(); 

    void draw_self();
};

Graph reverse_dag(Graph& graph);