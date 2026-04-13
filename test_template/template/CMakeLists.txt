cmake_minimum_required(VERSION 3.20)

project(template_study LANGUAGES CXX)

add_executable(template_study
    11_2.cpp
)

# Optional: speed-up on MSVC
if(MSVC)
    target_compile_options(time_anomaly_detector PRIVATE /utf-8)
endif()
