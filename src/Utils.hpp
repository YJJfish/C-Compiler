/*
 ******************** Utils.hpp *********************
 * This file defines and implements some frequently	*
 * used functions.									*
 ****************************************************
 */

#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include "CodeGenerator.h"

//Type casting
//Supported:
//1. Int -> Int, Float, Pointer
//2. Float -> Int, Float
//3. Pointer -> Int, Pointer
//Other types are not supported, and will return NULL.
llvm::Value* TypeCasting(llvm::Value* Value, llvm::Type* Type) {
	if (Value->getType() == Type) {
		return Value;
	}
	else if (Type == IRBuilder.getInt1Ty()) {	//Int1 (bool) is special.
		return Cast2I1(Value);
	}
	else if (Value->getType()->isIntegerTy() && Type->isIntegerTy()) {
		return IRBuilder.CreateIntCast(Value, Type, true);
	}
	else if (Value->getType()->isIntegerTy() && Type->isFloatingPointTy()) {
		return IRBuilder.CreateSIToFP(Value, Type);
	}
	else if (Value->getType()->isIntegerTy() && Type->isPointerTy()) {
		return IRBuilder.CreateIntToPtr(Value, Type);
	}
	else if (Value->getType()->isFloatingPointTy() && Type->isIntegerTy()) {
		return IRBuilder.CreateFPToSI(Value, Type);
	}
	else if (Value->getType()->isFloatingPointTy() && Type->isFloatingPointTy()) {
		return IRBuilder.CreateFPCast(Value, Type);
	}
	else if (Value->getType()->isPointerTy() && Type->isIntegerTy()) {
		return IRBuilder.CreatePtrToInt(Value, Type);
	}
	else if (Value->getType()->isPointerTy() && Type->isPointerTy()) {
		return IRBuilder.CreatePointerCast(Value, Type);
	}
}

//Cast a integer, or a floating-point number, or a pointer to i1 integer.
//Return NULL if failed.
//This function is very useful when generating a condition value for "if", "while", "for" statements.
llvm::Value* Cast2I1(llvm::Value* Value) {
	if (Value->getType() == IRBuilder.getInt1Ty())
		return Value;
	else if (Value->getType()->isIntegerTy())
		return IRBuilder.CreateICmpNE(Value, llvm::ConstantInt::get((llvm::IntegerType*)Value->getType(), 0, true));
	else if (Value->getType()->isFloatingPointTy())
		return IRBuilder.CreateFCmpONE(Value, llvm::ConstantFP::get(Context, llvm::APFloat(0.0)));
	else if (Value->getType()->isPointerTy())
		return IRBuilder.CreateICmpNE(IRBuilder.CreatePtrToInt(Value, IRBuilder.getInt64Ty()), IRBuilder.getInt64(0));
	else
		return NULL;
}

//Create an alloca instruction in the entry block of the current function,
//wherever the variable declaration is.
//This function is useful when the variable declaration is not in the entry block of the function,
//e.g. in the body of a loop.
//In this case, this function can help prevent stack overflow.
llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* Func, std::string VarName, llvm::Type* VarType) {
	llvm::IRBuilder<> TmpB(&Func->getEntryBlock(), Func->getEntryBlock().begin());
	return TmpB.CreateAlloca(VarType, 0, VarName);
}

//Create an equal-comparison instruction. This function will automatically do type casting
//if the two input values are not of the same type.
llvm::Value* CreateCmpEQ(llvm::Value* LHS, llvm::Value* RHS) {
	return NULL;
}

//Create an unconditional branch if the current block doesn't have a terminator.
//This function is safer than IRBuilder.CreateBr(llvm::BasicBlock* BB),
//because if the current block already has a terminator, it does nothing.
//For example, when generating if-statement, we create three blocks: ThenBB, ElseBB, MergeBB.
//At the end of ThenBB and ElseBB, an unconditional branch to MergeBB needs to be created respectively,
//UNLESS ThenBB or ElseBB is already terminated.
//e.g.
//	if (i) break;
//	else continue;
llvm::BranchInst* TerminateBlockByBr(llvm::BasicBlock* BB) {
	//If not terminated, jump to the target block
	if (!IRBuilder.GetInsertBlock()->getTerminator())
		return IRBuilder.CreateBr(BB);
	else
		return NULL;
}