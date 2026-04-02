project(ArgParser)

add_library(ArgParser
    src/ArgParser.cpp
)

set_target_properties(ArgParser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

target_include_directories(ArgParser
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(demo src/demo.cpp)

target_link_libraries(demo PUBLIC ArgParser)