# GPU LLVM Optimizer

## Overview

GPU LLVM Optimizer is a custom LLVM-based analysis framework designed to analyze LLVM IR and extract optimization opportunities from functions and loops. The project provides a modular infrastructure for building compiler analysis passes, loop analysis, and heuristic-based optimization scoring.

The framework is intended for research and experimentation in compiler design, loop optimization, and GPU-oriented performance analysis.

---

## Features

### IR Analysis
- Instruction-level classification
- Load, store, arithmetic, and branch counting
- Function-level statistics extraction

### Loop Analysis
- Loop detection using LLVM LoopInfo
- Loop header identification
- Loop block enumeration
- Loop nesting depth analysis

### Optimization Heuristics
- Loop scoring system (0–100)
- Memory-bound detection
- Vectorization suitability hints
- Loop unrolling suggestions
- Nested loop detection

### Execution Modes
- Standalone analysis tool (`run_pass`)
- LLVM pass plugin (`libMyAnalysisPass.so`)
- IR file processing support

---

## Project Structure

