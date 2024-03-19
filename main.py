from build.sabre import Backend, Model
from build.sabre import DAGCircuit as Cpp_DAGCircuit
from build.sabre import InstructionNode as Cpp_InstructionNode

import numpy as np

import sys
sys.path.append("/Users/air/workspace/qusteed")
from qusteed.dag.dag_circuit import DAGCircuit
from qusteed.dag.circuit_dag  import circuit_to_dag, dag_to_circuit, draw_dag
from qusteed.dag.instruction_node import InstructionNode 

from quafu import QuantumCircuit



def test(): 
    #c_list = [(0, 1,0.95), (1, 0,0.95), (1,2,0.99), (2,3,0.96), (2,1,0.99), (3,2,0.96), (3,4,0.9),(4,3,0.9)]
    #backend = Backend(c_list)
    #model = Model(backend)

    # cpp_dag = Cpp_DAGCircuit()
    # cpp_dag.add_node(Cpp_InstructionNode("testNode"))
    # cpp_dag.draw_self()

    simple = QuantumCircuit(3)
    simple.cnot(0, 1)
    simple.cnot(1, 0)
    simple.x(2)
    simple.cnot(2, 1)
    simple.cp(1, 0,np.pi / 2)
    simple.barrier([0, 1, 2])

    dag = circuit_to_dag(simple)

    cpp_dag = dag_to_cppDag(dag)
    cpp_dag.draw_self()


def dag_to_cppDag(dag: DAGCircuit) -> Cpp_DAGCircuit:
    cppDag = Cpp_DAGCircuit();

    for node in dag.nodes(data=True):
        cpp_node = node_to_cppNode(node[0])
        cppDag.add_node(cpp_node)
    return cppDag 


def node_to_cppNode(node: InstructionNode) -> Cpp_InstructionNode:
    cpp_node = Cpp_InstructionNode()

    ##* start_node & end_node
    if node == -1:
        cpp_node.name = "start"
        cpp_node.label = -1
        return cpp_node
    if node == float('inf'):
        cpp_node.name = "end"
        cpp_node.label = -2
        return cpp_node

    ##* pass pos
    cpp_node.name = node.name 
    if isinstance(node.pos, list):
        cpp_node.pos = node.pos
    elif isinstance(node.pos, dict):
        cpp_node.measure_pos = node.pos

    return cpp_node


if __name__ == "__main__":
    test()