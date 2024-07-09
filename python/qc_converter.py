from build.sabre import DAGCircuit as Cpp_DAGCircuit
from build.sabre import InstructionNode as Cpp_InstructionNode

from quafu import QuantumCircuit
from quafu.elements.element_gates import *
from quafu.elements import Barrier, Delay, ControlledGate, MultiQubitGate, QuantumGate, SingleQubitGate, XYResonance


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

