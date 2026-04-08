project(IniParser)

file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src *.cpp)

#add_library(IniParser src/IniParser.cpp)

add_library(string_splits ${SOURCE})

set_target_properties(string_splits PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)



target_include_directories(string_splits PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/include)



add_executable(IniParser_demo src/demo.cpp)

target_include_directories(IniParser_demo PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(IniParser_demo PUBLIC string_splits)
