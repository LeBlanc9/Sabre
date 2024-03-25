#include <sabre_routing.h>
#include <iostream>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>

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
    std::cout << "Run sabre routing !" << std::endl;

    //* Precheck
    int qubits_used = dag.get_qubits_used();
    if (qubits_used == 1) { 
        std::cerr << "Warning: single qubit circuit no need optimize." << std::endl;
        return dag; 
    }
    if (qubits_used > this->c_circuit.num_qubits)   
        throw std::runtime_error("More virtual qubits than physical qubits.");
    

    //* Parameter preparation before iteration.
    DAGCircuit mapped_dag;
    std::unordered_map<int, int> pre_executed_counts;
    std::vector<int> front_layer = {1}; 

    DagGraph::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(dag.graph); ei != ei_end; ++ei) 
    {
        std::cout << *ei << dag.graph[*ei].qubit_id << std::endl;
        if ( boost::source(*ei, dag.graph) == 0) {
            std::cout << 0 << std::endl;
            if (pre_executed_counts[ boost::target(*ei, dag.graph) ] == 2) {
                front_layer.push_back(boost::target(*ei, dag.graph));
            }
        }

    }


    //* Start the algorithm from the front layer and iterate until all gates are completed.    
    std::cout << "Iterate DAG edges!" << std::endl;
    int iteration = 0;
    while (!front_layer.empty())
    {

    }
    


    if (this->modify_dag == true)   return mapped_dag;
    return dag;
}