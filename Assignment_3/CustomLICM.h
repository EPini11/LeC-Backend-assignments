#ifndef LLVM_TRANSFORMS_CUSTOMLICM_H
#define LLVM_TRANSFORMS_CUSTOMLICM_H

#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/IR/Dominators.h"

namespace llvm {

class CustomLICM : public PassInfoMixin<CustomLICM> {
public:
	PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM, LoopStandardAnalysisResults &AR, LPMUpdater &U);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_TESTPASS _H
