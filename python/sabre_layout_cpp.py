from build.sabre import SabreLayout as Cpp_SabreLayout
from build.sabre import CouplingCircuit as CouplingCircuit_cpp
from build.sabre import Heuristic


from dag_converter import *
from qc_converter import *



class SabreLayout_():
    """
    SabreLayout_ class represents a layout algorithm for quantum circuits using the SabreLayout technique.

    Args:
        heuristic (str): The heuristic to be used for layout optimization. Possible values are "distance", "fidelity", and "mixture".
        max_iterations (int): The maximum number of iterations for the layout optimization algorithm.

    Attributes:
        sabre_layout: The SabreLayout cpp_object used for layout optimization.
        model: The model of backend.
    """

    def __init__(self, heuristic="distance", max_iterations=3):
        self.sabre_layout = None
        self.model = None

        if heuristic == "distance":
            self.heuristic = Heuristic.DISTANCE
        elif heuristic == "fidelity": 
            self.heuristic = Heuristic.FIDELITY
        elif heuristic == "mixture":
            self.heuristic = Heuristic.MIXTURE

        self.max_iterations = max_iterations


    def set_model(self, model):
        """
        Sets the model of backend.

        Args:
            model: The model to be set.
        """
        self.model = model
        c_list = model.get_backend().get_property("coupling_list") 
        c_circuit = CouplingCircuit_cpp(c_list)

        self.sabre_layout = Cpp_SabreLayout(c_circuit)
        self.sabre_layout.heuristic = self.heuristic
        self.sabre_layout.max_iterations = self.max_iterations


    def run(self, dag):
        """
        Runs the layout optimization algorithm on the given DAG circuit.

        Args:
            dag: The DAG circuit to be optimized.

        Returns:
            The optimized DAG circuit.
        """
        if isinstance(dag, DAGCircuit):
            dag = dag_to_cppDag(dag)
            self.sabre_layout.run(dag)
            return cppDag_to_dag(dag)

        elif isinstance(dag, QuantumCircuit):
            dag = QuantumCircuit_to_cppDag(dag)
            self.sabre_layout.run(dag)
            return cppDag_to_QuantumCircuit(dag)
