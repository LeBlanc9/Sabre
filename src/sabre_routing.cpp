#include <sabre_routing.h>
#include <iostream>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include "layout.h"

SabreRouting::SabreRouting(const CouplingCircuit& c_circuit) : c_circuit(c_circuit) {}

void SabreRouting::set_model(Model& model) 
{
    this->model_ptr = std::make_shared<Model>(model);
}

Model SabreRouting::get_model() 
{
    return *(this->model_ptr);
}

/*
    Return a @c DAGCircuit : The original dag or the mapped_dag with added swap gate depending on modify_flag.
*/
DAGCircuit SabreRouting::run(const DAGCircuit& dag) {

    //* Precheck
    std::set<int> qubits_used = dag.get_qubits_used();
    
    if (qubits_used.size() == 1) 
    { 
        std::cerr << "Warning: single qubit circuit no need optimize." << std::endl;
        return dag; 
    }
    if (qubits_used.size() > this->c_circuit.num_qubits)   
        throw std::runtime_error("More virtual qubits than physical qubits.");

    for(const auto& qubit : qubits_used) 
        this->qubits_decay[qubit] = 1;

    this->extended_set_size = this->c_circuit.num_qubits;

    // Decay coefficient for penalizing serial swaps. Set to average fidelity
    //if (this->heuristic == 'fidelity') {
    //}

    //* Parameter preparation before iteration.
    DAGCircuit mapped_dag;

    Layout current_layout;
    if (model_ptr->init_layout.empty())
    {
        model_ptr->init_layout = generate_random_layout(qubits_used.size(), c_circuit.num_qubits);
    }

    current_layout = model_ptr->init_layout;
    // for (const auto& pair : current_layout) {
    //         std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    // }
    print_layout(current_layout.get_v2p());
    std::unordered_map<int, int> pre_executed_counts;
    std::vector<int> front_layer; 

    DagGraph::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(dag.graph); ei != ei_end; ++ei) 
    {
        if ( boost::source(*ei, dag.graph) == 0) 
        {
            const auto target = boost::target(*ei, dag.graph);
            pre_executed_counts[target]++;
            if ((pre_executed_counts[target] == 2) || (dag.graph[target].qubit_pos.size() == 1))
                front_layer.push_back(target);
        }

    }
    //* TEST pre_executed_counts and front_layer
    // for (const auto& pair : pre_executed_counts) 
    //     std::cout << "Key: " << dag.graph[pair.first] << ", Value: " << pair.second << std::endl;
    // for (const auto& item : front_layer)     
    //     std::cout << "front_layer" << item << dag.graph[item] << ", ";

    std::set<std::pair<int, int>> unavailable_2qubits;

    //* Start the algorithm from the front layer and iterate until all gates are completed.    
    int iteration_count = 0;
    while ( !front_layer.empty() )
    {
        std::vector<int> execute_gate_list;
        /* 处理 front_layer 中节点, 记录
            executable_gates        -> (单比特的门，barrier，XY门，measure) + (layout后两个qubit直接相连的两比特门) = 直接可执行的门
            unabailable_2quibits    -> 
        */
        for (const auto& node_index : front_layer)
        {
            const InstructionNode& node = dag.graph[node_index];
            // 如果是两个qubit的门
            if (node.qubit_pos.size() == 2 && node.name != "barrier" && node.name != "XY" && node.name != "measure") 
            {
                int v0 = node.qubit_pos[0];
                int v1 = node.qubit_pos[1];
                int p0 = current_layout[v0];
                int p1 = current_layout[v1];

                // 如果物理上这front_layer中某个门两个qubit直接相连
                if (boost::edge(p0, p1, c_circuit.graph).second) 
                {
                    execute_gate_list.push_back(node_index);

                    std::set<std::pair<int, int>> unavailable_2qubits_new;
                    for (const auto& item : unavailable_2qubits) 
                    {
                        if (item.first != p0 && item.second != p0 && item.first != p1 && item.second != p1) 
                            unavailable_2qubits_new.insert(item);
                    }  
                    unavailable_2qubits = std::move(unavailable_2qubits_new);
                }
            } 
            else 
                execute_gate_list.push_back(node_index); // Single-qubit gates, barriers, XY-gates and measures are both executable gates.
        }

        // for (auto it = execute_gate_list.begin(); it != execute_gate_list.end(); ++it) 
        //     std::cout << "execute_gate: " << *it << dag.graph[*it] << std::endl;
        if ( !execute_gate_list.empty() ) 
        {
            for (const int& node_index : execute_gate_list)
            {
                _apply_gate(mapped_dag, dag.graph[node_index], current_layout);
                front_layer.erase(std::remove(front_layer.begin(), front_layer.end(), node_index), front_layer.end());
                for ( int successor : _dag_successors(dag, node_index) ) 
                {
                    pre_executed_counts[successor]++;
                    if (pre_executed_counts[successor] == dag.graph[successor].qubit_pos.size())
                        front_layer.push_back(successor);  
                }
                
            }
            iteration_count = 0;
            _reset_qubits_decay();

            continue;
        }
        std::set<int> extended_set = _calc_extended_set(dag, front_layer);
        // std::cout << "extended_set: ";
        // for (const auto& item : extended_set) 
        //     std::cout << item << ", ";
        // std::cout << std::endl;

        std::set<std::pair<int, int>> swap_candidates = _obtain_swaps(front_layer, current_layout, dag);
        // for (const auto& swap : swap_candidates) {
        //     std::cout << "(" << swap.first << ", " << swap.second << ")" << std::endl;
        // } 


        break;
    }

    if (this->modify_dag)   
        return mapped_dag;
    else 
        return dag;
}



std::set<int> SabreRouting::_calc_extended_set(const DAGCircuit& dag, const std::vector<int>& front_layer)
{
    /*Calculate the extended set for lookahead capabilities.
    Args:
        dag (DAGCircuit): a dag
        front_layer (list): The front layer in the dag.
    Returns:
        extended_set (set): Set of expansion gates obtained according to requirements.
    */ 

    std::set<int> extended_set{};
    std::vector<int> new_front_layer(front_layer);

    while ( !new_front_layer.empty() && extended_set.size() < this->extended_set_size )
    {
        int node_index = new_front_layer.front();
        new_front_layer.erase(new_front_layer.begin());  

        auto successors_nodes = this->_dag_successors(dag, node_index);
        new_front_layer.insert(new_front_layer.end(), successors_nodes.begin(), successors_nodes.end());          

        for (int successor : successors_nodes)
        {
            const auto& node = dag.graph[successor];
            if ( node.qubit_pos.size() == 2 )
                extended_set.insert(successor);
        }   
    }
    return extended_set;
}


std::set<SwapPos> SabreRouting::_obtain_swaps(const std::vector<int>& front_layer, const Layout& current_layout, const DAGCircuit& dag)
{   
    std::set<SwapPos> candiate_swaps{};
    for ( const auto& node_index : front_layer )
    {
        for ( const auto& virtual_pos : dag.graph[node_index].qubit_pos )
        {
            int physical_pos = current_layout[virtual_pos];
            auto neighbors = boost::adjacent_vertices(physical_pos, c_circuit.graph);

            for ( auto it =  neighbors.first; it != neighbors.second; ++it )
            {
                int virtual_neighbor = current_layout.get_p2v().at(*it);
                SwapPos swap = std::minmax(virtual_pos, virtual_neighbor);
                candiate_swaps.insert(swap);
            }
        }
    }
    return candiate_swaps;
}



