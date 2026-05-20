
cmake_minimum_required(VERSION 3.10)

project(GPU_LLVM_Optimizer)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB SOURCES
    src/*.cpp
    src/core/*.cpp
    src/analysis/*.cpp
    src/optimization/*.cpp
    src/utils/*.cpp
)

add_executable(gpu_opt ${SOURCES})