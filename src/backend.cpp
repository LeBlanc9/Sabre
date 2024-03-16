#include "backend.h"




Backend::Backend() {}

//Backend::Backend(CouplingList c_list) :
//   c_list(c_list) {}

Backend::Backend(std::vector<std::tuple<int, int, float>>) {}

Backend::Backend(CouplingList c_list, int qubits_num) :
    c_list(c_list), qubits_num(qubits_num) {}




