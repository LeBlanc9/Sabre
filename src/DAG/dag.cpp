#include <set>
#include "DAG/dag.h"
#include "vis.h"
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_utility.hpp>



void DAGCircuit::draw_self() const { 
    draw_dot(graph_to_dot(this->graph)); 
}

void DAGCircuit::print_self() const { 
    print_graph(this->graph);
}


// Utils Function
DagGraph reverse_DagGraph(const DagGraph& graph) {
    DagGraph rev_graph;
    boost::copy_graph(boost::make_reverse_graph(graph), rev_graph);
    return rev_graph;
}