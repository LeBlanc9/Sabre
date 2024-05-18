#include <istream>
#include "DAG/instructionNode.h"
#include "vis.h"


std::ostream& operator<< (std::ostream& os, const InstructionNode& node) {
    os << "{" << node.name; 
    os << "("<< vectorToString(node.qubit_pos);
    os << ")" << "}";
    return os;
}
