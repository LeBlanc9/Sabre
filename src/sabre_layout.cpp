#include "sabre_layout.h"
#include "sabre_routing.h"
#include "dag.h"
//#include <algorithm>
#include <iostream>


SabreLayout::SabreLayout(CouplingCircuit c_circuit) : c_circuit(c_circuit) {
    std::cout << "SabreLayout Created" << std::endl;
    // init routing_ptr
    this->routing_ptr = std::make_unique<SabreRouting>(c_circuit);
    this->routing_ptr->set_model(this->model_ptr);
}

void SabreLayout::set_model(const Model& model) {
    this->model_ptr = std::make_shared<Model>(model);
} 

Model SabreLayout::get_model() {
    return *(this->model_ptr); 
}

DAGCircuit SabreLayout::run(DAGCircuit& dag) {
    std::cout << "Run sabre layout !" << std::endl;

    if (dag.get_qubits_used() == 1)
        return dag;


    for (int i; i < max_iteration; i++) {
        for (const auto& direction : {"forward", "backward"}) {
            this->run_single(dag);
        }
    }
    return dag;
}

DAGCircuit SabreLayout::run_single(DAGCircuit& dag) {
    DAGCircuit new_dag = this->routing_ptr->run(dag);
    return new_dag;
}