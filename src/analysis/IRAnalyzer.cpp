#include <iostream>

#include "llvm/IR/CFG.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"


using namespace llvm;

int main() {

    LLVMContext Context;
    SMDiagnostic Error;

    std::unique_ptr<Module> Mod =
        parseIRFile("examples/loop.ll", Error, Context);

    if (!Mod) {
        Error.print("IR Parser", errs());
        return 1;
    }

    std::cout << "===== LLVM IR Analysis =====\n";

    for (Function &F : *Mod) {

        if (F.isDeclaration())
            continue;

        int basicBlockCount = 0;
        int instructionCount = 0;

        int loadCount = 0;
        int storeCount = 0;
        int branchCount = 0;
        int arithmeticCount = 0;
        int callCount = 0;

        for (BasicBlock &BB : F) {

    basicBlockCount++;

    for (BasicBlock *Succ : successors(&BB)) {

        if (Succ == &BB) {
            std::cout << "Self Loop Detected\n";
        }
    }

    for (Instruction &I : BB) {

        instructionCount++;

        if (isa<LoadInst>(I))
            loadCount++;

        else if (isa<StoreInst>(I))
            storeCount++;

        else if (BranchInst *BI = dyn_cast<BranchInst>(&I)) {

            branchCount++;

            if (BI->isConditional()) {
                std::cout << "Conditional Branch Found\n";
            }

            for (unsigned i = 0; i < BI->getNumSuccessors(); i++) {

                BasicBlock *Succ = BI->getSuccessor(i);

                if (Succ == &BB) {
                    std::cout << "Loop Back Edge Detected\n";
                }
            }
        }

        else if (isa<BinaryOperator>(I))
            arithmeticCount++;

        else if (isa<CallInst>(I))
            callCount++;
    }
}

        std::cout << "\nFunction: "
                  << F.getName().str() << "\n";

        std::cout << "Basic Blocks: "
                  << basicBlockCount << "\n";

        std::cout << "Instructions: "
                  << instructionCount << "\n";

        std::cout << "Load Instructions: "
                  << loadCount << "\n";

        std::cout << "Store Instructions: "
                  << storeCount << "\n";

        std::cout << "Arithmetic Instructions: "
                  << arithmeticCount << "\n";

        std::cout << "Branch Instructions: "
                  << branchCount << "\n";

        std::cout << "Call Instructions: "
                  << callCount << "\n";
    }

    return 0;
}