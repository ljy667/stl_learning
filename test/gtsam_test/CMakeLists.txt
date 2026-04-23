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
if(NOT TARGET Eigen3::Eigen)
  find_package(Eigen3 REQUIRED)
endif()

#add_library(gpmp2_test ${SOURCE})

# target_include_directories(gpmp2_test PUBLIC
#   ${CMAKE_CURRENT_SOURCE_DIR}/include
#   #${GPMP2_SOURCE_DIR}
#   ${GTSAM_INCLUDE_DIR}
# )
# target_link_libraries(gpmp2_test PUBLIC gtsam)


file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)


add_library( gpmp2_lib STATIC ${SOURCES})


add_executable(my_gtsam_app
  main.cpp
)

set_target_properties(my_gtsam_app gpmp2_lib PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set_target_properties(my_gtsam_app gpmp2_lib PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)



target_include_directories(gpmp2_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  #${GPMP2_SOURCE_DIR}
  ${GTSAM_INCLUDE_DIR}
)
target_link_libraries(gpmp2_lib PUBLIC gtsam Eigen3::Eigen)

target_include_directories(my_gtsam_app  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  #${GPMP2_SOURCE_DIR}
  ${GTSAM_INCLUDE_DIR}
)
target_link_libraries(my_gtsam_app PRIVATE gpmp2_lib)

#add_test(NAME gtsam_arm_fk_test COMMAND my_gtsam_app)







