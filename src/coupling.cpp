#include "coupling.h"
#include "vis.h"
#include <set>
#include <algorithm>
#include <iostream>

CouplingCircuit::CouplingCircuit() {}
CouplingCircuit::CouplingCircuit(CouplingList c_list) : c_list(c_list) {
    //* Init sorted qubit vecter
    std::set<int> qubits_set;
    for (const auto& link : c_list) {
        qubits_set.insert(std::get<0>(link));
        qubits_set.insert(std::get<1>(link));
    }
    std::copy(qubits_set.begin(), qubits_set.end(), std::back_inserter(_qubits_vec));


    //* add node&edge to graph
    for (int num : _qubits_vec) {
        CouplingNode c_node{num};
        boost::add_vertex(c_node, graph);
    }
    for (const auto& link : c_list) {
        CouplingEdge c_egde{std::get<2>(link)};
        boost::add_edge(std::get<0>(link), std::get<1>(link), c_egde, graph);
    } 
    
}


unsigned int CouplingCircuit::get_num_qubits() { return _num_qubits; }
void CouplingCircuit::draw_self() { draw_dot(graph_to_dot(graph)); }




