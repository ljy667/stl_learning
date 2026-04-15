cmake_minimum_required(VERSION 3.16)
project(my_gtsam_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(GTSAM REQUIRED
  PATHS "D:/2026/robotcontrolAlgorithm/stl_learning/src/extend/gtsam/build-mingw"
  NO_DEFAULT_PATH
)

add_executable(my_gtsam_app main.cpp)

set_target_properties(my_gtsam_app PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

target_include_directories(my_gtsam_app PRIVATE ${GTSAM_INCLUDE_DIR})
target_link_libraries(my_gtsam_app PRIVATE gtsam)










