#pragma once
#include <iostream>
#include <model.h>
#include <algorithm>
#include <cmath>
#include "dag.h"
#include "coupling.h"
#include "layout.h"

using SwapPos = std::pair<int, int>;

enum class Heuristic {
    Fidelity,
    Distance,
    Mixture  
};

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
    Heuristic heuristic = Heuristic::Distance;
    std::unordered_map<int, int> qubits_decay = {};
    
public:
    SabreRouting(const CouplingCircuit& c_circuit) : c_circuit(c_circuit) {}

    void set_model(Model& model) { this->model_ptr = std::make_shared<Model>(model); }

    Model get_model() { return *(this->model_ptr); }

    DAGCircuit run(const DAGCircuit& dag); 

private:
    inline void _apply_gate(DAGCircuit& mapped_dag, 
                            const InstructionNode& node, 
                            const Layout& current_layout) const;

    inline std::vector<int> _dag_successors(const DAGCircuit& dag, 
                                            int node_index) const;
    
    inline void _reset_qubits_decay();

    inline double _swap_score(const SwapPos& physical_swap_qubits) const;

    std::set<int> _calc_extended_set(const DAGCircuit& dag, 
                                     const std::vector<int>& front_layer);

    std::set<SwapPos> _obtain_swaps(const std::vector<int>& front_layer, 
                                    const Layout& current_layout, 
                                    const DAGCircuit& dag);

    void _get_best_swap(const std::set<SwapPos>& swap_candidates, 
                        const Layout& current_layout, 
                        const std::vector<int>& front_layer, 
                        const std::set<int>& extended_set, 
                        const std::set<std::pair<int, int>>& unavailable_2qubits) const;

    double _score_heuristic(Heuristic heuristic, 
                            std::vector<int> front_layer, 
                            std::set<int> extended_set, 
                            Layout current_layout) const; 
};


inline void SabreRouting::_apply_gate(  DAGCircuit& mapped_dag, 
                                        const InstructionNode& node, 
                                        const Layout& current_layout) const 
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


inline double SabreRouting::_swap_score(const SwapPos& physical_swap_qubits) const
{
    const auto pair_forward = boost::edge(physical_swap_qubits.first, physical_swap_qubits.second, c_circuit.graph); 
    const auto pair_backward = boost::edge(physical_swap_qubits.second, physical_swap_qubits.first, c_circuit.graph); 

    const double fidelity_forward = boost::get(boost::edge_bundle, c_circuit.graph)[pair_forward.first].fidelity;
    const double fidelity_backward =  boost::get(boost::edge_bundle, c_circuit.graph)[pair_backward.first].fidelity;

    const double min_f = std::min(std::log(fidelity_forward), std::log(fidelity_backward));    
    const double max_f = std::max(std::log(fidelity_forward), std::log(fidelity_backward));    

    return 2 * max_f + min_f;
}