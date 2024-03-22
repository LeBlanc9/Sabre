#include <sabre_routing.h>
#include <iostream>

SabreRouting::SabreRouting(CouplingCircuit& c_circuit, DAGCircuit& dag_circuit) : c_circuit(c_circuit), dag_circuit(dag_circuit){}

void SabreRouting::set_model(Model model) {
    this->model = model;
}

Model SabreRouting::get_model() {
    return model;
}

int SabreRouting::run(DAGCircuit& dag) {
    std::cout << "Run sabre routing !" << std::endl;

    // Precheck
    int virtual_qubits_used = dag.get_qubits_used();
    int physical_qubits = c_circuit.get_num_qubits(); 

    if (virtual_qubits_used == 1)   return 0;  // Single-qubit circuit do not need to perform sabre
    if (virtual_qubits_used > physical_qubits) 
        throw std::invalid_argument("More virtual qubits than physical qubits.");

    if (this->heuristic == "")
        return 0;


    return 0;
}