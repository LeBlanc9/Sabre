from build.sabre import SabreRouting as Cpp_SabreRouting
from build.sabre import CouplingCircuit as CouplingCircuit_cpp
from build.sabre import Heuristic

from dag_converter import *
from qc_converter import *


class SabreRouting_():
    """
    A class representing the SabreRouting algorithm.

    Args:
        heuristic (str): The heuristic to be used for routing. Can be one of "distance", "fidelity", or "mixture".

    Attributes:
        model: The model of backend.
        sabre_routing: The SabreRouting cpp_object.
        heuristic: The selected heuristic method.
    """

    def __init__(self, heuristic="distance"):
        self.model = None
        self.sabre_routing = None
        self.heuristic = heuristic

        if heuristic == "distance":
            self.heuristic = Heuristic.DISTANCE
        elif heuristic == "fidelity": 
            self.heuristic = Heuristic.FIDELITY
        elif heuristic == "mixture":
            self.heuristic = Heuristic.MIXTURE


    def set_model(self, model):
        """
        Set the model of backend.

        Args:
            model: The model to be set.
        """
        self.model = model
        c_list = model.get_backend().get_property("coupling_list") 
        c_circuit = CouplingCircuit_cpp(c_list)

        self.sabre_routing = Cpp_SabreRouting(c_circuit)
        self.sabre_layout.heuristic = self.heuristic


    def run(self, dag):
        """
        Run the SabreRouting algorithm on the given DAG.

        Args:
            dag: The DAG to be routed.

        Returns:
            The routed DAG.
        """
        if isinstance(dag, DAGCircuit):
            dag = dag_to_cppDag(dag)
            self.sabre_routing.run(dag)
            return cppDag_to_dag(dag)

        elif isinstance(dag, QuantumCircuit):
            dag = QuantumCircuit_to_cppDag(dag)
            self.sabre_routing.run(dag)
            return cppDag_to_QuantumCircuit(dag)


