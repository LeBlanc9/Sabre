#pragma once
#include <string>
#include <type.h>
#include <model.h>
#include "sabre_routing.h"
#include "dag_cuit.h" 

class SabreLayout
{
public:
    Model model;
    CouplingList coupling_list = EMPTY_COUPLING_LIST;
    std::string heuristic="distance";
    int max_iteration = 3; 
    SabreRouting routing_pass;

    SabreLayout();

    void set_model(Model model);
    Model get_model();
    void run(DAGCircuit& dag);
    void run_single(DAGCircuit& dag);
};