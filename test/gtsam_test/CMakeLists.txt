cmake_minimum_required(VERSION 3.16)
project(my_gtsam_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(GPMP2_SOURCE_DIR "D:/2026/robotcontrolAlgorithm/stl_learning/src/extend/gpmp2")

#file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src *.cpp)

find_package(GTSAM REQUIRED
  PATHS "D:/2026/robotcontrolAlgorithm/stl_learning/src/extend/gtsam/build-mingw"
  NO_DEFAULT_PATH
)

#add_library(gpmp2_test ${SOURCE})

# target_include_directories(gpmp2_test PUBLIC
#   ${CMAKE_CURRENT_SOURCE_DIR}/include
#   #${GPMP2_SOURCE_DIR}
#   ${GTSAM_INCLUDE_DIR}
# )
# target_link_libraries(gpmp2_test PUBLIC gtsam)

add_executable(my_gtsam_app main.cpp)

set_target_properties(my_gtsam_app PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

target_include_directories(my_gtsam_app PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  #${GPMP2_SOURCE_DIR}
  ${GTSAM_INCLUDE_DIR}
)
target_link_libraries(my_gtsam_app PRIVATE gtsam )









