#include "vis.h"
#include <iostream>
#include <sstream>
#include <vector>

std::string vectorToString(const std::vector<int>& vec) {
    std::ostringstream ss;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0)  ss << ",";
        ss << vec[i]; 
    }
    return ss.str();
}

class DagNodeWriter {
public:
    DagGraph g;
    DagNodeWriter(DagGraph g) : g(g) {}

    template <typename VertexDescriptor, typename OutputStream>
    void operator()(OutputStream& os, VertexDescriptor v) const {
        const auto& node = g[v];
        os << "[label=\"" << "{" << node.name << "("<< vectorToString(node.qubit_pos) << ")" << "}" << "\"]";
    }
};

class DagEdgeWriter {
public:
    DagGraph g;
    DagEdgeWriter(DagGraph g) : g(g) {}

    template <typename EdgeDescriptor, typename OutputStream>
    void operator()(OutputStream& os, EdgeDescriptor e) const {
        const auto& edge = g[e];
    os << "[label=\"" << "q" << edge.id << "\"]";
    }
};


class CouplingNodeWriter {
public:
    CouplingGraph g;
    CouplingNodeWriter(CouplingGraph g) : g(g) {}

    template <typename VertexDescriptor, typename OutputStream>
    void operator()(OutputStream& os, VertexDescriptor v) const {
        const auto& node = g[v];
        os << "[label=\"" << node.id << "\"]";
    }
};

class CouplingEdgeWriter {
public:
    CouplingGraph g;
    CouplingEdgeWriter(CouplingGraph g) : g(g) {}

    template <typename EdgeDescriptor, typename OutputStream>
    void operator()(OutputStream& os, EdgeDescriptor e) const {
        const auto& edge = g[e];
        os << "[label=\"" << edge.fidelity << "\"]";
    }
};

std::string graph_to_dot(DagGraph& graph) {
    std::ostringstream os;
    boost::write_graphviz(os, graph, DagNodeWriter(graph), DagEdgeWriter(graph));
    return os.str();
}

std::string graph_to_dot(CouplingGraph& graph) {
    std::ostringstream os;
    boost::write_graphviz(os, graph, CouplingNodeWriter(graph), CouplingEdgeWriter(graph));
    return os.str();
}


void draw_dot(std::string dot_str) {
    std::cout << "-- drawing" << std::endl;

    // save .dot file
    std::ofstream dot_file("output.dot");
    dot_file << dot_str;
    dot_file.close();

    GVC_t *gvc = gvContext();
    // 使用 agmemread() 读取 Graphviz 字符串, c_str() in Temporary buffer
    Agraph_t *g = agmemread(dot_str.c_str());

    // 设置布局引擎
    gvLayout(gvc, g, "dot");

    // Render img & save as .pnd
    gvRenderFilename(gvc, g, "png", "output.png");

    // Free layout resource
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
}