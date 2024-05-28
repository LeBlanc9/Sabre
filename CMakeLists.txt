##! Project Setting
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

project(Sabre)

##! Python Setting
set(Python_FIND_STRATEGY LOCATION)
set(Python_ROOT_DIR /Users/air/anaconda3/envs/quantum)
find_package(Python 3.10 COMPONENTS Interpreter Development REQUIRED)

##! Boost lib
set(Boost_ROOT "/opt/homebrew/opt/boost")
find_package(Boost REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
endif()

##! Graphviz lib
set(Graphviz_INCLUDE_DIRS "/opt/homebrew/Cellar/graphviz/10.0.1/include/")
include_directories(${Graphviz_INCLUDE_DIRS})
link_directories("/opt/homebrew/Cellar/graphviz/10.0.1/lib")


include_directories("extern/pybind11/include")
include_directories("include" "include/DAG")
##! Test CPP  设置包含src目录下除了binding.cpp之外的所有文件
file(GLOB SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "src/binding.cpp")
file(GLOB DAG_SOURCES "src/DAG/*.cpp")

add_executable(sabre_cpp ${SOURCES} ${DAG_SOURCES})
target_link_libraries(sabre_cpp gvc cgraph)         # Graphviz
target_link_libraries(sabre_cpp ${Boost_LIBRARIES}) # Boost


##! Test pybind
add_subdirectory("extern/pybind11")
pybind11_add_module(sabre src/binding.cpp ${SOURCES} ${DAG_SOURCES})
target_link_libraries(sabre PRIVATE gvc cgraph)         # Graphviz
target_link_libraries(sabre PRIVATE ${Boost_LIBRARIES}) # Boost
