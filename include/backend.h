#pragma once
#include <string>
#include <type.h>


class Backend
{
private:
    CouplingList c_list;
    int qubits_num;
    std::string name;
    std::string backend_type;

public:
    Backend();
    Backend(CouplingList c_list); 
    Backend(CouplingList c_list, int qubits_num); 
};