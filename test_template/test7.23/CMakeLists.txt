project(test7.23)

add_executable(${PROJECT_NAME}  test7.23.cpp)



add_test(
    NAME ${PROJECT_NAME}
    
    COMMAND ${PROJECT_NAME})