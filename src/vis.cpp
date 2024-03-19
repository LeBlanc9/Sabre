#include "vis.h"
#include <iostream>
#include <string>
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


class InstructionNodeWriter {
public:
    Graph g;
    InstructionNodeWriter(Graph g) : g(g) {}

    template <typename VertexDescriptor, typename OutputStream>
    void operator()(OutputStream& os, VertexDescriptor v) const {
        const auto& node = g[v];
        os << "[label=\"" << "{" << node.id << node.name << "("<< vectorToString(node.pos) << ")" << "}" << "\"]";
    }
};

class EdgeWriter {
public:
    Graph g;
    EdgeWriter(Graph g) : g(g) {}

    template <typename EdgeDescriptor, typename OutputStream>
    void operator()(OutputStream& os, EdgeDescriptor e) const {
        const auto& ep = g[e];
        os << "[label=\"" << "q" << ep.id << "\"]";
    }
};



void draw_graph(Graph& graph) {
    GVC_t *gvc = gvContext();

    std::cout << "drawing" << std::endl;

    //  Boost -> Graphviz -> dot_str
    std::ostringstream os;

    boost::write_graphviz(os, graph, InstructionNodeWriter(graph), EdgeWriter(graph));
                                
    std::string dot_str = os.str();

    // save .dot file
    std::ofstream dot_file("output.dot");
    dot_file << dot_str;
    dot_file.close();

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