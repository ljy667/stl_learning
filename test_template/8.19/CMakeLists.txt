project(dur_storge)

# 使用 file(GLOB) 匹配所有 .cpp 文件
file(GLOB SOURCES *.cpp)

add_executable(${PROJECT_NAME} 
${SOURCES}
)

target_include_directories(${PROJECT_NAME}
PUBLIC
${CMAKE_SOURCE_DIR})

#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}):