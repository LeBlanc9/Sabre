#pragma once
#include <vector>
#include <tuple>
#include <memory>
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
    CouplingList c_list;
    bool is_bidirectional = true;    
    unsigned int num_qubits = 0;

    CouplingCircuit(CouplingList c_list);
    void update_num_qubits();
    void draw_self();


private:
};