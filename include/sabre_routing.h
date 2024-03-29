#pragma once
#include <iostream>
#include <model.h>
#include "dag.h"
#include "coupling.h"
#include "layout.h"

using SwapPos = std::pair<int, int>;

class SabreRouting
{
public:
    bool modify_dag = false;
    float decay_delta = 0.001;
    int decay_reset_interval = 5;
    int extended_set_size = 20;
    float extended_set_weight = 0.5;

private:
    const CouplingCircuit& c_circuit;
    std::shared_ptr<Model> model_ptr;
    std::string heuristic = "distance";
    std::unordered_map<int, int> qubits_decay = {};
    
public:
    SabreRouting(const CouplingCircuit& c_circuit);
    void set_model(Model& model);
    Model get_model();
    DAGCircuit run(const DAGCircuit& dag); 

private:
    inline void _apply_gate(DAGCircuit& mapped_dag, InstructionNode node, Layout& current_layout) const;

    inline std::vector<int> _dag_successors(const DAGCircuit& dag, int node_index) const;
    
    inline void _reset_qubits_decay();

    std::set<int> _calc_extended_set(const DAGCircuit& dag, const std::vector<int>& front_layer);

    std::set<SwapPos> _obtain_swaps(const std::vector<int>& front_layer, const Layout& current_layout, const DAGCircuit& dag);

    // def _get_best_swap(self, swap_candidates, current_layout, front_layer, extended_set, unavailable_2qubits):
    void _get_best_swap(const std::set<SwapPos>& swap_candidates, const Layout& current_layout, const std::vector<int>& front_layer, const std::set<int>& extended_set, const std::set<std::pair<int, int>>& unavailable_2qubits) const;

};


inline void SabreRouting::_apply_gate(DAGCircuit& mapped_dag, InstructionNode node, Layout& current_layout) const 
{
    if (this->modify_dag == true) 
    {
        std::vector<int> mapped_qubit_pos;
        for (const auto& qubit_pos : node.qubit_pos) 
            mapped_qubit_pos.push_back(current_layout[qubit_pos]);

        // Add the gate to the mapped_dag
        mapped_dag.add_node(node);
    }
}

inline std::vector<int> SabreRouting::_dag_successors(const DAGCircuit& dag, int node_index) const
{
    std::vector<int> successors;
    auto out_edges = boost::out_edges(node_index, dag.graph);
    for (auto it = out_edges.first; it != out_edges.second; ++it) 
        successors.push_back(boost::target(*it, dag.graph));

    return successors;
}


inline void SabreRouting::_reset_qubits_decay()
{
    for (auto& pair : this->qubits_decay) 
        pair.second = 1;
}