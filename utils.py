from build.sabre import DAGCircuit as Cpp_DAGCircuit
from build.sabre import InstructionNode as Cpp_InstructionNode
from build.sabre import MeasureNode as Cpp_MeasureNode
from build.sabre import EdgeProperties

import sys
sys.path.append("/Users/air/workspace/qusteed")
from qusteed.dag.dag_circuit import DAGCircuit
from qusteed.dag.circuit_dag  import circuit_to_dag, dag_to_circuit, draw_dag
from qusteed.dag.instruction_node import InstructionNode 


from quafu import QuantumCircuit
from quafu.elements.element_gates import *
from quafu.elements import Barrier, Delay, ControlledGate, MultiQubitGate, QuantumGate, SingleQubitGate, XYResonance
from quafu.elements.quantum_gate import ParametricGate


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
    
    if node == -1:
        cpp_node = Cpp_InstructionNode()
        cpp_node.name = "start"
        return cpp_node
    elif node == float('inf'):
        cpp_node = Cpp_InstructionNode()
        cpp_node.name = "end"
        return cpp_node

    elif isinstance(node.pos, list):
        cpp_node = Cpp_InstructionNode()
        cpp_node.name = node.name 
        cpp_node.qubit_pos = node.pos

        if node.paras:
            cpp_node.paras = node.paras
        if node.duration:
            cpp_node.duration = node.duration
        if  node.unit:
            cpp_node.unit = node.unit
        return cpp_node

    elif isinstance(node.pos, dict):
        if node.pos:
            cpp_node = Cpp_MeasureNode(node.pos.keys(), node.pos.values())
        else:
            cpp_node = Cpp_MeasureNode()
        return cpp_node


def cppDag_to_dag(cppDag: Cpp_DAGCircuit) -> DAGCircuit:
    # Starting Label Index
    i = 0

    # A dictionary to store the last use of any qubit
    qubit_last_use = {}

    # Add the start node 
    dag = DAGCircuit()
    dag.add_nodes_from([(-1, {"color": "green"})])

    for vertex in cppDag.vertices():
        if cppDag.graph[vertex].name == "start" or cppDag.graph[vertex].name == "end":
            continue
        else:
            hashable_gate = cppNode_to_node(cppDag.graph[vertex], i)
            i += 1

            dag.add_node(hashable_gate, color="blue") 

            for qubit in hashable_gate.pos:
                if qubit in qubit_last_use:
                    dag.add_edge(qubit_last_use[qubit], hashable_gate, label=f'q{qubit}')
                else:
                    dag.add_edge(-1, hashable_gate, label=f'q{qubit}', color="green")

                qubit_last_use[qubit] = hashable_gate


    dag.add_nodes_from([(float('inf'), {"color": "red"})])
    for qubit in qubit_last_use:
        dag.add_edge(qubit_last_use[qubit], float('inf'), label=f'q{qubit}', color="red")

    dag.update_qubits_used()
    dag.update_cbits_used()
    dag.update_num_instruction_nodes()

    return dag


def cppNode_to_node(cpp_node: Cpp_InstructionNode, specific_label) -> InstructionNode:
    node = InstructionNode(cpp_node.name, cpp_node.qubit_pos, cpp_node.paras, cpp_node.duration, cpp_node.unit, label=specific_label)
    return node




def QuantumCircuit_to_cppDag(circuit: QuantumCircuit) -> Cpp_DAGCircuit:
    dag = Cpp_DAGCircuit()

    for gate in circuit.gates:
        node = gate_to_cppNode(gate)
        dag.add_instruction_node_end(node) 
          
    dag.measure = circuit.measures

    return dag


def gate_to_cppNode(gate) -> Cpp_InstructionNode:
    if gate.paras == None:
        paras = []
    else:
        paras = gate.paras      

    if gate.name in ["delay", "xy"]:
        duration = gate.duration
        unit = gate.unit
    else:
        duration = 0
        unit = ""

    node = Cpp_InstructionNode(gate.name, gate.pos, paras, duration, unit)
    return node


# Convert CppDAG to Quantum Circuit
def cppDag_to_QuantumCircuit(dag: Cpp_DAGCircuit) -> QuantumCircuit:
    q_circuit = QuantumCircuit(len(dag.get_qubits_used()))
    for vertex in dag.vertices():
        if dag.graph[vertex].name == "start" or dag.graph[vertex].name == "end":
            continue
        else:
            gate = cppNode_to_gate(dag.graph[vertex])
            q_circuit.gates.append(gate)

    if dag.measure:
        # print(dag.measure)
        q_circuit.measure(list(dag.measure.keys()), list(dag.measure.values()))

    return q_circuit


def cppNode_to_gate(node: Cpp_InstructionNode):
    gate_name = node.name.lower()
    gate_class = gate_classes.get(gate_name)


    if not gate_class:
        raise ValueError("gate is not supported")

    if gate_name == "barrier":
        return gate_class(node.qubit_pos)

    args = node.qubit_pos
    if node.paras :
        args += node.paras


    # 处理 gate.duration 和 gate.unit
    if gate_name in ["delay", "xy"]:
        args.append(node.duration)
        args.append(node.unit)

    # 处理多量子比特门
    if gate_name in ["mcx", "mcy", "mcz"]:
        control_qubits = node.qubit_pos[:-1]
        target_qubit = node.qubit_pos[-1]
        return gate_class(control_qubits, target_qubit)

    # print("gate_name: ", gate_name)
    # print("gate_class: ", gate_class)
    # print("args: ", args)
    return gate_class(*args)
  


# For Conversion of nodes to Quantum Gates.
gate_classes = {
    "x": XGate,
    "y": YGate,
    "z": ZGate,
    "h": HGate,
    "s": SGate,
    "sdg": SdgGate,
    "t": TGate,
    "tdg": TdgGate,
    "rx": RXGate,
    "ry": RYGate,
    "rz": RZGate,
    "id": IdGate,
    "sx": SXGate,
    "sy": SYGate,
    "w": WGate,
    "sw": SWGate,
    "p": PhaseGate,
    "delay": Delay,
    "barrier": Barrier,
    "cx": CXGate,
    "cp": CPGate,
    "swap": SwapGate,
    "rxx": RXXGate,
    "ryy": RYYGate,
    "rzz": RZZGate,
    "cy": CYGate,
    "cz": CZGate,
    "cs": CSGate,
    "ct": CTGate,
    "xy": XYResonance,
    "ccx": ToffoliGate,
    "cswap": FredkinGate,
    "mcx": MCXGate,
    "mcy": MCYGate,
    "mcz": MCZGate,
}




from build.sabre import SabreLayout as Cpp_SabreLayout
from build.sabre import Model as Cpp_Model

class SabreLayout_():
    def __init__(self) -> None:
        self.sabre_layout = Cpp_SabreLayout()

    def set_model(self, model):
        cpp_model = Cpp_Model()
        cpp_model.couple_list = model.couple_list


    def run(self, dag):
        self.sabre_layout.run(dag)



if __name__ == "__main__":
    pass