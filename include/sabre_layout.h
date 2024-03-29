#pragma once
#include <string>
#include "coupling.h"
#include "model.h"
#include "sabre_routing.h"
#include "dag.h" 

class SabreLayout
{
public:
    CouplingCircuit c_circuit;
    std::shared_ptr<Model> model_ptr;
    std::unique_ptr<SabreRouting> routing_ptr;
    std::string heuristic = "distance";
    int max_iteration = 3; 

    SabreLayout(CouplingCircuit c_circuit);

    void set_model(const Model& model);
    Model get_model();
    DAGCircuit run(DAGCircuit& dag);
    DAGCircuit run_single(DAGCircuit& dag);
};