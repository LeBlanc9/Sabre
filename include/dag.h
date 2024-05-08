#pragma once
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/graphviz.hpp>
#include <graphviz/gvc.h>
#include "gate_name.h"


using qubit_t = int;
using cbit_t = int;
using node_pos_t = int;
using edge_pos_t = std::pair<node_pos_t, node_pos_t>;


struct InstructionNode 
{
public:
    std::string name = "NoName"; 
    std::vector<qubit_t> qubit_pos;
    std::vector<cbit_t> classic_pos;

    InstructionNode();
    InstructionNode(const std::string& name);
    InstructionNode(const std::string& name, std::vector<int> pos);

    friend std::ostream& operator<< (std::ostream& os, const InstructionNode& node);
};


struct EdgeProperties 
{
public:
    qubit_t qubit_id; // 1,2,3 for qubits 

    EdgeProperties();
    EdgeProperties(qubit_t qubit_id);
};


// using DagGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, InstructionNode, EdgeProperties>;
using DagGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, InstructionNode, EdgeProperties>;


class DAGCircuit
{
public:
    DagGraph graph;

public:
    DAGCircuit();
    DAGCircuit(DagGraph& graph);
    
    int add_node(const InstructionNode& node) {
        auto vertex_description = boost::add_vertex(node, graph);
        return vertex_description;
    }
    void add_edge(int from, int to, const EdgeProperties& ep) {
        boost::add_edge(from, to, ep, graph); 
    }
    int get_num_nodes() const {
        return boost::num_vertices(graph); 
    }

    std::set<int> get_qubits_used() const {
        std::set<int> qubits_id_set;      
        DagGraph::edge_iterator ei, ei_end;
        for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei)
            qubits_id_set.insert(graph[*ei].qubit_id);
        return qubits_id_set;
    }


    void add_instruction_node_end(const InstructionNode& node) {
        if (is_start_exist() && is_end_exist()) {
            std::map<qubit_t, int> inf_predecessors = node_qubits_predecessors(1);
            std::map<qubit_t, int> gate_predecessors = {};
            std::map<qubit_t, int> gate_successors= {};
            for (const auto& qubit: node.qubit_pos) {
                if (inf_predecessors.find(qubit) == inf_predecessors.end()) {
                    gate_predecessors[qubit] = 0; // 0 is the start node
                } else {
                    gate_predecessors[qubit] = inf_predecessors[qubit];
                }
            }
            for (const auto& qubit: node.qubit_pos) {
                gate_successors[qubit] = 1; // 1 is the end node
            } 
            add_instruction_node(node, gate_predecessors, gate_successors);

        } else {
            add_node(InstructionNode("start"));
            add_node(InstructionNode("end"));
            add_node(node);
            for (const auto& qubit: node.qubit_pos) {
                add_edge(0, 2, EdgeProperties{qubit});
                add_edge(2, 1, EdgeProperties{qubit});
            }
        }
    }

    void add_instruction_node(  const InstructionNode& node, 
                                std::map<qubit_t, node_pos_t> predecessors, 
                                std::map<qubit_t, node_pos_t> successors) {
        // 1. 移除原本连接着的边                                
        std::set<edge_pos_t> qubits_pre_out_edges = {};
        std::set<edge_pos_t> qubits_suc_in_edges = {};
        for (const auto& qubit: node.qubit_pos) {
            std::set<int> qubits_used = get_qubits_used();
            if  (qubits_used.find(qubit) != qubits_used.end()) {
                std::map<qubit_t, edge_pos_t> pre_out_edges = node_qubits_outedges(predecessors[qubit]);
                qubits_pre_out_edges.insert(pre_out_edges[qubit]);

                std::map<qubit_t, edge_pos_t> suc_in_edges = node_qubits_inedges(successors[qubit]);
                qubits_suc_in_edges.insert(suc_in_edges[qubit]);
            }
        }
        qubits_pre_out_edges.insert(qubits_suc_in_edges.begin(), qubits_suc_in_edges.end()); // Merge two set to be removed
        for (const auto& edge: qubits_pre_out_edges) {
            remove_edge(edge);
        }

        // 2. Add New node and edges
        int node_index = add_node(node); 
        for (const auto& qubit : node.qubit_pos) {
            add_edge(predecessors[qubit], node_index, EdgeProperties{qubit});
            add_edge(node_index ,successors[qubit], EdgeProperties{qubit});
        } 
    }
   
    void draw_self() const;
    void print_self() const;


private:
    bool is_start_exist() const {
        return get_num_nodes() != 0 && graph[0].name == "start"; 
    }

    bool is_end_exist() const {
        return get_num_nodes() != 0 && graph[1].name == "end"; 
    }

    void remove_edge(node_pos_t source, node_pos_t target) {
        boost::remove_edge(source, target, graph);
    }
    void remove_edge(edge_pos_t edge_pos) {
        boost::remove_edge(edge_pos.first, edge_pos.second, graph);
    }


    // return a dict of {qubits -> predecessors }of node
    std::map<qubit_t, int> node_qubits_predecessors(int node_index) const {
        std::map<qubit_t, int> predecessors;
        DagGraph::edge_iterator ei, ei_end;
        for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei) {
            if (boost::target(*ei, graph) == node_index)
                predecessors[graph[*ei].qubit_id] = boost::source(*ei, graph);
        }
        return predecessors;
    }

    // return a dict of { qubits -> inedges } of node
    std::map<qubit_t, edge_pos_t> node_qubits_inedges(int node_index) {
        std::map<qubit_t, edge_pos_t> qubits_inedges;    
        boost::graph_traits<DagGraph>::in_edge_iterator ei, ei_end; 
        for (boost::tie(ei, ei_end) = boost::in_edges(node_index, graph); ei != ei_end; ++ei) {
            qubits_inedges[graph[*ei].qubit_id] = std::make_pair( boost::source(*ei, graph), boost::target(*ei, graph) );
        }
        return qubits_inedges;
    }

    // return a dict of { qubits -> outedges } of node
    std::map<qubit_t, edge_pos_t> node_qubits_outedges(int node_index) {
        std::map<qubit_t, edge_pos_t> qubits_outedges;    
        boost::graph_traits<DagGraph>::out_edge_iterator ei, ei_end; 
        for (boost::tie(ei, ei_end) = boost::out_edges(node_index, graph); ei != ei_end; ++ei) {
            qubits_outedges[graph[*ei].qubit_id] = std::make_pair( boost::source(*ei, graph), boost::target(*ei, graph) );
        }
        return qubits_outedges;
    }
};

DagGraph reverse_DagGraph(const DagGraph& graph);