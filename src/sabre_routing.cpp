#include <pybind11/pybind11.h>
#include <iostream>
#include <typeinfo>

namespace py=pybind11;


class SabreRouting 
{

public:
    py::list coupling_list;
    std::string heuristic = "distance";
    py::object initial_layout;
    bool modify_dag = false;
    float decay_delta = 0.001;
    int decay_reset_interval = 5;
    int extended_set_size = 20;
    float extended_set_weight = 0.5;


    SabreRouting(   
                    // py::list coupling_list,
                    // py::object initial_layout,
                    // std::string heuristic = "distance",
                    // bool modify_dag = false,
                    // float decay_delta = 0.001,
                    // int decay_reset_interval = 5,
                    // int extended_set_size = 20,
                    // float extended_set_weight = 0.5
                )
        // :   coupling_list(coupling_list),   initial_layout(initial_layout), heuristic(heuristic),
        //     modify_dag(modify_dag),         decay_delta(decay_delta),       decay_reset_interval(decay_reset_interval),
        //     extended_set_size(extended_set_size), extended_set_weight(extended_set_weight)
    {
        std::cout << "Creating SabreRouting" << std::endl;
    }
    ~SabreRouting(){}

    void set_model(py::object model) {
        this->model = model;
    }

    py::object get_model() {
        return this->model;
    }

    void run(){} 




private:
    py::object model;
};

