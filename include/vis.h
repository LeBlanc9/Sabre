#pragma once
#include <string>
#include "dag_cuit.h"
#include "coupling.h"


std::string graph_to_dot(DagGraph& graph);
std::string graph_to_dot(CouplingGraph& graph);

void draw_dot(std::string dot_str);