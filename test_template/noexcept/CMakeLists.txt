project(reaction_demo)



add_executable(${PROJECT_NAME} noexcept_demo.cpp)

target_include_directories(${PROJECT_NAME} 
PUBLIC
${Reaction_Dir})

# target_link_libraries(${PROJECT_NAME}
# PUBLIC
# reaction)