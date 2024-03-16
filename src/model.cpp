#include"model.h"


Model::Model(Backend& backend) : _backend(backend) {}

Backend& Model::get_backend() {
    return _backend;
}
