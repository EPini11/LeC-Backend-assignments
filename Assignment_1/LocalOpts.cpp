
//===-- LocalOpts.cpp - Example Transformations --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Utils/LocalOpts.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LLVMContext.h"

using namespace llvm;

bool AlgebraicIdentity(BasicBlock &B) {
	
	bool tranformed = false;

	for (Instruction &I : B) {

		Value *FirstOperand = I.getOperand(0);
		Value *SecondOperand = I.getOperand(1);
		
		if (I.getOpcode() == llvm::Instruction::BinaryOps::Add) {

			if (isa<ConstantInt>(FirstOperand) && dyn_cast<ConstantInt>(FirstOperand)->isZero()) {
				I.replaceAllUsesWith(SecondOperand);
				tranformed = true;
			}

			else if (isa<ConstantInt>(SecondOperand) && dyn_cast<ConstantInt>(SecondOperand)->isZero()) {
				I.replaceAllUsesWith(FirstOperand);
				tranformed = true;
			}

		}

		if (I.getOpcode() == llvm::Instruction::BinaryOps::Mul) {
	 
			if (isa<ConstantInt>(FirstOperand) && dyn_cast<ConstantInt>(FirstOperand)->getValue() == 1) {
				I.replaceAllUsesWith(SecondOperand);
				tranformed = true;
			}

			else if (isa<ConstantInt>(SecondOperand) && dyn_cast<ConstantInt>(SecondOperand)->getValue() == 1) {
				I.replaceAllUsesWith(FirstOperand);
				tranformed = true;
			}

		}

		if (I.getOpcode() == llvm::Instruction::BinaryOps::Sub) {

			if (isa<ConstantInt>(SecondOperand) && dyn_cast<ConstantInt>(SecondOperand)->isZero()) {
				I.replaceAllUsesWith(FirstOperand);
				tranformed = true;
			}
		}

	}

	return tranformed;

}


bool AdvancedStrengthReduction(BasicBlock &B) {

	bool tranformed = false;

	for (Instruction &I : B) {

		Value *firstOperand = I.getOperand(0);
		Value *secondOperand = I.getOperand(1);

		if (I.getOpcode() == llvm::Instruction::BinaryOps::Mul) {

			Value *costante = nullptr;
			Value *operandoShift = nullptr;
			bool addToo = false;

			if (isa<ConstantInt>(firstOperand) && dyn_cast<ConstantInt>(firstOperand)->getValue().exactLogBase2() != -1) {
				costante = firstOperand;
				operandoShift = secondOperand;
			}
			else if (isa<ConstantInt>(secondOperand) && dyn_cast<ConstantInt>(secondOperand)->getValue().exactLogBase2() != -1) {
				costante = secondOperand;
				operandoShift = firstOperand;
			}
			else if (isa<ConstantInt>(firstOperand) &&  ( (dyn_cast<ConstantInt>(firstOperand)->getValue() + 1).exactLogBase2() != -1 || (dyn_cast<ConstantInt>(firstOperand)->getValue() - 1).exactLogBase2() != -1 ) ) {
				costante = firstOperand;
				operandoShift = secondOperand;
				addToo = true;
			}
			else if (isa<ConstantInt>(secondOperand) && ( (dyn_cast<ConstantInt>(secondOperand)->getValue() + 1).exactLogBase2() != -1 || (dyn_cast<ConstantInt>(secondOperand)->getValue() - 1).exactLogBase2() != -1 ) ) {
				costante = secondOperand;
				operandoShift = firstOperand;
				addToo = true;
			}
			else continue;

			if (dyn_cast<ConstantInt>(costante)->getValue().exactLogBase2() == 0) continue;

			ConstantInt *log2 = ConstantInt::get(Type::getInt32Ty(I.getContext()), dyn_cast<ConstantInt>(costante)->getValue().nearestLogBase2());
			Instruction *ShiftLeftInstr = BinaryOperator::Create(Instruction::Shl, operandoShift, log2);
			ShiftLeftInstr->insertAfter(&I);
			Instruction *AddInstr = nullptr;

			if (addToo) {

				//y = x * (2^n - 1) oppure y = (2^n - 1) * x ⇒ y = (x << n) - x
				if ( (dyn_cast<ConstantInt>(costante)->getValue() + 1).exactLogBase2() != -1 )
					AddInstr = BinaryOperator::Create(Instruction::Sub, ShiftLeftInstr, operandoShift);
				//y = x * (2^n + 1) oppure y = (2^n + 1) * x ⇒ y = (x << n) + x
				else 
					AddInstr = BinaryOperator::Create(Instruction::Add, ShiftLeftInstr, operandoShift);

				AddInstr->insertAfter(ShiftLeftInstr);
				I.replaceAllUsesWith(AddInstr);

			}
			else I.replaceAllUsesWith(ShiftLeftInstr);

			tranformed = true;

		}

		if (I.getOpcode() == llvm::Instruction::BinaryOps::SDiv) {

			//controllo solo il secondo operando perchè la divisione non è commutativa
			if (isa<ConstantInt>(secondOperand) && dyn_cast<ConstantInt>(secondOperand)->getValue().exactLogBase2() != -1) {
				ConstantInt *log2 = ConstantInt::get(Type::getInt32Ty(I.getContext()), dyn_cast<ConstantInt>(secondOperand)->getValue().exactLogBase2());
				Instruction *ShiftRightInstr = BinaryOperator::Create(Instruction::LShr, firstOperand, log2);
				ShiftRightInstr ->insertAfter(&I);
				I.replaceAllUsesWith(ShiftRightInstr);
				tranformed = true;
			} 

		}

	}

	return tranformed;
}


bool MultiInstructionOpt(BasicBlock &B) {

	bool tranformed = false;

	for (Instruction &I : B) {

		Value *firstOperand = I.getOperand(0);
		Value *secondOperand = I.getOperand(1);

		if (I.getOpcode() == llvm::Instruction::BinaryOps::Add) {

			Value *costante = nullptr;
			Value *addendo = nullptr;

			if (isa<ConstantInt>(firstOperand)) {
				costante = firstOperand;
				addendo = secondOperand;
			}
			else if (isa<ConstantInt>(secondOperand)) {
				costante = secondOperand;
				addendo = firstOperand;
			} 
			else continue;

			for (auto U = I.user_begin(); U != I.user_end(); ++U) {

				Instruction *InstructionUser = dyn_cast<Instruction>(*U);
				Value *firstOperandUser = InstructionUser->getOperand(0);
				Value *secondOperandUser = InstructionUser->getOperand(1);

				if (InstructionUser->getOpcode() == llvm::Instruction::BinaryOps::Sub) {
					/*
					a = b + costante; 
					c = costante - a;
					c = costante - b - costante = -b
					equivalentemente:
					a = costante + b; 
					c = costante - a;
					c = costante - costante - b = -b
					*/
					if (isa<ConstantInt>(firstOperandUser) && ( dyn_cast<ConstantInt>(costante)->getValue() == dyn_cast<ConstantInt>(firstOperandUser)->getValue() )) {
						ConstantInt *zero = ConstantInt::get(Type::getInt32Ty(I.getContext()), 0);
						Instruction *minus_b = BinaryOperator::Create(Instruction::Sub, zero, addendo);
						minus_b->insertAfter(InstructionUser);
						InstructionUser->replaceAllUsesWith(minus_b);
						tranformed = true;
					}
					/*
					a = b + costante; 
					c = a - costante;
					c = b + costante - costante = b;
					equivalentemente:
					a = costante + b; 
					c = a - costante;
					c = costante + b - costante = b;
					*/
					else if (isa<ConstantInt>(secondOperandUser) && (dyn_cast<ConstantInt>(costante)->getValue() == dyn_cast<ConstantInt>(secondOperandUser)->getValue())) {
						InstructionUser->replaceAllUsesWith(addendo);
						tranformed = true;
					}
				}

			}
		}

		else if (I.getOpcode() == llvm::Instruction::BinaryOps::Sub) {

			/*
			a = b - costante; 
			c = a + costante;
			c = b
			equivalentemente:
			a = b - costante;
			c = costante + a;
			c = b
			*/
			if (isa<ConstantInt>(secondOperand)) {

				for (auto U = I.user_begin(); U != I.user_end(); ++U) {

					Instruction *InstructionUser = dyn_cast<Instruction>(*U);
					Value *firstOperandUser = InstructionUser->getOperand(0);
					Value *secondOperandUser = InstructionUser->getOperand(1);

					if (InstructionUser->getOpcode() == llvm::Instruction::BinaryOps::Add) {

						Value *costanteUser = nullptr;

						if (isa<ConstantInt>(firstOperandUser)) {
							costanteUser = firstOperandUser;
						}
						else if (isa<ConstantInt>(secondOperandUser)) {
							costanteUser = secondOperandUser;
						}	
						else continue;	

						if (dyn_cast<ConstantInt>(costanteUser)->getValue() == dyn_cast<ConstantInt>(secondOperand)->getValue()) {
							InstructionUser->replaceAllUsesWith(firstOperand);
							tranformed = true;
						}

					}

				}

			}

			/* se il valore costante è nel primo non faccio nessuna ottimizzazione poichè avrei:
			a = costante - b; 
			c = a + costante;
			c = costante - b + costante = 2 * costante - b
			equivalentemente:
			a = costante - b; 
			c = costante + a;
			c = costante + costante - b = 2 * costante - b
			*/

		}
		
	}

	return tranformed;

}


bool runOnFunction(Function &F) {
	bool Transformed = false;

	for (auto Iter = F.begin(); Iter != F.end(); ++Iter) {
		if (AlgebraicIdentity(*Iter)) {
			Transformed = true;
		}
		if (AdvancedStrengthReduction(*Iter)) {
			Transformed = true;
		}
		if (MultiInstructionOpt(*Iter)) {
			Transformed = true;
		}
	}
	return Transformed;
}


PreservedAnalyses LocalOpts::run(Module &M, ModuleAnalysisManager &AM) {
	for (auto Fiter = M.begin(); Fiter != M.end(); ++Fiter)
		if (runOnFunction(*Fiter))
			return PreservedAnalyses::none();

	return PreservedAnalyses::all();
}
