##! Project Setting
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 11)
project(Sabre)

##! Python Setting
set(Python_FIND_STRATEGY LOCATION)
set(Python_ROOT_DIR /Users/air/anaconda3/envs/quantum)
find_package(Python 3.10 COMPONENTS Interpreter Development REQUIRED)


include_directories("include" "extern/pybind11/include")
##! Test CPP
add_executable(sabre_cpp src/main.cpp src/backend.cpp src/model.cpp src/sabre_routing.cpp src/sabre_layout.cpp)


##! Test pybind
add_subdirectory("extern/pybind11")
pybind11_add_module(sabre src/binding.cpp src/backend.cpp src/model.cpp)