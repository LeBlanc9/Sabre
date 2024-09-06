## Project Setting
cmake_minimum_required(VERSION 3.15...3.29)
project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type not set - defaulting to Release")
    set(CMAKE_BUILD_TYPE Release)
endif()


# Find Boost
find_package(Boost REQUIRED)
if(Boost_FOUND)
  if(NOT TARGET Boost::headers)
    add_library(Boost::headers IMPORTED INTERFACE)
    if(TARGET Boost::boost)
      # Classic FindBoost
      set_property(TARGET Boost::boost PROPERTY INTERFACE_LINK_LIBRARIES Boost::boost)
    else()
      # Very old FindBoost, or newer Boost than CMake in older CMakes
      set_property(TARGET Boost::headers PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
    endif()
  endif()
endif()


# Find Python
find_package(Python REQUIRED COMPONENTS Interpreter Development)
message(STATUS "Python Interpreter: ${Python_EXECUTABLE}")
message(STATUS "Python Version: ${Python_VERSION}")
message(STATUS "Python Include Dir: ${Python_INCLUDE_DIRS}")
message(STATUS "Python Libraries: ${Python_LIBRARIES}")

# Find pybind11
add_subdirectory(extern/pybind11)


## Test CPP  设置包含src目录下除了binding.cpp之外的所有文件
file(GLOB SOURCES "src/*.cpp")
file(GLOB DAG_SOURCES "src/DAG/*.cpp")
file(GLOB Vis_SOURCES "src/Vis/*.cpp")
file(GLOB Model_SOURCES "src/Model/*.cpp")

set(target sabre)
pybind11_add_module(
    ${target} 
        src/Binding/binding.cpp 
        ${SOURCES} 
        ${DAG_SOURCES}
        ${Model_SOURCES}
)
target_include_directories(
    ${target} 
    PRIVATE 
        "include" 
        "include/DAG" 
        "include/Vis"
        "include/Model"
)    


target_link_libraries(sabre PRIVATE pybind11::headers)


if(Boost_FOUND)
    target_link_libraries(${target} PRIVATE Boost::headers)
    target_compile_definitions(${target} PRIVATE BOOST_NO_CXX98_FUNCTION_BASE)
endif()


# Install
install(TARGETS sabre LIBRARY DESTINATION .)