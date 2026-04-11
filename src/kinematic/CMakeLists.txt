cmake_minimum_required(VERSION 3.14)
project(kinematic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)



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

add_executable(kinematic_demo
    src/demo.cpp
)

target_link_libraries(kinematic_demo
    PUBLIC Eigen3::Eigen
)
