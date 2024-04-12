#pragma once
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/graphviz.hpp>
#include <graphviz/gvc.h>

struct InstructionNode 
{
public:
    std::string name = "NoName";  // 门操作的名称
    std::vector<int> qubit_pos;
    //std::unordered_map<int, int> measure_pos = {};
    std::vector<int> classic_pos;


    InstructionNode();
    InstructionNode(const std::string& name);
    InstructionNode(const std::string& name, std::vector<int> pos);

    friend std::ostream& operator<< (std::ostream& os, const InstructionNode& node);
};

// TODO:
// struct MesureNode : public InstructionNode 
// {
// public:
//     const std::string name = "measure";
//     std::unordered_map<int, int> measure_pos = {};
// };



struct EdgeProperties 
{
public:
    int qubit_id; // 1,2,3 for qubits 

    EdgeProperties();
    EdgeProperties(int qubit_id);
};

using DagGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, InstructionNode, EdgeProperties, boost::bidirectionalS>;


class DAGCircuit 
{
public:
    DagGraph graph;

    DAGCircuit();
    DAGCircuit(DagGraph& graph);
    
    void add_node(const InstructionNode& node);
    void add_edge(int from, int to, EdgeProperties ep);
    int get_num_nodes() const;    // return num of gates.

    std::set<int> get_qubits_used() const {
        std::set<int> qubits_id_set;      
        DagGraph::edge_iterator ei, ei_end;
        for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei)
            qubits_id_set.insert(graph[*ei].qubit_id);
        return qubits_id_set;
    }


    // 获取某个节点的后继节点&前置节点
    std::pair<std::vector<int>, std::vector<int>> get_related_node(int node_index) const {
        std::vector<int> predecessors;
        std::vector<int> successors;

        DagGraph::edge_iterator ei, ei_end;
        for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei) {
            if (boost::target(*ei, graph) == node_index) {
                predecessors.push_back(boost::source(*ei, graph));
                successors.push_back(boost::target(*ei, graph));
            }
        }
        return std::make_pair(predecessors, successors);
    }

    std::vector<int> get_predecessors_of_node(int node_index) const {
        std::vector<int> predecessors;
        DagGraph::edge_iterator ei, ei_end;
        for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei) {
            if (boost::target(*ei, graph) == node_index)
                predecessors.push_back(boost::source(*ei, graph));
        }
        return predecessors;
    }

    std::vector<int> get_successors_of_node(int node_index) const {
        std::vector<int> successors;
        DagGraph::edge_iterator ei, ei_end;
        for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei) {
            if (boost::target(*ei, graph) == node_index)
                successors.push_back(boost::target(*ei, graph));
        }
        return successors;
    } 



    std::vector<int> get_pre; 

   
    void draw_self() const;
    void print_self() const;
};



DagGraph reverse_DagGraph(const DagGraph& graph);