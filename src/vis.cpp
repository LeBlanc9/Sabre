#include "vis.h"
#include <iostream>

void draw_graph(Graph& graph) {
    GVC_t *gvc = gvContext();

    std::cout << "drawing" << std::endl;

    //  Boost 图  ->  Graphviz 图形  ->  dot_str
    std::ostringstream os;
    boost::write_graphviz(os, graph,
                            boost::make_label_writer(boost::get(&InstructionNode::name, graph)));
                                
    std::string dot_str = os.str();

    // 使用 agmemread() 读取 Graphviz 字符串, c_str() in Temporary buffer
    Agraph_t *g = agmemread(dot_str.c_str());

    // 设置布局引擎
    gvLayout(gvc, g, "dot");

    // 渲染图像并保存为 PNG 文件
    gvRenderFilename(gvc, g, "png", "output.png");

    // 释放布局资源
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
}