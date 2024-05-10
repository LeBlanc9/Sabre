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
using node_pos_t = unsigned long;
using edge_pos_t = std::pair<node_pos_t, node_pos_t>;


struct InstructionNode 
{
public:
    std::string name = "NoName"; 
    std::vector<qubit_t> qubit_pos;

    InstructionNode();
    InstructionNode(const std::string& name);
    InstructionNode(const std::string& name, std::vector<int> pos);

    friend std::ostream& operator<< (std::ostream& os, const InstructionNode& node);
};


struct MeasureNode : public InstructionNode
{
public:
    std::vector<cbit_t> classic_pos;

public:
    MeasureNode() 
        : InstructionNode("measure") {
    }

    MeasureNode(std::vector<int> qubit_pos, std::vector<int> classic_pos) 
        : InstructionNode("measure", qubit_pos), classic_pos(classic_pos) {
    }
};


struct EdgeProperties 
{
public:
    qubit_t qubit_id; // 1,2,3 for qubits 

    EdgeProperties();
    EdgeProperties(qubit_t qubit_id);
};


struct Edge 
/*
    A struct to represent an edge in the DAG graph. For convenience, we use a struct to store the edge information.
    It contains the source node, target node and the edge properties.
*/
{
public:
    node_pos_t source;    
    node_pos_t target;
    EdgeProperties ep;
public:
    Edge(node_pos_t source, node_pos_t target, EdgeProperties ep) 
        : source(source), target(target), ep(ep){
    }
};


using DagGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, InstructionNode, EdgeProperties>;


class DAGCircuit
/*
   A directed acyclic graph (DAG) representation of a quantum circuit.     

   Using boost graph library to represent the DAG.
   Each vertex is an InstructionNode which represents a quantum gate or operation. 
   Each edge is an EdgeProperties which represents qubit.
*/ 
{
public:
    DagGraph graph;
private:
    const node_pos_t start_node_pos = 0;
    const node_pos_t end_node_pos = 1;

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
        if (!this->empty()) {
            const node_pos_t node_index = add_node(node);
            std::vector<DagGraph::edge_descriptor> edges_to_remove;
            std::vector<Edge> edges_to_add;
            std::vector<qubit_t> node_remain_qubits = node.qubit_pos;  
              
            DagGraph::in_edge_iterator ei, ei_end;
            for (boost::tie(ei, ei_end) = boost::in_edges(end_node_pos, graph); ei != ei_end; ++ei) {
                const qubit_t qubit = graph[*ei].qubit_id; 
                // 判断这条边的qubit是否在node的qubit_pos中
                if ( std::find(node.qubit_pos.begin(), node.qubit_pos.end(), qubit) != node.qubit_pos.end() ) {
                    edges_to_remove.push_back(*ei);
                    edges_to_add.push_back(Edge{boost::source(*ei, graph), node_index, EdgeProperties{qubit}});
                    edges_to_add.push_back(Edge{node_index, end_node_pos, EdgeProperties{qubit}});
                    node_remain_qubits.erase(std::remove(node_remain_qubits.begin(), node_remain_qubits.end(), qubit), node_remain_qubits.end());     
                }
            }
            for (const auto& edge: edges_to_remove) {
                boost::remove_edge(edge, graph);
            }
            for (const auto& edge: edges_to_add) {
                boost::add_edge(edge.source, edge.target, edge.ep, graph);
            }
            for (const auto& qubit: node_remain_qubits) {
                add_edge(start_node_pos, node_index, EdgeProperties{qubit});
                add_edge(node_index, end_node_pos, EdgeProperties{qubit});
            }

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


    bool empty() const {
        return get_num_nodes() == 0;
    }

    void draw_self() const;
    void print_self() const;


private:
    bool _is_start_exist() const {
        return get_num_nodes() != 0 && graph[0].name == "start"; 
    }

    bool _is_end_exist() const {
        return get_num_nodes() != 0 && graph[1].name == "end"; 
    }

    void remove_edge(node_pos_t source, node_pos_t target) {
        boost::remove_edge(source, target, graph);
    }
    void remove_edge(Edge edge) {
        boost::remove_edge(edge.source, edge.target, graph);
    }

};

DagGraph reverse_DagGraph(const DagGraph& graph);