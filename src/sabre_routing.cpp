#include <sabre_routing.h>
#include <sabre.h>
#include <iostream>
#include <memory>


SabreRouting::SabreRouting() {}

SabreRouting::SabreRouting(Model model) : model(model) {
    std::cout << "Creating SabreRouting" << std::endl;
}

void SabreRouting::set_model(Model model) {
    this->model = model;
}

Model SabreRouting::get_model() {
    return model;
}

void SabreRouting::run(){}