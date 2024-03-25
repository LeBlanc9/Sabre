#pragma once
#include "backend.h"
#include <map> 


class Model
{
private:
    Backend _backend;

public:
    Model();
    Model(Backend backend);

    Backend get_backend(); 
};