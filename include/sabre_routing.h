#pragma once
#include <iostream>
#include <memory>
#include <model.h>
#include "dag.h"
#include "coupling.h"

class SabreRouting
{
private:
    Model model;
    CouplingCircuit& c_circuit;
    DAGCircuit& dag_circuit;
    std::string heuristic = "distance";
    bool modify_dag = false;
    float decay_delta = 0.001;
    int decay_reset_interval = 5;
    int extended_set_size = 20;
    float extended_set_weight = 0.5;
    

public:
    SabreRouting(CouplingCircuit& c_circuit, DAGCircuit& dag_circiut);
    void set_model(Model model);
    Model get_model();
    int run(DAGCircuit& dag); 
};
