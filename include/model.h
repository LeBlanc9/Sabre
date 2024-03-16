#pragma once
#include"backend.h"


class Model
{
private:
    Backend& _backend;

public:
    Model(Backend& backend);
    Backend& get_backend(); 
};