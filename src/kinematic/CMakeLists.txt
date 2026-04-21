cmake_minimum_required(VERSION 3.16)
project(kinematic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 把下面路径改成你的 py38_812 环境路径！
set(Python3_ROOT_DIR "D:/robot_ws/conda/envs/py38_812")
set(Python3_EXECUTABLE "D:/robot_ws/conda/envs/py38_812/python.exe")
set(Python3_LIBRARY "D:/robot_ws/conda/envs/py38_812/libs/python38.lib")
set(Python3_INCLUDE_DIR "D:/robot_ws/conda/envs/py38_812/include")

# 查找Python和PyBind11
find_package(Python3 3.8 REQUIRED COMPONENTS Interpreter Development)

find_package(pybind11 REQUIRED)
 

add_library(kinematic STATIC
    src/car_control.cpp
)

target_include_directories(kinematic
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(kinematic
    PUBLIC Eigen3::Eigen
)


# 创建模块
pybind11_add_module(M20id35_CarDist src/M20id35_CarDist.cpp)

target_link_libraries(M20id35_CarDist
    PUBLIC Eigen3::Eigen
)


set_target_properties(M20id35_CarDist PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(M20id35_CarDist PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)