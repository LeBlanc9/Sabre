#include <iostream>
#include "sabre_layout.h"
#include "dag_cuit.h"
#include "coupling.h"
#include "vis.h"

#include <boost/graph/graphviz.hpp>
#include <graphviz/gvc.h>

void test_dag() {
    InstructionNode node1{"a"};
    InstructionNode node2{"b"};
    InstructionNode node3{"c"};

    DagGraph graph;
    
    auto v1 = boost::add_vertex(node1, graph);
    auto v2 = boost::add_vertex(node2, graph);
    auto v3 = boost::add_vertex(node3, graph);


    //boost::remove_vertex(v2, graph);
    boost::add_edge(0,1, graph);

    //std::cout << typeid(decltype(graph[v1])).name() << std::endl;
    std::cout << graph[v1].name << std::endl;
    std::cout << graph[10].name << std::endl;
    //std::cout << graph[2].name << std::endl;

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
    // CouplingList c_list = {
    //     {6, 2, 3.14},
    //     {1, 4, 5.67},
    //     {6, 5, 7.89}
    // };

    // CouplingGraph c_graph{c_list};

    // return 0;
}
