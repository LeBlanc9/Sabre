import sys
sys.path.append("/Users/air/workspace/qusteed")
from qusteed.dag.dag_circuit import DAGCircuit
from qusteed.compiler.transpiler import Transpiler
from qusteed.passes.unroll.unroll_to_basis import UnrollToBasis
from qusteed.passes.unroll.unroll_to_2qubit import UnrollTo2Qubit
from qusteed.passes.model import Model
from qusteed.backends.backend import Backend



from dag_converter import *
from qc_converter import *
from sabre_layout_cpp import SabreLayout_




if __name__ == "__main__":
    import time
    from qusteed.utils.random_circuit import RandomCircuit 
    from qusteed.passes.mapping.layout.sabre_layout import SabreLayout


    basis_gates = ['cx','rx','ry','rz','id','h']
    c_list = [ (0, 1, 0.98), (1, 0, 0.98), (1, 2, 0.97), (2, 1, 0.97),(2, 3, 0.982), (3, 2, 0.982)]


    passflow_cpp = [
                UnrollTo2Qubit(),
                SabreLayout_(),  # heuristic='distance' or 'fidelity', 'mixture'
                UnrollToBasis(basis_gates=basis_gates),
                # GateOptimization(),
            ]

    passflow_python = [
                UnrollTo2Qubit(),
                SabreLayout(),  # heuristic='distance' or 'fidelity', 'mixture'
                UnrollToBasis(basis_gates=basis_gates),
                # GateOptimization(),
    ]

    backend_properties = {
        'name': 'ExampleBackend',
        'backend_type': 'superconducting',
        'qubits_num': 4,
        'coupling_list': c_list,
    }
    backend_instance = Backend(**backend_properties)
    initial_model = Model(backend=backend_instance)


    rqc = RandomCircuit(num_qubit=4, gates_number=3000, gates_list=['cx', 'rx', 'rz', 'ry', 'h'])
    qc = rqc.random_circuit()
    qc.measure([0, 1, 2, 3], [0, 1, 2, 3])   

    st = time.time()
    compiler = Transpiler(passflow_cpp, initial_model)
    compiled_circuit_cpp = compiler.transpile(qc)
    print('qusteed time cpp (s):', time.time() - st)


    st = time.time()
    compiler = Transpiler(passflow_python, initial_model)
    compiled_circuit_python = compiler.transpile(qc)
    print('qusteed time python (s):', time.time() - st)



    from quafu import simulate
    import matplotlib.pyplot as plt

    simu_res = simulate(compiled_circuit_cpp, output="probabilities")
    simu_res.plot_probabilities()
    plt.title("CPP")
    plt.show()


    simu_res_python = simulate(compiled_circuit_python, output="probabilities")
    simu_res_python.plot_probabilities()
    plt.title("Python")

    plt.show()
