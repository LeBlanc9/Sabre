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
        # print(f"index:{index}, node:{node}") 
        cpp_node = node_to_cppNode(node[0])
        cppDag.add_node(cpp_node)

    node_to_index = {node_name: index for index, node_name in enumerate(dag.nodes())}
    # Add edge
    for u, v, data in dag.edges(data=True):
        #print(f"Edge from {u} to {v} with label {data['label']}")
        ep = EdgeProperties(int(data['label'][1]))
        cppDag.add_edge(node_to_index[u], node_to_index[v], ep)

    return cppDag


def node_to_cppNode(node: InstructionNode) -> Cpp_InstructionNode:
    cpp_node = Cpp_InstructionNode()

    # start_node & end_node
    if node == -1:
        cpp_node.name = "start"
        return cpp_node
    if node == float('inf'):
        cpp_node.name = "end"
        return cpp_node
    else:
        cpp_node.name = node.name 

    # pass pos
    if isinstance(node.pos, list):
        cpp_node.qubit_pos = node.pos
    elif isinstance(node.pos, dict):
        cpp_node.qubit_pos = list(node.pos.keys())
        cpp_node.classic_pos = list(node.pos.values())

    return cpp_node