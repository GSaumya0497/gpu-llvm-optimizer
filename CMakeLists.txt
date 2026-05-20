cmake_minimum_required(VERSION 3.10)

project(GPU_LLVM_Optimizer)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(ir_analyzer
    src/analysis/IRAnalyzer.cpp
)

llvm_map_components_to_libnames(LLVM_LIBS
    Core
    IRReader
    Support
)

target_link_libraries(ir_analyzer
    ${LLVM_LIBS}
    zstd
)