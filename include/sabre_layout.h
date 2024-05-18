#pragma once
#include <string>
#include "coupling.h"
#include "model.h"
#include "sabre_routing.h"
#include "DAG/dag.h" 

class SabreLayout
{
public:
    const CouplingCircuit& c_circuit;
    std::shared_ptr<Model> model_ptr;
    std::unique_ptr<SabreRouting> routing_ptr;
    Heuristic heuristic = Heuristic::Distance;
    int max_iteration = 3; 

public:
    SabreLayout(const CouplingCircuit& c_circuit);

    void set_model(const Model& model) { 
        this->model_ptr = std::make_shared<Model>(model); 
    } 

    Model get_model() const { 
        return *(this->model_ptr); 
    }

    DAGCircuit run(const DAGCircuit& dag) {
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
                this->run_single(direction == 0 ? dag : rev_dag);
            }
        }
        // The last forward iteration obtains the final circuit. 
        this->routing_ptr->modify_dag = true;
        DAGCircuit physical_dag = this->run_single(dag);

        return dag;
    }


    DAGCircuit run_single(const DAGCircuit& dag) {
        DAGCircuit new_dag = this->routing_ptr->run(dag);
        return new_dag;
    }


};