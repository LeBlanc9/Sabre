#include "sabre_layout.h"
#include <iostream>


SabreLayout::SabreLayout() {
    std::cout << "SabreLayout Created" << std::endl;
}

void SabreLayout::set_model(Model model) {
    this->model = model;
} 

Model SabreLayout::get_model() {
    return this->model; 
}

void SabreLayout::run() {}

void SabreLayout::run_single() {}