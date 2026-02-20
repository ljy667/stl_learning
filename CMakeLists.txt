cmake_minimum_required(VERSION 3.28.3)
project(stl_planning
VERSION 0.0.1
DESCRIPTION "a demo for stl planning"
HOMEPAGE_URL "https://github.com/ljy667/stl_learning.git"
LANGUAGES CXX
) 

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_STANDARD 20)
enable_testing()


#如果使用mingw，添加编译选项：静态链接，减少依赖
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libstdc++ -static-libgcc")

# # Set the build output location to a common directory
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

#设置动态库和可执行文件到bin目录（同一个目录）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#第三方
set(EIGEN3_INCLUDE_DIR "D:/study/cxx/eigen-3.4.0")
#APPEND：向列表变量追加元素
#作用：将Eigen3的cmake目录路径添加到CMAKE_MODULE_PATH列表中
#Eigen自带FindEigen3.cmake脚本，该脚本通常位于Eigen的cmake/子目录，需要告诉CMake去哪里找这个脚本
#APPEND是CMake的list命令的专用关键字；作用：向现有列表变量追加新元素；语法：list(APPEND <变量名> <元素1> <元素2>...)

# 执行：
#list(APPEND CMAKE_MODULE_PATH "/path/to/eigen/cmake")
# 结果：
# CMAKE_MODULE_PATH = "/usr/share/cmake/Modules;/path/to/eigen/cmake"

list(APPEND CMAKE_MODULE_PATH "${EIGEN3_INCLUDE_DIR}/cmake")
message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

#会执行FindEigen3.cmake脚本，成功后会：设置Eigen3_FOUND变量；创建Eigen3::Eigen目标；自动处理包含路径
find_package(Eigen3 3.4 REQUIRED)
#TARGET：检查是否存在名为Eigen3::Eigen的CMake目标
if(TARGET Eigen3::Eigen)
message(STATUS "Found Eigen3")
endif()

#添加子目录中的CMakeLists.txt文件
add_subdirectory(src)

add_subdirectory(test/stl)
