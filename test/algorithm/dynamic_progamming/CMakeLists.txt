project(dynamic_progamming)

file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src *.cpp)

#add_library(IniParser src/IniParser.cpp)

add_library(dp_lib ${SOURCE})

set_target_properties(dp_lib PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set_target_properties(dp_lib PROPERTIES 
LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

target_include_directories(dp_lib PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/include)


add_executable(dp_demo main.cpp)

target_include_directories(dp_demo PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(dp_demo PUBLIC dp_lib)


