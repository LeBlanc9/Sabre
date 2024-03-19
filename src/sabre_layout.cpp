#include "sabre_layout.h"
#include "dag_cuit.h"
#include <iostream>


SabreLayout::SabreLayout() {
    std::cout << "SabreLayout Created" << std::endl;
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

    }
}

void SabreLayout::run_single(DAGCircuit& dag) {
    this->routing_pass.run(dag);
}