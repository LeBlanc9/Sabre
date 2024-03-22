#pragma once
#include <vector>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

using CouplingList = std::vector<std::tuple<int, int, float>>;
const CouplingList EMPTY_COUPLING_LIST = {std::make_tuple(0, 0, 0)};

struct CouplingNode{
    int id;
};
struct CouplingEdge{
    float fidelity;
};

using CouplingGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, CouplingNode, CouplingEdge>;

class CouplingCircuit
{
public:     
    CouplingGraph graph;
    CouplingList c_list = EMPTY_COUPLING_LIST;
    bool is_bidirectional = true;    

    CouplingCircuit();
    CouplingCircuit(CouplingList c_list);

    unsigned int get_num_qubits();
    void draw_self();



private:
    unsigned int _num_qubits = 0;
    std::vector<int> _qubits_vec = {};        
};