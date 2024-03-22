#include "sabre_layout.h"
#include "sabre_routing.h"
#include "dag.h"
#include <iostream>


 SabreLayout::SabreLayout(CouplingCircuit c_circuit, DAGCircuit dag_circuit) : c_circuit(c_circuit), dag_circuit(dag_circuit) {
    std::cout << "SabreLayout Created" << std::endl;
    this->routing_ptr = std::make_unique<SabreRouting>(c_circuit, dag_circuit);
}

void SabreLayout::set_model(Model model) {
    this->model = model;
} 

Model SabreLayout::get_model() {
    return this->model; 
}

void SabreLayout::run(DAGCircuit& dag) {
    std::cout << "Run sabre layout !" << std::endl;

    for (int i; i < max_iteration; i++) {
        //TODO
    }
}

void SabreLayout::run_single(DAGCircuit& dag) {
    (*routing_ptr).run(dag);
}