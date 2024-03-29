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
    c_list = [(0, 1,0.95), (1, 0,0.95), (1,2,0.99), (2,3,0.96), (2,1,0.99), (3,2,0.96), (3,4,0.9), (4,3,0.9), (5,4, 0.99), (4,5,0.99)]
    dataDict = DataDict()
    dataDict['coupling_list'] = c_list
    backend_properties = {
        'name': 'ExampleBackend',
        'backend_type': 'superconducting',
        'qubits_num': 4,
        'coupling_list': c_list,
    }
    backend_instance = Backend(**backend_properties)
    model = Model(backend=backend_instance, datadict=dataDict)
    return model

def get_random_qc():
    gate_list = ['cx', 'rxx', 'rzz', 'rx', 'id', 'h']
    rqc = RandomCircuit(num_qubit=6, gates_number=10, gates_list=gate_list)
    qc = rqc.random_circuit()
    # qc.measure([0, 1, 2, 3, 4], [0, 1, 2, 3, 4])
    #qc.plot_circuit()
    #plt.show()

    return qc

def test_coupling():
    c_list = [(0, 1,0.95), (1, 0,0.95), (1,2,0.99), (2,3,0.96), (2,1,0.99), (3,2,0.96), (3,4,0.9), (4,3,0.9), (5,4, 0.99), (4,5,0.99)]
    c_circuit = CouplingCircuit_cpp(c_list)
    # c_circuit.draw_self()
    backend = Backend_cpp(c_list)
    model = Model_cpp(backend)

    return c_circuit

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
    # simple.measure([2], [1])

    dag = circuit_to_dag(simple)
    draw_dag(dag)
    cpp_dag = dag_to_cppDag(dag)
    # cpp_dag.draw_self()

    return dag, cpp_dag; 


if __name__ == "__main__":



    c_circuit = test_coupling()
    model = get_preset_model()
    

    qc = get_random_qc()
    dag = circuit_to_dag(qc)
    dag_cpp = dag_to_cppDag(dag)
    dag_cpp.draw_self()



    
    sabre_routing = SabreRouting()
    sabre_routing.set_model(model)

    print("---- python ----")
    st = time.time()
    sabre_routing.run(dag)
    print(f"python-time: {time.time()-st}")


    model_cpp = Model_cpp() 
    print(sabre_routing.model._layout["initial_layout"].v2p)
    model_cpp.init_layout =  Layout_cpp(sabre_routing.model._layout["initial_layout"].v2p)
    

    sabre_routing_cpp = SabreRouting_cpp(c_circuit)
    sabre_routing_cpp.set_model(model_cpp)

    print("----  c++   ----")
    st = time.time()
    sabre_routing_cpp.run(dag_cpp)
    print(f"cpp-time: {time.time()-st}")