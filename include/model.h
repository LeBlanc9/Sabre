#pragma once
#include "backend.h"
#include <map> 


class Model
{
private:
    Backend _backend;

public:
    std::unordered_map<int, int> init_layout;
    std::unordered_map<int, int> final_layout;

    Model();
    Model(Backend backend);

    Backend get_backend() const; 
};