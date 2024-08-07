#include <set>
#include "DAG/dag.h"
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_utility.hpp>


// Utils Function
DagGraph reverse_DagGraph(const DagGraph& graph) {
    DagGraph rev_graph;
    boost::copy_graph(boost::make_reverse_graph(graph), rev_graph);
    return rev_graph;
}