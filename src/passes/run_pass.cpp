#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Passes/PassBuilder.h"


#include "MyAnalysisPass.cpp"   // OK for now in your setup

using namespace llvm;

int main(int argc, char **argv) {

    if (argc < 2) {
        errs() << "Usage: ./run_pass <file.ll>\n";
        return 1;
    }

    LLVMContext Context;
    SMDiagnostic Err;

    std::unique_ptr<Module> M = parseIRFile(argv[1], Err, Context);

    if (!M) {
        errs() << "Error: could not load IR file\n";
        return 1;
    }

    PassBuilder PB;

    FunctionAnalysisManager FAM;
    FunctionPassManager FPM;

    PB.registerFunctionAnalyses(FAM);

    // add your pass
    FPM.addPass(MyAnalysisPass());

    outs() << "RUNNING PASS ON MODULE\n";

    for (Function &F : *M) {
        if (!F.isDeclaration()) {
            FPM.run(F, FAM);
        }
    }

    outs() << "DONE\n";

    return 0;
}