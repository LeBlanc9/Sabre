#pragma once
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <graphviz/gvc.h>


class InstructionNode {
public:
    std::string name;  // 门操作的名称

    InstructionNode() : name("NoNmae") {}
    InstructionNode(const std::string& name) : name(name){}

    void print_name() { std::cout << this->name << std::endl; }
};


using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    InstructionNode>;


class DAGCircuit {

public:
    Graph graph;

    DAGCircuit() : graph() {}
    DAGCircuit(Graph& graph) : graph(graph) {}
    
    void add_node(InstructionNode node) { boost::add_vertex(node, graph);}
    void add_edge(int from, int to) { boost::add_edge(from, to, graph); }
    int get_num_vertices() { return boost::num_vertices(graph); }
};

