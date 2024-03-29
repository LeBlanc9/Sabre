#pragma once
#include <iostream>
#include <model.h>
#include "dag.h"
#include "coupling.h"

class SabreRouting
{
private:
    CouplingCircuit& c_circuit;
    std::shared_ptr<Model> model_ptr;
    std::string heuristic = "distance";
    bool modify_dag = false;
    float decay_delta = 0.001;
    int decay_reset_interval = 5;
    int extended_set_size = 20;
    float extended_set_weight = 0.5;
    

public:
    SabreRouting(CouplingCircuit& c_circuit);
    void set_model(std::shared_ptr<Model> model_ptr);
    Model get_model();
    DAGCircuit run(DAGCircuit& dag); 
};
