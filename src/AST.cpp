/*
 ********************* AST.cpp **********************
 * This file implements the methods of the node		*
 * classes in the AST.								*
 * All classes are defined in the AST namespace.	*
 ****************************************************
 */

#include "AST.h"

namespace AST {
	//A program is composed of several declarations
	llvm::Value* Program::CodeGen(CodeGenerator& Generator) {
		for (auto Decl : *(this->DeclList))
			Decl->CodeGen(Generator);
		return NULL;
	}

	//Function declaration
	llvm::Value* FuncDecl::CodeGen(CodeGenerator& Generator) {
		//Set the argument type list. We need to call "GetLLVMType"
		//to change AST::VarType* type to llvm::Type* type
		std::vector<llvm::Type*> ArgTypes;
		for (auto ArgType : *(this->_ArgList))
			ArgTypes.push_back(ArgType->_VarType->GetLLVMType());
		//Get function type
		llvm::FunctionType* FuncType = llvm::FunctionType::get(this->RetType->GetLLVMType(), ArgTypes, false);
		//Create function
		llvm::Function* Func = llvm::Function::Create(FuncType, llvm::GlobalValue::InternalLinkage, this->Name, Generator.Module);
		//If the function name conflictes, there was already something with the same name.
		//If it has a body, don't allow redefinition.
		if (Func->getName() != this->Name) {
			//Delete the one we just made and get the existing one.
			Func->eraseFromParent();
			Func = Generator.Module->getFunction(this->Name);
			//If this function already has a body,
			//or the current declaration doesn't have a body,
			//reject this declaration.
			if (!Func->empty() || !this->FuncBody) {
				std::cout << "[CodeGen] Redefinition of function " << this->Name << ".\n";
				return NULL;
			}
			//Check arg types. If Func took a different number of args, reject.
			if (Func->arg_size() != ArgTypes.size()) {
				std::cout << "[CodeGen] Redefinition of function " << this->Name << " with different args.\n";
				return NULL;
			}
		}
		//If this function has a body, generate its body's code.
		if (this->FuncBody) {	
			//Create a new basic block to start insertion into.
			llvm::BasicBlock* FuncBlock = llvm::BasicBlock::Create(Context, "entry", Func);
			IRBuilder.SetInsertPoint(FuncBlock);
			//Create allocated space for arguments
			size_t Index = 0;
			for (auto ArgIter = Func->arg_begin(); ArgIter < Func->arg_end(); ArgIter++, Index++) {
				llvm::IRBuilder<> TmpB(&Func->getEntryBlock(), Func->getEntryBlock().begin());
				auto Alloc = TmpB.CreateAlloca(ArgTypes[Index], NULL, this->_ArgList->at(Index)->Name);
				IRBuilder.CreateStore(ArgIter, Alloc);
			}
			//Generate code of the function body
			Generator.FuncStack.push(Func);
			llvm::Value* RetVal = this->FuncBody->CodeGen(Generator);
			Generator.FuncStack.pop();
			//Create return value
			if (!this->RetType->GetLLVMType()->isVoidTy())
				IRBuilder.CreateRet(RetVal);
			else
				IRBuilder.CreateRetVoid();
			//Reset insert point
			IRBuilder.SetInsertPoint(&(Generator.FuncStack.top())->getBasicBlockList().back());
		}
		return Func;
	}
}