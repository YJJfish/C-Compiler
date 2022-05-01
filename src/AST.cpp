/*
 ********************* AST.cpp **********************
 * This file implements the methods of the node		*
 * classes in the AST.								*
 * All classes are defined in the AST namespace.	*
 ****************************************************
 */

#include "AST.h"
#include "Utils.hpp"

namespace AST {
	//A program is composed of several declarations
	llvm::Value* Program::CodeGen(CodeGenerator& __Generator) {
		for (auto Decl : *(this->_Decls))
			if (Decl)	//We allow empty-declaration which is represented by NULL pointer.
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
			//Check return type. If Func took different different return types, reject.
			if (this->_RetType->GetLLVMType(__Generator) != Func->getReturnType()) {
				std::cout << "[CodeGen] Redefinition of function " << this->_Name << " with different return types.\n";
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
				//Create alloca
				llvm::IRBuilder<> TmpB(&Func->getEntryBlock(), Func->getEntryBlock().begin());
				auto Alloc = TmpB.CreateAlloca(ArgTypes[Index], NULL, this->_ArgList->at(Index)->_Name);
				//Assign the value by "store" instruction
				IRBuilder.CreateStore(ArgIter, Alloc);
			}
			//Generate code of the function body
			__Generator.PushFunction(Func);
			__Generator.PushTypedefTable();
			__Generator.PushVariableTable();
			this->_FuncBody->CodeGen(__Generator);
			__Generator.PopVariableTable();
			__Generator.PopTypedefTable();
			__Generator.PopFunction();
			//We don't need to create return value explicitly,
			//because "return" statement will be used explicitly in the function body.
		}
		return NULL;
	}

	//Function body
	llvm::Value* FuncBody::CodeGen(CodeGenerator& __Generator) {
		//Generate the statements in FuncBody, one by one.
		for (auto& Stmt : *(this->_Content))
			//If the current block already has a terminator,
			//i.e. a "return" statement is generated, stop;
			//Otherwise, continue generating.
			if (IRBuilder.GetInsertBlock()->getTerminator())
				break;
			else if (Stmt)	//We allow empty-statement which is represented by NULL pointer.
				Stmt->CodeGen(__Generator);
		return NULL;
	}

	//Variable declaration
	llvm::Value* VarDecl::CodeGen(CodeGenerator& __Generator) {
		//Get the llvm type
		llvm::Type* VarType = this->_VarType->GetLLVMType(__Generator);
		//Create variables one by one.
		for (auto& NewVar : *(this->_VarList)) {
			//Determine whether the declaration is inside a function.
			//If so, create an alloca;
			//Otherwise, create a global variable.
			if (__Generator.GetCurrentFunction()) {
				//Create an alloca
				auto Alloc = IRBuilder.CreateAlloca(VarType, NULL, NewVar->_Name);
				__Generator.AddVariable(NewVar->_Name, Alloc);
				//Assign the value by "store" instruction
				if (NewVar->_InitialValue)
					IRBuilder.CreateStore(NewVar->_InitialValue->CodeGen(__Generator), Alloc);
				//TODO: llvm::AllocaInst doesn't has the "constant" attribute, so we need to implement it manually.
				//Unfortunately, I haven't worked out a graceful solution.
			}
			else {
				//Create the constant initializer
				llvm::Constant* Initializer = NULL;
				if (NewVar->_InitialValue)
					if (!NewVar->_InitialValue->_IsConstant) {
						//Global variable must be initialized (if any) by a constant.
						std::cout << "[CodeGen] Initializing global variable " << NewVar->_Name << " with non-constant value.\n";
					}
					else
						Initializer = (llvm::Constant*)NewVar->_InitialValue->CodeGen(__Generator);
				//Create a global variable
				auto Alloc = new llvm::GlobalVariable(
					*(__Generator.Module),
					VarType,
					this->_VarType->_isConst,
					llvm::Function::ExternalLinkage,
					Initializer,
					NewVar->_Name
				);
				__Generator.AddVariable(NewVar->_Name, Alloc);
			}
		}
		return NULL;
	}

	//Type declaration
	llvm::Value* TypeDecl::CodeGen(CodeGenerator& __Generator) {
		//Add an item to the current typedef symbol table
		//If an old value exists (i.e., conflict), raise an error
		if (__Generator.AddType(this->_Alias, this->_VarType->GetLLVMType(__Generator)))
			std::cout << "Redefinition of typename " << this->_Alias << std::endl;
		return NULL;
	}

	//Pointer type.
	llvm::Type* PointerType::GetLLVMType(CodeGenerator& __Generator) {
		if (this->_LLVMType)
			return _LLVMType;
		llvm::Type* BaseType = this->_BaseType->GetLLVMType(__Generator);
		return this->_LLVMType = llvm::PointerType::get(BaseType, 0U);
	}

	//Array type.
	llvm::Type* ArrayType::GetLLVMType(CodeGenerator& __Generator) {
		if (this->_LLVMType)
			return _LLVMType;
		llvm::Type* BaseType = this->_BaseType->GetLLVMType(__Generator);
		return this->_LLVMType = llvm::ArrayType::get(BaseType, this->_Length);
	}

	//Struct type.
	llvm::Type* StructType::GetLLVMType(CodeGenerator& __Generator) {
		if (this->_LLVMType)
			return _LLVMType;
		//Generate the body of the struct type
		std::vector<llvm::Type*> Members;
		for (auto FDecl : *(this->_StructBody))
			if (FDecl)	//We allow empty-declaration which is represented by NULL pointer.
				Members.insert(Members.end(), FDecl->_MemList->size(), FDecl->_VarType->GetLLVMType(__Generator));
		return this->_LLVMType = llvm::StructType::get(Context, Members);
	}

	//Enum type
	llvm::Type* EnumType::GetLLVMType(CodeGenerator& __Generator) {
		if (this->_LLVMType)
			return _LLVMType;
		//Generate the body of the enum type
		int LastVal = -1;
		for (auto Mem : *(this->_EnmList))
			if (Mem->_hasValue)
				LastVal = Mem->_Value;
			else {
				Mem->_hasValue = true;
				Mem->_Value = ++LastVal;
			}
		//Enum type is actually an int32 type.
		return llvm::IntegerType::getInt32Ty(Context);
	}

	//Statement block
	llvm::Value* Block::CodeGen(CodeGenerator& __Generator) {
		__Generator.PushTypedefTable();
		__Generator.PushVariableTable();
		//Generate the statements in FuncBody, one by one.
		for (auto& Stmt : *(this->_Content))
			if (Stmt)	//We allow empty-statement which is represented by NULL pointer.
				Stmt->CodeGen(__Generator);
		__Generator.PopTypedefTable();
		__Generator.PopFunction();
		return NULL;
	}

	//If statement
	llvm::Value* IfStmt::CodeGen(CodeGenerator& __Generator) {
		//Evaluate condition
		llvm::Value* Condition = this->_Condition->CodeGen(__Generator);
		//Cast the type to i1
		if (!(Condition = Cast2I1(Condition))) {
			std::cout << "The condition value of if-statement must be either an integer, or a floating-point number, or a pointer." << std::endl;
			return NULL;
		}
		//Create "Then", "Else" and "Merge" block
		llvm::Function* CurrentFunc = __Generator.GetCurrentFunction();
		llvm::BasicBlock* ThenBB = llvm::BasicBlock::Create(Context, "Then", CurrentFunc);
		llvm::BasicBlock* ElseBB = llvm::BasicBlock::Create(Context, "Else");
		llvm::BasicBlock* MergeBB = llvm::BasicBlock::Create(Context, "Merge");
		//Create a branch instruction
		IRBuilder.CreateCondBr(Condition, ThenBB, ElseBB);
		//Generate code in the "Then" block
		IRBuilder.SetInsertPoint(ThenBB);
		if (this->_Then)
			this->_Then->CodeGen(__Generator);
		IRBuilder.CreateBr(MergeBB);
		//Generate code in the "Else" block
		CurrentFunc->getBasicBlockList().push_back(ElseBB);
		IRBuilder.SetInsertPoint(ElseBB);
		if (this->_Else)
			this->_Else->CodeGen(__Generator);
		IRBuilder.CreateBr(ElseBB);
		//Finish "Merge" block
		CurrentFunc->getBasicBlockList().push_back(MergeBB);
		return NULL;
	}
}