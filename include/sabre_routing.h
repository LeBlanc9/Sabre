#pragma once
#include <iostream>
#include <memory>
#include <model.h>

class SabreRouting
{
private:
    Model model;
    std::string heuristic = "distance";
    bool modify_dag = false;
    float decay_delta = 0.001;
    int decay_reset_interval = 5;
    int extended_set_size = 20;
    float extended_set_weight = 0.5;

public:
    SabreRouting();
    SabreRouting(Model model);

    void set_model(Model model);
    Model get_model();
    void run(); 
};
