project(map_test)

add_executable(${PROJECT_NAME} map_test.cpp)

#测试是把主程序中的一部分代码拉出来测试

target_include_directories(${PROJECT_NAME}
PUBLIC
${PNC_Map_Dir}
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
pnc_map
)

# add_test(NAME map_info_test COMMAND ${PROJECT_NAME})"这段CMake代码的含义是：
# NAME map_info_test：定义了一个测试用例，名称为"map_info_test"
# COMMAND {PROJECT_NAME}：指定要运行的测试命令是可执行文件{PROJECT_NAME}（即map_test）

# 这个测试用例的作用是运行map_test可执行文件，测试PNC_Map类的功能。
# 根据map_test.cpp的内容，它会创建一个PNC_Map对象并调用mapInfo()方法。

# 在CMake中，add_test命令用于定义   CTest测试框架   的测试用例。
# 当运行"ctest"命令时，它会执行所有通过add_test定义的测试用例。
# 这个特定的测试用例map_info_test会运行map_test程序来验证PNC_Map类的功能是否正常。

add_test(
    NAME map_info_test
    COMMAND ${PROJECT_NAME}
    )