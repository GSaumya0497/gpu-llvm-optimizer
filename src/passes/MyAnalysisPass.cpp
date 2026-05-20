#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"

#include "llvm/Support/raw_ostream.h"

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Passes/PassBuilder.h"

using namespace llvm;

class MyAnalysisPass : public PassInfoMixin<MyAnalysisPass> {

public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {

        outs() << "LOOP SCORING ANALYSIS\n";
        outs() << "Function: " << F.getName() << "\n\n";

        // =========================
        // IR STATS
        // =========================
        int instCount = 0;
        int loadCount = 0;
        int storeCount = 0;
        int branchCount = 0;
        int arithCount = 0;

        for (BasicBlock &BB : F) {
            for (Instruction &I : BB) {

                instCount++;

                if (isa<LoadInst>(I)) loadCount++;
                else if (isa<StoreInst>(I)) storeCount++;
                else if (isa<BranchInst>(I)) branchCount++;
                else if (I.isBinaryOp()) arithCount++;
            }
        }

        outs() << "=== Instruction Stats ===\n";
        outs() << "Inst       : " << instCount << "\n";
        outs() << "Loads      : " << loadCount << "\n";
        outs() << "Stores     : " << storeCount << "\n";
        outs() << "Branches   : " << branchCount << "\n";
        outs() << "Arithmetic : " << arithCount << "\n\n";

        // =========================
        // LOOP SCORING ENGINE
        // =========================
        LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);

        outs() << "=== Loop Scoring System ===\n";

        int loopIndex = 0;

        for (Loop *L : LI) {

            BasicBlock *header = L->getHeader();
            unsigned depth = L->getLoopDepth();
            unsigned blockCount = L->getNumBlocks();

            // =========================
            // SCORING MODEL (0–100)
            // =========================
            int score = 0;

            // 1. Loop size (more blocks = more work)
            score += blockCount * 10;

            // 2. Arithmetic intensity
            score += arithCount * 5;

            // 3. Memory pressure penalty
            if (loadCount + storeCount > 0)
                score -= (loadCount + storeCount) * 2;

            // 4. Depth boost (nested loops more important)
            score += depth * 15;

            // clamp score
            if (score < 0) score = 0;
            if (score > 100) score = 100;

            // =========================
            // OUTPUT
            // =========================
            outs() << "\n Loop " << loopIndex++ << "\n";
            outs() << "Depth   : " << depth << "\n";
            outs() << "Blocks  : " << blockCount << "\n";
            outs() << "Header  : " << header << "\n";

            outs() << "Loop Score: " << score << "/100\n";

            // =========================
            // ACTIONABLE CLASSIFICATION
            // =========================
            outs() << "Recommendations:\n";

            if (score >= 70)
                outs() << "  🚀 HIGH optimization priority\n";

            if (score >= 50)
                outs() << "  ✔ Candidate for vectorization\n";

            if (blockCount <= 3)
                outs() << "  ✔ Candidate for loop unrolling\n";

            if (loadCount + storeCount > arithCount)
                outs() << "  ⚠ Memory-bound loop (optimize memory access)\n";

            if (depth > 1)
                outs() << "  ⚠ Nested loop (consider tiling/fusion)\n";

            outs() << "Blocks: ";
            for (BasicBlock *BB : L->blocks()) {
                outs() << BB << " ";
            }

            outs() << "\n";
        }

        outs() << "\n=========================\n";

        return PreservedAnalyses::all();
    }
};