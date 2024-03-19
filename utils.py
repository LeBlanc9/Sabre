from build.sabre import DAGCircuit as Cpp_DAGCircuit
from build.sabre import InstructionNode as Cpp_InstructionNode
from build.sabre import EdgeProperties

import sys
sys.path.append("/Users/air/workspace/qusteed")
from qusteed.dag.dag_circuit import DAGCircuit
from qusteed.dag.circuit_dag  import circuit_to_dag, dag_to_circuit, draw_dag
from qusteed.dag.instruction_node import InstructionNode 
from quafu import QuantumCircuit


def qc_to_cppDag():
    # TODO:
    pass


def dag_to_cppDag(dag: DAGCircuit) -> Cpp_DAGCircuit:
    cppDag = Cpp_DAGCircuit();

    # Add node
    for index, node in enumerate(dag.nodes(data=True)):
        cpp_node = node_to_cppNode(node[0], index)
        cppDag.add_node(cpp_node)

    # Add edge
    for u, v, data in dag.edges(data=True):
        ep = EdgeProperties(int(data['label'][1]))
        #print(f"Edge from {u} to {v} with label {data['label']}")
        if u == -1: 
            start=0
        else: start = u.label+1

        if v == float("inf"): 
            end = cppDag.get_num_nodes() -1
        elif v.label != 'm':
            end = v.label+1

        cppDag.add_edge(start, end, ep)
    return cppDag 


def node_to_cppNode(node: InstructionNode, index=0) -> Cpp_InstructionNode:
    cpp_node = Cpp_InstructionNode()

    # start_node & end_node
    if node == -1:
        cpp_node.name = "start"
        cpp_node.id = -1
        return cpp_node
    if node == float('inf'):
        cpp_node.name = "end"
        cpp_node.id = -2
        return cpp_node
    else:
        cpp_node.id = index - 1
        cpp_node.name = node.name 


    # pass pos
    if isinstance(node.pos, list):
        cpp_node.pos = node.pos
    elif isinstance(node.pos, dict):
        cpp_node.measure_pos = node.pos

    return cpp_node
