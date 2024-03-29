#include "model.h"
#include <iostream>


Model::Model() {
    std::cout << "Empty model created" << std::endl;
}

Model::Model(Backend backend) : _backend(backend) {
    std::cout << "Model with backend created" << std::endl;
}

Backend  Model::get_backend() {
    return _backend;
}
