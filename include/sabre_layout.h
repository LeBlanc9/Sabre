#pragma once
#include <string>
#include "coupling.h"
#include "model.h"
#include "sabre_routing.h"
#include "dag.h" 

class SabreLayout
{
public:
    Model model;
    CouplingCircuit c_circuit;
    DAGCircuit dag_circuit;
    std::unique_ptr<SabreRouting> routing_ptr;
    std::string heuristic = "distance";
    int max_iteration = 3; 

    SabreLayout(CouplingCircuit c_circuit, DAGCircuit dag_circuit);

    void set_model(Model model);
    Model get_model();
    void run(DAGCircuit& dag);
    void run_single(DAGCircuit& dag);
};