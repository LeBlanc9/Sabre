#pragma once
#include <iostream>
#include <model.h>
#include "dag.h"
#include "coupling.h"
#include "layout.h"

class SabreRouting
{
public:
    bool modify_dag = false;
    float decay_delta = 0.001;
    int decay_reset_interval = 5;
    int extended_set_size = 20;
    float extended_set_weight = 0.5;

private:
    const CouplingCircuit& c_circuit;
    std::shared_ptr<Model> model_ptr;
    std::string heuristic = "distance";
    
public:
    SabreRouting(const CouplingCircuit& c_circuit);
    void set_model(std::shared_ptr<Model> model_ptr);
    Model get_model();
    DAGCircuit run(const DAGCircuit& dag); 

private:
    void _apply_gate(DAGCircuit& mapped_dag, InstructionNode node, Layout& current_layout);
    
};
