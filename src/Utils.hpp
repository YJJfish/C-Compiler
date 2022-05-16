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
#include "CodeGenerator.hpp"

 //Cast a integer, or a floating-point number, or a pointer to i1 integer.
 //Return NULL if failed.
 //This function is very useful when generating a condition value for "if", "while", "for" statements.
llvm::Value* Cast2I1(llvm::Value* Value) {
	if (Value->getType() == IRBuilder.getInt1Ty())
		return Value;
	else if (Value->getType()->isIntegerTy())
		return IRBuilder.CreateICmpNE(Value, llvm::ConstantInt::get((llvm::IntegerType*)Value->getType(), 0, true));
	else if (Value->getType()->isFloatingPointTy())
		return IRBuilder.CreateFCmpONE(Value, llvm::ConstantFP::get(Value->getType(), 0.0));
	else if (Value->getType()->isPointerTy())
		return IRBuilder.CreateICmpNE(IRBuilder.CreatePtrToInt(Value, IRBuilder.getInt64Ty()), IRBuilder.getInt64(0));
	else {
		throw std::logic_error("Cannot cast to bool type.");
		return NULL;
	}
}

//Type casting
//Supported:
//1. Int -> Int, FP, Pointer
//2. FP -> Int, FP
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
		return IRBuilder.CreateIntCast(Value, Type, !Value->getType()->isIntegerTy(1));
	}
	else if (Value->getType()->isIntegerTy() && Type->isFloatingPointTy()) {
		return Value->getType()->isIntegerTy(1) ?
			IRBuilder.CreateUIToFP(Value, Type) : IRBuilder.CreateSIToFP(Value, Type);
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
	else {
		return NULL;
	}
}

//Upgrade the type, given another type.
//1. int1
//2. int8
//3. int16
//4. int32
//5. int64
//6. float
//7. double
//Return NULL if failed.
//For example,
//	TypeUpgrading(int16, int32) -> int32
//	TypeUpgrading(int32, double) -> double
//	TypeUpgrading(int64, float) -> float
llvm::Value* TypeUpgrading(llvm::Value* Value, llvm::Type* Type) {
	if (Value->getType()->isIntegerTy() && Type->isIntegerTy()) {
		size_t Bit1 = ((llvm::IntegerType*)Value->getType())->getBitWidth();
		size_t Bit2 = ((llvm::IntegerType*)Type)->getBitWidth();
		if (Bit1 < Bit2)
			return IRBuilder.CreateIntCast(Value, Type, Bit1 != 1);
		else return Value;
	}
	else if (Value->getType()->isFloatingPointTy() && Type->isFloatingPointTy()) {
		if (Value->getType()->isFloatTy() && Type->isDoubleTy())
			return IRBuilder.CreateFPCast(Value, Type);
		else return Value;
	}
	else if (Value->getType()->isIntegerTy() && Type->isFloatingPointTy()) {
		return Value->getType()->isIntegerTy(1) ?
			IRBuilder.CreateUIToFP(Value, Type) : IRBuilder.CreateSIToFP(Value, Type);
	}
	else if (Value->getType()->isFloatingPointTy() && Type->isIntegerTy()) {
		return Value;
	}
	else return NULL;
}

//Upgrade two types at the same time.
//1. int1
//2. int8
//3. int16
//4. int32
//5. int64
//6. float
//7. double
//Return false if failed.
//For example,
//	TypeUpgrading(int16, int32) -> int32
//	TypeUpgrading(int32, double) -> double
//	TypeUpgrading(int64, float) -> float
bool TypeUpgrading(llvm::Value*& Value1, llvm::Value*& Value2) {
	if (Value1->getType()->isIntegerTy() && Value2->getType()->isIntegerTy()) {
		size_t Bit1 = ((llvm::IntegerType*)Value1->getType())->getBitWidth();
		size_t Bit2 = ((llvm::IntegerType*)Value2->getType())->getBitWidth();
		if (Bit1 < Bit2)
			Value1 = IRBuilder.CreateIntCast(Value1, Value2->getType(), Bit1 != 1);
		else if (Bit1 > Bit2)
			Value2 = IRBuilder.CreateIntCast(Value2, Value1->getType(), Bit2 != 1);
		return true;
	}
	else if (Value1->getType()->isFloatingPointTy() && Value2->getType()->isFloatingPointTy()) {
		if (Value1->getType()->isFloatTy() && Value2->getType()->isDoubleTy())
			Value1 = IRBuilder.CreateFPCast(Value1, IRBuilder.getDoubleTy());
		else if (Value1->getType()->isDoubleTy() && Value2->getType()->isFloatTy())
			Value2 = IRBuilder.CreateFPCast(Value2, IRBuilder.getDoubleTy());
		return true;
	}
	else if (Value1->getType()->isIntegerTy() && Value2->getType()->isFloatingPointTy()) {
		Value1 = Value1->getType()->isIntegerTy(1) ?
			IRBuilder.CreateUIToFP(Value1, Value2->getType()) : IRBuilder.CreateSIToFP(Value1, Value2->getType());
		return true;
	}
	else if (Value1->getType()->isFloatingPointTy() && Value2->getType()->isIntegerTy()) {
		Value2 = Value2->getType()->isIntegerTy(1) ?
			IRBuilder.CreateUIToFP(Value2, Value1->getType()) : IRBuilder.CreateSIToFP(Value2, Value1->getType());
		return true;
	}
	else return false;
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
	//Arithmatic compare
	if (TypeUpgrading(LHS, RHS)) {
		if (LHS->getType()->isIntegerTy())
			return IRBuilder.CreateICmpEQ(LHS, RHS);
		else
			return IRBuilder.CreateFCmpOEQ(LHS, RHS);
	}
	//Pointer compare
	if (LHS->getType()->isPointerTy() && LHS->getType() == RHS->getType()) {
		return IRBuilder.CreateICmpEQ(
			IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
			IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
		);
	}
	else if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
		return IRBuilder.CreateICmpEQ(
			IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
			TypeUpgrading(RHS, IRBuilder.getInt64Ty())
		);
	}
	else if (LHS->getType()->isIntegerTy() && RHS->getType()->isPointerTy()) {
		return IRBuilder.CreateICmpEQ(
			TypeUpgrading(LHS, IRBuilder.getInt64Ty()),
			IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
		);
	}
	throw std::domain_error("Comparison \"==\" using unsupported type combination.");
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

//Create an addition instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int + Int -> Int			(TypeUpgrading)
//2. Int + FP -> FP				(TypeUpgrading)
//3. Int + Pointer -> Pointer
//4. FP + Int -> FP				(TypeUpgrading)
//2. FP + FP -> FP				(TypeUpgrading)
//3. Pointer + Int -> Pointer
llvm::Value* CreateAdd(llvm::Value* LHS, llvm::Value* RHS, CodeGenerator& Generator) {
	if (TypeUpgrading(LHS, RHS)) {
		if (LHS->getType()->isIntegerTy())
			return IRBuilder.CreateAdd(LHS, RHS);
		else
			return IRBuilder.CreateFAdd(LHS, RHS);
	}
	if (LHS->getType()->isIntegerTy() && RHS->getType()->isPointerTy()) {
		auto TMP = LHS;
		LHS = RHS;
		RHS = TMP;
	}
	if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
		return IRBuilder.CreateIntToPtr(
			IRBuilder.CreateAdd(
				IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
				IRBuilder.CreateMul(
					IRBuilder.CreateIntCast(RHS, IRBuilder.getInt64Ty(), true),
					IRBuilder.getInt64(Generator.GetTypeSize(LHS->getType()->getNonOpaquePointerElementType()))
				)
			),
			LHS->getType()
		);
	}
	throw std::logic_error("Addition using unsupported type combination.");
	return NULL;
}

//Create a subtraction instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int - Int -> Int			(TypeUpgrading)
//2. Int - FP -> FP				(TypeUpgrading)
//3. FP - Int -> FP				(TypeUpgrading)
//4. FP - FP -> FP				(TypeUpgrading)
//5. Pointer - Int -> Pointer
//6. Pointer - Pointer -> Int64
llvm::Value* CreateSub(llvm::Value* LHS, llvm::Value* RHS, CodeGenerator& Generator) {
	if (TypeUpgrading(LHS, RHS)) {
		if (LHS->getType()->isIntegerTy())
			return IRBuilder.CreateSub(LHS, RHS);
		else
			return IRBuilder.CreateFSub(LHS, RHS);
	}
	if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
		return IRBuilder.CreateIntToPtr(
			IRBuilder.CreateSub(
				IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
				IRBuilder.CreateMul(
					IRBuilder.CreateIntCast(RHS, IRBuilder.getInt64Ty(), true),
					IRBuilder.getInt64(Generator.GetTypeSize(LHS->getType()->getNonOpaquePointerElementType()))
				)
			),
			LHS->getType()
		);
	}
	if (LHS->getType()->isPointerTy() && LHS->getType() == RHS->getType())
		return IRBuilder.CreatePtrDiff(LHS->getType()->getNonOpaquePointerElementType(), LHS, RHS);
	throw std::logic_error("Subtraction using unsupported type combination.");
	return NULL;
}

//Create a multiplication instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int * Int -> Int			(TypeUpgrading)
//2. Int * FP -> FP				(TypeUpgrading)
//3. FP * Int -> FP				(TypeUpgrading)
//4. FP * FP -> FP				(TypeUpgrading)
llvm::Value* CreateMul(llvm::Value* LHS, llvm::Value* RHS, CodeGenerator& Generator) {
	if (TypeUpgrading(LHS, RHS)) {
		if (LHS->getType()->isIntegerTy())
			return IRBuilder.CreateMul(LHS, RHS);
		else
			return IRBuilder.CreateFMul(LHS, RHS);
	}
	else {
		throw std::logic_error("Multiplication operator \"*\" must only be applied to integers or floating-point numbers.");
		return NULL;
	}
}

//Create a division instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int / Int -> Int			(TypeUpgrading)
//2. Int / FP -> FP				(TypeUpgrading)
//3. FP / Int -> FP				(TypeUpgrading)
//4. FP / FP -> FP				(TypeUpgrading)
llvm::Value* CreateDiv(llvm::Value* LHS, llvm::Value* RHS, CodeGenerator& Generator) {
	if (TypeUpgrading(LHS, RHS)) {
		if (LHS->getType()->isIntegerTy())
			return IRBuilder.CreateSDiv(LHS, RHS);
		else
			return IRBuilder.CreateFDiv(LHS, RHS);
	}
	else {
		throw std::logic_error("Division operator \"/\" must only be applied to integers or floating-point numbers.");
		return NULL;
	}
}

//Create a modulo instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int % Int -> Int			(TypeUpgrading)
llvm::Value* CreateMod(llvm::Value* LHS, llvm::Value* RHS, CodeGenerator& Generator) {
	if (!(LHS->getType()->isIntegerTy() && RHS->getType()->isIntegerTy())) {
		throw std::domain_error("Modulo operator \"%\" must be applied to 2 integers.");
		return NULL;
	}
	TypeUpgrading(LHS, RHS);
	return IRBuilder.CreateSRem(LHS, RHS);
}

//Create a shl instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int << Int -> Int			(TypeUpgrading)
llvm::Value* CreateShl(llvm::Value* LHS, llvm::Value* RHS, CodeGenerator& Generator) {
	if (!(LHS->getType()->isIntegerTy() && RHS->getType()->isIntegerTy())) {
		throw std::domain_error("Left shifting operator \"<<\" must be applied to 2 integers.");
		return NULL;
	}
	TypeUpgrading(LHS, RHS);
	return IRBuilder.CreateShl(LHS, RHS);
}

//Create a shr instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int >> Int -> Int			(TypeUpgrading)
llvm::Value* CreateShr(llvm::Value* LHS, llvm::Value* RHS, CodeGenerator& Generator) {
	if (!(LHS->getType()->isIntegerTy() && RHS->getType()->isIntegerTy())) {
		throw std::domain_error("Left shifting operator \"<<\" must be applied to 2 integers.");
		return NULL;
	}
	TypeUpgrading(LHS, RHS);
	return IRBuilder.CreateAShr(LHS, RHS);
}

//Create a bitwise AND instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int & Int -> Int			(TypeUpgrading)
llvm::Value* CreateBitwiseAND(llvm::Value* LHS, llvm::Value* RHS, CodeGenerator& Generator) {
	if (!(LHS->getType()->isIntegerTy() && RHS->getType()->isIntegerTy())) {
		throw std::domain_error("Bitwise AND operator \"&\" must be applied to 2 integers.");
		return NULL;
	}
	TypeUpgrading(LHS, RHS);
	return IRBuilder.CreateAnd(LHS, RHS);
}

//Create a bitwise OR instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int | Int -> Int			(TypeUpgrading)
llvm::Value* CreateBitwiseOR(llvm::Value* LHS, llvm::Value* RHS, CodeGenerator& Generator) {
	if (!(LHS->getType()->isIntegerTy() && RHS->getType()->isIntegerTy())) {
		throw std::domain_error("Bitwise OR operator \"|\" must be applied to 2 integers.");
		return NULL;
	}
	TypeUpgrading(LHS, RHS);
	return IRBuilder.CreateOr(LHS, RHS);
}

//Create a bitwise XOR instruction. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int ^ Int -> Int			(TypeUpgrading)
llvm::Value* CreateBitwiseXOR(llvm::Value* LHS, llvm::Value* RHS, CodeGenerator& Generator) {
	if (!(LHS->getType()->isIntegerTy() && RHS->getType()->isIntegerTy())) {
		throw std::domain_error("Bitwise XOR operator \"^\" must be applied to 2 integers.");
		return NULL;
	}
	TypeUpgrading(LHS, RHS);
	return IRBuilder.CreateXor(LHS, RHS);
}

//Create an assignment. This function will automatically do type casting
//if the two input values are not of the same type.
//Supported:
//1. Int = FP
//2. Int = Int
//3. Int = Pointer
//4. FP = Int
//5. FP = FP
//6. Pointer = Int
//7. Pointer = Pointer
//8. Exactly the same type assignment
//The "pLHS" argument should be a pointer pointing to the variable (the left-value in C)
llvm::Value* CreateAssignment(llvm::Value* pLHS, llvm::Value* RHS, CodeGenerator& Generator) {
	RHS = TypeCasting(RHS, pLHS->getType()->getNonOpaquePointerElementType());
	if (RHS == NULL) {
		throw std::domain_error("Assignment with values that cannot be cast to the target type.");
		return NULL;
	}
	IRBuilder.CreateStore(RHS, pLHS);
	return pLHS;
}