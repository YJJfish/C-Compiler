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
	llvm::Value* Program::CodeGen(CodeGenerator& __Generator) {
		for (auto Decl : *(this->_Decls))
			Decl->CodeGen(__Generator);
		return NULL;
	}

	//Function declaration
	llvm::Value* FuncDecl::CodeGen(CodeGenerator& __Generator) {
		//Set the argument type list. We need to call "GetLLVMType"
		//to change AST::VarType* type to llvm::Type* type
		std::vector<llvm::Type*> ArgTypes;
		for (auto ArgType : *(this->_ArgList))
			ArgTypes.push_back(ArgType->_VarType->GetLLVMType(__Generator));
		//Get function type
		llvm::FunctionType* FuncType = llvm::FunctionType::get(this->_RetType->GetLLVMType(__Generator), ArgTypes, false);
		//Create function
		llvm::Function* Func = llvm::Function::Create(FuncType, llvm::GlobalValue::InternalLinkage, this->_Name, __Generator.Module);
		//If the function name conflictes, there was already something with the same name.
		//If it already has a body, don't allow redefinition.
		if (Func->getName() != this->_Name) {
			//Delete the one we just made and get the existing one.
			Func->eraseFromParent();
			Func = __Generator.Module->getFunction(this->_Name);
			//If this function already has a body,
			//or the current declaration doesn't have a body,
			//reject this declaration.
			if (!Func->empty() || !this->_FuncBody) {
				std::cout << "[CodeGen] Redefinition of function " << this->_Name << ".\n";
				return NULL;
			}
			//Check number of args. If Func took a different number of args, reject.
			if (Func->arg_size() != ArgTypes.size()) {
				std::cout << "[CodeGen] Redefinition of function " << this->_Name << " with different number of args.\n";
				return NULL;
			}
			//Check arg types. If Func took different different arg types, reject.
			size_t Index = 0;
			for (auto ArgIter = Func->arg_begin(); ArgIter < Func->arg_end(); ArgIter++, Index++)
				if (ArgIter->getType() != ArgTypes[Index]) {
					std::cout << "[CodeGen] Redefinition of function " << this->_Name << " with different arg types.\n";
					return NULL;
				}
		}
		//If this function has a body, generate its body's code.
		if (this->_FuncBody) {
			//Create a new basic block to start insertion into.
			llvm::BasicBlock* FuncBlock = llvm::BasicBlock::Create(Context, "entry", Func);
			IRBuilder.SetInsertPoint(FuncBlock);
			//Create allocated space for arguments
			size_t Index = 0;
			for (auto ArgIter = Func->arg_begin(); ArgIter < Func->arg_end(); ArgIter++, Index++) {
				llvm::IRBuilder<> TmpB(&Func->getEntryBlock(), Func->getEntryBlock().begin());
				auto Alloc = TmpB.CreateAlloca(ArgTypes[Index], NULL, this->_ArgList->at(Index)->_Name);
				IRBuilder.CreateStore(ArgIter, Alloc);
			}
			//Generate code of the function body
			__Generator.FuncStack.push(Func);
			this->_FuncBody->CodeGen(__Generator);
			__Generator.FuncStack.pop();
			//We don't need to create return value explicitly,
			//because "return" statement will be used explicitly in the function body.
			/*
			if (!this->RetType->GetLLVMType(Generator)->isVoidTy())
				IRBuilder.CreateRet(RetVal);
			else
				IRBuilder.CreateRetVoid();
			*/
			//Reset insert point
			IRBuilder.SetInsertPoint(&(__Generator.FuncStack.top())->getBasicBlockList().back());
		}
		return Func;
	}

	//Variable declaration
	llvm::Value* VarDecl::CodeGen(CodeGenerator& __Generator) {

	}

	//Type declaration
	llvm::Value* TypeDecl::CodeGen(CodeGenerator& __Generator) {

	}
}