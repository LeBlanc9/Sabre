from build.sabre import DAGCircuit as Cpp_DAGCircuit
from build.sabre import InstructionNode as Cpp_InstructionNode
from build.sabre import Parameter as Cpp_Parameter
from build.sabre import ParameterType as Cpp_ParameterType
from build.sabre import OperatorType


from quafu import QuantumCircuit
from quafu.elements.element_gates import *
from quafu.elements import Barrier, Delay, ControlledGate,  QuantumGate , XYResonance
from quafu.elements.parameters import Parameter, ParameterExpression


def QuantumCircuit_to_cppDag(circuit: QuantumCircuit) -> Cpp_DAGCircuit:
    """
    Converts a QuantumCircuit object to a Cpp_DAGCircuit object.

    Args:
        circuit (QuantumCircuit): The input QuantumCircuit object.

    Returns:
        Cpp_DAGCircuit: The converted Cpp_DAGCircuit object.
    """
    dag = Cpp_DAGCircuit()
    for gate in circuit.gates:
        node = gate_to_cppNode(gate)
        dag.add_instruction_node_end(node) 
          
    dag.measure = circuit.measures

    return dag


def cppDag_to_QuantumCircuit(dag: Cpp_DAGCircuit) -> QuantumCircuit:
    """
    Converts a Cpp_DAGCircuit to a QuantumCircuit.

    Args:
        dag (Cpp_DAGCircuit): The Cpp_DAGCircuit object to be converted.

    Returns:
        QuantumCircuit: The converted QuantumCircuit object.
    """
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



def gate_to_cppNode(gate) -> Cpp_InstructionNode:
    # convert parameters to CppParameter object
    parameters = [] #Cpp_Parameter

    if not isinstance(gate.paras, List):
        gate.paras = [gate.paras] 
    parameters = [para for para in parameters]


    if gate.name in ["delay", "xy"]:
        duration = gate.duration
        unit = gate.unit
    else:
        duration = 0
        unit = ""

    node = Cpp_InstructionNode(gate.name, gate.pos, parameters, duration, unit)
    return node


def parameter_to_cppParameter(para: Parameter) -> Cpp_Parameter:
    if isinstance(para, (float, int)):
        parameter = Cpp_Parameter(Cpp_ParameterType.DOUBLE, para)
    elif isinstance(para, Parameter):
        parameter = Cpp_Parameter(Cpp_ParameterType.PARAMETER, para.name, para.value)
    elif isinstance(para, ParameterExpression):
        parameter = Cpp_Parameter(Cpp_ParameterType.PARAMETER_EXPRESSION, para.name, para.value)

    parameter.funcs = operator_to_cpp_operator(para.funcs)
    parameter.operands = [op for op in para.operands] 
    return parameter


def cppParameter_to_parameter(para: Cpp_Parameter) -> Parameter:
    if para.type == Cpp_ParameterType.DOUBLE:
        return para.value
    elif para.type == Cpp_ParameterType.PARAMETER:
        parameter = Parameter(name=para.name, value=para.value)
        parameter.funcs = cpp_operator_to_operator(parameter.funcs)
        parameter.operands = [op for op in para.operands]
        return parameter
    elif para.type == Cpp_ParameterType.PARAMETER_EXPRESSION:
        return ParameterExpression(para.name, para.value, para.operands, para.funcs)


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


def operator_to_cpp_operator(ops):
    import _operator
    import autograd.numpy as anp

    cpp_ops = []
    for op in ops:
        if op == _operator.add:
            cpp_ops.append(OperatorType.ADD)
        elif op == _operator.sub:
            cpp_ops.append(OperatorType.SUB)
        elif op == _operator.mul:
            cpp_ops.append(OperatorType.MUL)
        elif op == _operator.truediv:
            cpp_ops.append(OperatorType.TRUEDIV)
        elif op == _operator.pow:
            cpp_ops.append(OperatorType.POW)
        elif op == anp.sin:
            cpp_ops.append(OperatorType.SIN)
        elif op == anp.cos:
            cpp_ops.append(OperatorType.COS)        
        elif op == anp.tan:
            cpp_ops.append(OperatorType.TAN)
        elif op == anp.arcsin:
            cpp_ops.append(OperatorType.ARCSIN)
        elif op == anp.arccos:
            cpp_ops.append(OperatorType.ARCCOS)
        elif op == anp.arctan:
            cpp_ops.append(OperatorType.ARCTAN)
        elif op == anp.log:
            cpp_ops.append(OperatorType.LOG)
        elif op == anp.exp:
            cpp_ops.append(OperatorType.EXP)
        else:
            raise ValueError("Operator not supported")
    return cpp_ops


def cpp_operator_to_operator(ops):
    import _operator
    import autograd.numpy as anp

    py_ops = []
    for op in ops:
        if op == OperatorType.ADD:
            py_ops.append(_operator.add)
        elif op == OperatorType.SUB:
            py_ops.append(_operator.sub)
        elif op == OperatorType.MUL:
            py_ops.append(_operator.mul)
        elif op == OperatorType.TRUEDIV:
            py_ops.append(_operator.truediv)
        elif op == OperatorType.POW:
            py_ops.append(_operator.pow)
        elif op == OperatorType.SIN:
            py_ops.append(anp.sin)
        elif op == OperatorType.COS:
            py_ops.append(anp.cos)        
        elif op == OperatorType.TAN:
            py_ops.append(anp.tan)
        elif op == OperatorType.ARCSIN:
            py_ops.append(anp.arcsin)
        elif op == OperatorType.ARCCOS:
            py_ops.append(anp.arccos)
        elif op == OperatorType.ARCTAN:
            py_ops.append(anp.arctan)
        elif op == OperatorType.LOG:
            py_ops.append(anp.log)
        elif op == OperatorType.EXP:
            py_ops.append(anp.exp)
        else:
            raise ValueError("Operator not supported")
    return py_ops


if __name__ == "__main__":
    print("Hello, World!")
    from quafu.elements.parameters import Parameter
    p = Parameter("theta", 3.14)

    cpp_parameter = parameter_to_cppParameter(p)
    parameter = cppParameter_to_parameter(cpp_parameter) 