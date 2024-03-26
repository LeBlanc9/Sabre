#include <sabre_routing.h>
#include <iostream>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include "layout.h"

SabreRouting::SabreRouting(const CouplingCircuit& c_circuit) : c_circuit(c_circuit) {
    //this->extended_set_weight = self.coupling_graph    
}

void SabreRouting::set_model(std::shared_ptr<Model> model_ptr) {
    this->model_ptr = model_ptr;
}

Model SabreRouting::get_model() {
    return *(this->model_ptr);
}

/*
    Return a @c DAGCircuit : The original dag or the mapped_dag with added swap gate depending on modify_flag.
*/
DAGCircuit SabreRouting::run(const DAGCircuit& dag) {

    //* Precheck
    int qubits_used = dag.get_qubits_used();
    if (qubits_used == 1) 
    { 
        std::cerr << "Warning: single qubit circuit no need optimize." << std::endl;
        return dag; 
    }
    if (qubits_used > this->c_circuit.num_qubits)   
        throw std::runtime_error("More virtual qubits than physical qubits.");
    

    //* Parameter preparation before iteration.
    DAGCircuit mapped_dag;
    // mapping (dict): {virtual qubit: physical qubit, v0: p0, v1: p1, ...}
    std::unordered_map<int, int> current_layout = generate_random_layout(qubits_used, c_circuit.num_qubits);
    // for (const auto& pair : current_layout) {
    //         std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    // }


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
    int iteration = 0;
    while ( !front_layer.empty() )
    {
        std::vector<int> execute_gate_list; 

        // 处理 front_layer 中节点 -> 记录 executable gates, unabailable_2quibits
        for (const auto& node_index : front_layer)
        {
            const InstructionNode& node = dag.graph[node_index];
            // 如果是两个qubit的门
            if (node.qubit_pos.size() == 2 && node.name != "barrier" && node.name != "XY" && node.name != "measure") {
                int v0 = node.qubit_pos[0];
                int v1 = node.qubit_pos[1];
                int p0 = current_layout[v0];
                int p1 = current_layout[v1];

                // 如果物理上这front_layer中某个门两个qubit直接相连
                if (boost::edge(p0, p1, c_circuit.graph).second) {
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
            else {
                // Single-qubit gates, barriers, XY-gates and measures are both executable gates.
                execute_gate_list.push_back(node_index); 
            }
        }

        for (auto it= execute_gate_list.begin(); it != execute_gate_list.end(); ++it) 
        {
            std::cout << "execute_gate_list: " << *it << std::endl;
        }

        if ( !execute_gate_list.empty() ) {
            for (const int& node_index : execute_gate_list)
            {
                this->_apply_gate(mapped_dag, dag.graph[node_index], current_layout);
            }
        }
        break;
    }
    
    if (this->modify_dag == true)   return mapped_dag;
    else return dag;
}



void SabreRouting::_apply_gate(DAGCircuit& mapped_dag, InstructionNode node, Layout& current_layout) 
{
    if (this->modify_dag == true) 
    {
        std::vector<int> mapped_qubit_pos;
        for (const auto& qubit_pos : node.qubit_pos) {
            mapped_qubit_pos.push_back(current_layout[qubit_pos]);
        }

        // Add the gate to the mapped_dag
        mapped_dag.add_node(node);
    }
}