from build.sabre import CouplingCircuit
from build.sabre import Backend as Backend_cpp
from build.sabre import Model as Model_cpp
from build.sabre import SabreLayout as SabreLayout_cpp 
from build.sabre import SabreRouting as SabreRouting_cpp
from build.sabre import DAGCircuit as DAGCircuit_cpp
from build.sabre import CouplingCircuit as CouplingCircuit_cpp
from build.sabre import InstructionNode as InstructionNode_cpp
from build.sabre import Layout as Layout_cpp

import numpy as np
import time

import sys
sys.path.append("/Users/air/workspace/qusteed")
from qusteed.dag.dag_circuit import DAGCircuit
from qusteed.passes.mapping.routing.sabre_routing import SabreRouting
from qusteed.passes.model import Model
from qusteed.backends.backend import Backend
from qusteed.passes.datadict import DataDict
from qusteed.dag.circuit_dag  import circuit_to_dag, dag_to_circuit, draw_dag
from qusteed.dag.instruction_node import InstructionNode 
from qusteed.graph.couplinggraph import CouplingGraph
from qusteed.utils.random_circuit import RandomCircuit 
from quafu import QuantumCircuit

from utils import *

def get_preset_model():
    # c_list = [(0, 1,0.95), (1, 0,0.95), (1,2,0.99), (2,3,0.96), (2,1,0.99), (3,2,0.96), (3,4,0.9), (4,3,0.9), (5,4, 0.99), (4,5,0.99), (1,6, 0.99), (6,1, 0.99), (7,4, 0.99), (4,7,0.99),(5,8, 0.99), (8,5,0.99), (4,9,0.98), (9,4,0.98)]
    c_list = [(0, 1,0.95), (1, 0,0.95), (1,2,0.99), (2,3,0.96), (2,1,0.99), (3,2,0.96)] 
    dataDict = DataDict()
    dataDict['coupling_list'] = c_list
    backend_properties = {
        'name': 'ExampleBackend',
        'backend_type': 'superconducting',
        'qubits_num': 3,
        'coupling_list': c_list,
    }
    backend_instance = Backend(**backend_properties)
    model = Model(backend=backend_instance, datadict=dataDict)
    return model

def get_random_qc():
    gate_list = ['cx', 'rxx', 'h']
    rqc = RandomCircuit(num_qubit=4, gates_number = 500, gates_list=gate_list)
    qc = rqc.random_circuit()
    qc.measure([0, 1, 2, 3], [0, 1, 2, 3])
    #qc.plot_circuit()
    #plt.show()

    return qc

def test_coupling():
    # c_list = [(0, 1,0.95), (1, 0,0.95), (1,2,0.99), (2,3,0.96), (2,1,0.99), (3,2,0.96), (3,4,0.9), (4,3,0.9), (5,4, 0.99), (4,5,0.99), (1,6, 0.99), (6,1, 0.99), (7,4, 0.99), (4,7,0.99),(5,8, 0.99), (8,5,0.99), (4,9,0.98), (9,4,0.98)]
    c_list = [(0, 1,0.95), (1, 0,0.95), (1,2,0.99), (2,3,0.96), (2,1,0.99), (3,2,0.96)] 
    c_circuit = CouplingGraph(c_list)
    c_circuit_cpp = CouplingCircuit_cpp(c_list)
    # c_circuit.draw_self()
    backend = Backend_cpp(c_list)
    model = Model_cpp(backend)

    return c_circuit, c_circuit_cpp

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
    simple.measure([0], [0])
    simple.measure([1], [2])
    simple.measure([2], [1])

    # dag = circuit_to_dag(simple)
    # draw_dag(dag)

    dag = QuantumCircuit_to_cppDag(simple)
    # qc = cppDag_to_QuantumCircuit(dag)


    return simple, dag



if __name__ == "__main__":
    c_circuit, c_circuit_cpp = test_coupling()
    model = get_preset_model()

    qc = get_random_qc()
    dag = circuit_to_dag(qc)

    dag_cpp = QuantumCircuit_to_cppDag(qc)

    sabre_layout = SabreLayout_cpp(c_circuit_cpp)
    dag_cpp = sabre_layout.run(dag_cpp)





    