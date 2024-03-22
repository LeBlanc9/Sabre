from build.sabre import Backend, Model, CouplingCircuit
from build.sabre import DAGCircuit as Cpp_DAGCircuit
from build.sabre import InstructionNode as Cpp_InstructionNode

import numpy as np

import sys
sys.path.append("/Users/air/workspace/qusteed")
from qusteed.dag.dag_circuit import DAGCircuit
from qusteed.dag.circuit_dag  import circuit_to_dag, dag_to_circuit, draw_dag
from qusteed.dag.instruction_node import InstructionNode 
from quafu import QuantumCircuit

from utils import *


def test_coupling():
    c_list = [(0, 1,0.95), (1, 0,0.95), (1,2,0.99), (2,3,0.96), (2,1,0.99), (3,2,0.96), (3,4,0.9),(4,3,0.9)]
    c_circuit = CouplingCircuit(c_list)
    c_circuit.draw_self()
    backend = Backend(c_list)
    model = Model(backend)


def test_dag(): 
    simple = QuantumCircuit(3)
    simple.cnot(0, 1)
    simple.cnot(1, 0)
    simple.x(2)
    simple.cnot(2, 1)
    simple.cp(1, 0,np.pi / 2)
    simple.barrier([0, 1, 2])
    simple.rxx(0, 1, np.pi / 2)
    simple.delay(0, 100)
    simple.fredkin(0,1, 2)
    simple.mcx([0, 1], 2)
    # simple.measure([0], [0])
    # simple.measure([1], [2])
    simple.measure([2], [1])

    dag = circuit_to_dag(simple)

    cpp_dag = dag_to_cppDag(dag)
    cpp_dag.draw_self()


if __name__ == "__main__":
    test_coupling()