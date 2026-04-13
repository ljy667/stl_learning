project(thread_test)

add_executable(${PROJECT_NAME} thread_7.22.cpp)

target_include_directories(${PROJECT_NAME} 
PUBLIC
${Thread_Test_Dir})

target_link_libraries(${PROJECT_NAME} 
PUBLIC
thread_7.22)



add_test(
    NAME thread_test
    COMMAND ${PROJECT_NAME}
    )