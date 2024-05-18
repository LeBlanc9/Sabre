#include "sabre_layout.h"
#include "sabre_routing.h"
#include <algorithm>
#include <iostream>


SabreLayout::SabreLayout(const CouplingCircuit& c_circuit) : c_circuit(c_circuit) {
    std::cout << "SabreLayout Created" << std::endl;
    // init routing_ptr
    this->routing_ptr = std::make_unique<SabreRouting>(c_circuit);
    // this->routing_ptr->set_model(get_model());
}