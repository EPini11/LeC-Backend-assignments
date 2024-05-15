#include "llvm/Transforms/Utils/CustomLICM.h"

using namespace llvm;

bool DominateAllExits(DominatorTree &DT, BasicBlock *BB, Loop &L) {

	SmallVector<BasicBlock *> exitBlocks;
	L.getExitBlocks(exitBlocks);

	for (BasicBlock *exitBlock : exitBlocks) {
		if (!DT.dominates(BB, exitBlock)) 
			return false;
	}
	return true;
}

bool DominateAllUses(DominatorTree &DT, Instruction *I) {

	BasicBlock *IBB = I->getParent();

	for (Use &U : I->uses()) {
		Instruction *userInstr = dyn_cast<Instruction>(U.getUser());
		BasicBlock *UIBB = userInstr->getParent();
		if (!DT.dominates(IBB, UIBB)) {
			outs() << *I << " does not dominate" << *userInstr << "\n";
			return false;
		}
			
	}
	return true;

}

bool IsAliveAfterLoop(Loop &L, Instruction *I) {

	for (User *U : I->users()) {
		if (isa<PHINode>(U)) {
			for (User *UPHI : U->users()) {
				if (!L.contains(dyn_cast<Instruction>(UPHI))) {
					return true; 	
				}
			}
		}
	}

	return false;

}

bool isInCandidateInstr(Use* op, std::vector<Instruction*> candidateInstr) {

	if (candidateInstr.empty()) return false;

	for (Instruction* candidate : candidateInstr) 
		if (candidate == dyn_cast<Instruction>(op)) 
			return true;
	return false;

}

bool hasLoopInvariantOperands2(Instruction* I, std::vector<Instruction*> candidateInstr) {

	for (Use& op : I->operands()) 
		if ( !isa<Constant>(op) && !isInCandidateInstr(&op, candidateInstr) ) 
			return false;
	return true;

}

PreservedAnalyses CustomLICM::run(Loop &L, LoopAnalysisManager &AM, LoopStandardAnalysisResults &AR, LPMUpdater &U) {

	std::vector<Instruction*> candidateInstr;

	/* Trova tutte le Loop invariant instructions:
	 *
     * hasLoopInvariantOperands: 
	 * Per ogni istruzione del tipo A=B+C marca l’istruzione come Loop-INVARIANT se tutte le definizioni di B e C che 
	 * raggiungono l’istruzione A=B+C si trovano fuori dal loop o se B e/o C sono costanti
	 * (è stato provato anche il metodo isLoopInvariant della classe Loop, ma non restituisce nulla)
	 *
	 * hasLoopInvariantOperands2:
	 * Oppure c’è esattamente una reaching definition per B (e C), e si tratta di un’istruzione del loop che è stata marcata loop-invariant
	 * (a seguito di alcuni testi il metodo hasLoopInvariantOperands della classe Loop non sembra coprire questa casistica)
	 *
	 */
	
	for (BasicBlock* BB : L.getBlocks()) 
		for (Instruction& I : *BB) 
            if ( isa<BinaryOperator>(&I) && ( L.hasLoopInvariantOperands(&I) || hasLoopInvariantOperands2(&I, candidateInstr) ) ) 
					candidateInstr.push_back(&I);
					
	outs() << "1\n";
	for (Instruction* I : candidateInstr) {
		outs() << *I << "\n";
	}

	outs() << "\n2\n";
	
	std::vector<Instruction*> candidateInstr2;
	//Loop invariant instructions che dominano tutti le uscite del loop o sono dead alla fine del loop
	for (Instruction* I : candidateInstr) {
			BasicBlock *IBB = I->getParent();
			if (DominateAllExits(AR.DT, IBB, L)){ 
				outs() << *I << " Domina tutte le uscite\n";
				candidateInstr2.push_back(I);
			}		
			else if (!IsAliveAfterLoop(L, I)) {
				outs() << *I << " Muore alla fine del loop\n";
				candidateInstr2.push_back(I);
			}
	}

	candidateInstr = candidateInstr2;
	candidateInstr2.clear();

	outs() << "\n3\n";

	//Loop invariant instructions che dominano tutte le uscite del loop (o sono dead alla fine del loop) e i propri usi
	for (Instruction* I : candidateInstr) {
		if (DominateAllUses(AR.DT, I)) 
			candidateInstr2.push_back(I);
	}

	candidateInstr = candidateInstr2;
	candidateInstr2.clear();

	outs() << "Valid instructions for code motion are:\n";

	for (Instruction* I : candidateInstr) {
		outs() << *I << "\n";
	}

	if (candidateInstr.empty()) 
		return PreservedAnalyses::all();
	
	//Move instructions to the preheader
	BasicBlock *Preheader = L.getLoopPreheader();
	for (Instruction* I : candidateInstr) {
		I->moveBefore(Preheader->getTerminator());
	}
	
	return PreservedAnalyses::none();
	
}
