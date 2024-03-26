#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <stdexcept>

using Layout = std::unordered_map<int, int>;
Layout generate_random_layout(int virtual_qubits, int physical_qubits);