#pragma once
#include <vector>
#include <tuple>

using CouplingList = std::vector<std::tuple<int, int, float>>;
const CouplingList EMPTY_COUPLING_LIST = {std::make_tuple(0, 0, 0)};