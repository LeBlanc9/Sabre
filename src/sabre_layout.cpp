#include "sabre_layout.h"
#include "sabre_routing.h"
#include "dag.h"
#include <algorithm>
#include <iostream>


SabreLayout::SabreLayout(const CouplingCircuit& c_circuit) : c_circuit(c_circuit) 
{
    std::cout << "SabreLayout Created" << std::endl;
    // init routing_ptr
    this->routing_ptr = std::make_unique<SabreRouting>(c_circuit);
    // this->routing_ptr->set_model(get_model());
}

void SabreLayout::set_model(const Model& model) { this->model_ptr = std::make_shared<Model>(model); } 
Model SabreLayout::get_model() const { return *(this->model_ptr); }


DAGCircuit SabreLayout::run(const DAGCircuit& dag) 
{
    std::cout << "Run sabre layout !" << std::endl;
    //* Precheck
    std::set<int> qubits_used = dag.get_qubits_used();
    if (qubits_used.size() == 1) { 
        std::cerr << "Warning: single qubit circuit no need optimize." << std::endl;
        return dag; 
    }
    if (qubits_used.size() > this->c_circuit.num_qubits) 
        throw std::runtime_error("More virtual qubits than physical qubits.");

    //* Presetting
    // Todo: remove measure node    
    this->routing_ptr->modify_dag = false;  // make sure modify_dag = false
    DagGraph rev_graph = reverse_DagGraph(dag.graph);  
    DAGCircuit rev_dag(rev_graph);

    //* Start Iteration 
    for (int i=0; i < max_iteration; i++) {
        for (const auto& direction : {0, 1}) {
            // const DAGCircuit& selected_dag = direction == 0 ? dag : rev_dag;
            // this->run_single(selected_dag);
            this->run_single(direction == 0 ? dag : rev_dag);
        }
    }
    // TODO:
    // The last forward iteration obtains the final circuit. 
    this->routing_ptr->modify_dag = true;
    DAGCircuit physical_dag = this->run_single(dag);

    return dag;
}


DAGCircuit SabreLayout::run_single(const DAGCircuit& dag) 
{
    DAGCircuit new_dag = this->routing_ptr->run(dag);
    return new_dag;
}