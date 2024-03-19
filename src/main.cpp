#include <iostream>
#include "sabre_layout.h"
#include "dag_cuit.h"
#include "vis.h"

#include <boost/graph/graphviz.hpp>
#include <graphviz/gvc.h>

void test_dag() {
    InstructionNode node1{-1,"www"};
    InstructionNode node2{-2,"222"};

    Graph graph;
    
    auto v1 = boost::add_vertex(node1, graph);
    auto v2 = boost::add_vertex(node2, graph);
    boost::add_edge(0,1, graph);

    DAGCircuit dag{graph};
    dag.draw_self();

}

int main() {
    std::cout << "-- main function --" << std::endl;

    // CouplingList c_list = get_default_clist();
    // Backend backend(c_list);
    // Model model;

    //SabreLayout sabreLayout;
    //DAGCircuit dag;
    test_dag();
    return 0;
}
