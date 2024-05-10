#include <iostream>
#include "sabre_layout.h"
#include "dag.h"
#include "coupling.h"
#include "vis.h"
#include "layout.h"

#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>
#include <iomanip>
#include <graphviz/gvc.h>

DAGCircuit test_dag() {
    InstructionNode node1{"start"};
    InstructionNode node2{"b"};
    InstructionNode node3{"c"};

    DagGraph graph;
    
    auto v0 = boost::add_vertex(node1, graph);
    auto v1 = boost::add_vertex(node2, graph);
    auto v2 = boost::add_vertex(node3, graph);

    boost::add_edge(0,1,EdgeProperties{0}, graph);
    boost::add_edge(1,2,EdgeProperties{1}, graph);
    boost::add_edge(0,2,EdgeProperties{1}, graph);

    DagGraph test_graph;
    DAGCircuit dag_circuit{test_graph};

    dag_circuit.add_instruction_node_end(InstructionNode{"swap", {0, 1}});
    dag_circuit.add_instruction_node_end(InstructionNode{"oneqbit_gate", {0,1,2}});

    dag_circuit.print_self();
    dag_circuit.draw_self();

    return dag_circuit;
}

CouplingCircuit test_c_ciruit() {
    CouplingList c_list = {
        {0, 1, 0.99},
        {2, 3, 0.67},
        {2, 4, 0.89},
        {1, 4, 0.89},
        {3, 4, 0.89},
    };
    CouplingCircuit c_circuit{c_list};  

    CouplingGraph graph;
    auto v1 = boost::add_vertex(CouplingNode{20}, graph);
    auto v2 = boost::add_vertex(CouplingNode{10}, graph);


    std::cout << "c_circuit num of qubits: " <<  c_circuit.num_qubits << std::endl;
    print_graph(c_circuit.graph);

    //c_circuit.draw_self();
    return c_circuit;
}

void test_sabre_routing() {
    CouplingCircuit c_circuit = test_c_ciruit();
    DAGCircuit dag = test_dag();

    c_circuit.draw_self();
    SabreRouting sabre_routing{c_circuit};
    sabre_routing.run(dag);
}


int main() {
    std::cout << "---- main function ----" << std::endl;
    // test_c_ciruit();
    // test_sabre_routing();
    test_dag();
}