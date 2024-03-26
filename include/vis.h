#pragma once
#include <string>
#include "dag.h"
#include "coupling.h"

std::string vectorToString(const std::vector<int>& vec);

std::string graph_to_dot(const DagGraph& graph);
std::string graph_to_dot(const CouplingGraph& graph);

void draw_dot(const std::string dot_str);

// Graph = @c DagGraph or @c CouplingGraph
template<typename Graph>
void draw_graph(const Graph& graph) { 
    draw_dot(graph_to_dot(graph));
}