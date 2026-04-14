project(cxx_utils)

file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src *.cpp)

#add_library(IniParser src/IniParser.cpp)

add_library(cxx_utils_lib ${SOURCE})

set_target_properties(cxx_utils_lib PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)


target_include_directories(cxx_utils_lib PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/include)


add_executable(cxx_demo main.cpp)

target_include_directories(cxx_demo PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(cxx_demo PUBLIC cxx_utils_lib)