cmake_minimum_required(VERSION 3.10)

project(GPU_LLVM_Optimizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# =========================
# LLVM COMPONENTS
# =========================
llvm_map_components_to_libnames(LLVM_LIBS
    Core
    Analysis
    Passes
    Support
    IRReader
)

# =========================
# IR ANALYZER
# =========================
add_executable(ir_analyzer
    src/analysis/IRAnalyzer.cpp
)

target_link_libraries(ir_analyzer
    ${LLVM_LIBS}
    zstd
)

# =========================
# PASS RUNNER (FIXED PATH)
# =========================
add_executable(run_pass
    src/passes/run_pass.cpp
    src/passes/MyAnalysisPass.cpp
)

target_link_libraries(run_pass
    ${LLVM_LIBS}
)