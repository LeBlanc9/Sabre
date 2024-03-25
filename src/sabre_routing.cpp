#include <sabre_routing.h>
#include <iostream>

SabreRouting::SabreRouting(CouplingCircuit& c_circuit) : c_circuit(c_circuit) {
    //this->extended_set_weight = self.coupling_graph    
}

void SabreRouting::set_model(std::shared_ptr<Model> model_ptr) {
    this->model_ptr = model_ptr;
}

Model SabreRouting::get_model() {
    return *(this->model_ptr);
}

DAGCircuit SabreRouting::run(DAGCircuit& dag) {
    std::cout << "Run sabre routing !" << std::endl;

    // Precheck
    int virtual_qubits_used = dag.get_qubits_used();
    int physical_qubits = c_circuit.num_qubits; 

    if (virtual_qubits_used == 1)   return dag;  // Single-qubit circuit do not need to perform sabre
    if (virtual_qubits_used > physical_qubits) 
        throw std::invalid_argument("More virtual qubits than physical qubits.");

    
    // Parameter preparation before iteration.
    


    // Start the algorithm from the front layer and iterate until all gates are completed.    
    int iteration = 0;




    return dag;
}