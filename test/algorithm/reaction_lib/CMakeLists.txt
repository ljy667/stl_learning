project(reaction_lib)

file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src *.cpp)

#add_library(IniParser src/IniParser.cpp)

#纯头文件库
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)


set_target_properties(${PROJECT_NAME} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set_target_properties(${PROJECT_NAME} PROPERTIES 
LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)



# add_executable(dp_demo main.cpp)

# target_include_directories(dp_demo PUBLIC 
# ${CMAKE_CURRENT_SOURCE_DIR}/include)

# target_link_libraries(dp_demo PUBLIC dp_lib)




