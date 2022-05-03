/*
 ***************** CodeGenerator.h ******************
 * This file defines the CodeGenerator class,		*
 * along with some global variables needed for		*
 * code generation.									*
 ****************************************************
 */

#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <llvm/IR/Value.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Verifier.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Target/TargetMachine.h>


#include "AST.h"

//The global context.
extern llvm::LLVMContext Context;

//A helper object that makes it easy to generate LLVM instructions.
//It keeps track of the current place to insert instructions and
//has methods to create new instructions.
extern llvm::IRBuilder<> IRBuilder;

//TypedefTable class
//Since LLVM doesn't support "typedef" explicitly,
//we need to implement it manually.
using TypedefTable = std::map<std::string, llvm::Type*>;

//VariableTable class
//Since LLVM doesn't support visiting all alloca instances explicitly,
//we need to implement it manually.
using VariableTable = std::map<std::string, llvm::AllocaInst*>;

//Similar to VariableTable, but for llvm::GlobalVariable*
using GlobalVariableTable = std::map<std::string, llvm::GlobalVariable*>;

//CodeGenerator class
class CodeGenerator {
public:
	llvm::Module* Module;
	llvm::DataLayout* DL;
private:
	std::vector<llvm::Function*> FuncStack;				//Global function symbol table
	std::vector<TypedefTable*> TypedefStack;			//Typedef symbol table
	std::vector<VariableTable*> VariableStack;			//Local variable symbol table
	std::vector<llvm::BasicBlock*> ContinueBlockStack;	//Store blocks for "continue" statement
	std::vector<llvm::BasicBlock*> BreakBlockStack;		//Store blocks for "break" statement
	GlobalVariableTable* GlobalVarTable;
	unsigned int AddrSpace;
public:
	//Constructor
	CodeGenerator(void) :
		Module(new llvm::Module("main", Context)),
		DL(new llvm::DataLayout(Module)),
		FuncStack(),
		TypedefStack(),
		VariableStack(),
		ContinueBlockStack(),
		BreakBlockStack(),
		GlobalVarTable(NULL),
		AddrSpace(this->Module->getDataLayout().getAllocaAddrSpace())
	{}

	//Create and push an empty TypedefTable
	void PushTypedefTable(void) {
		this->TypedefStack.push_back(new TypedefTable);
	}

	//Remove the last TypedefTable
	void PopTypedefTable(void) {
		if (this->TypedefStack.size() == 0) return;
		delete this->TypedefStack.back();
		this->TypedefStack.pop_back();
	}

	//Find the llvm::Type* instance for the given name
	llvm::Type* FindType(std::string Name) {
		for (auto TableIter = this->TypedefStack.end() - 1; TableIter >= this->TypedefStack.begin(); TableIter--) {
			auto PairIter = (**TableIter).find(Name);
			if (PairIter != (**TableIter).end())
				return PairIter->second;
		}
		return NULL;
	}

	//Add an item to the current typedef symbol table
	//If an old value exists (i.e., conflict), return false
	bool AddType(std::string Name, llvm::Type* Type) {
		if (this->TypedefStack.size() == 0) return false;
		auto& TopTable = *(this->TypedefStack.back());
		auto PairIter = TopTable.find(Name);
		if (PairIter != TopTable.end())
			return false;
		TopTable[Name] = Type;
		return true;
	}

	//Create and push an empty VariableTable
	void PushVariableTable(void) {
		this->VariableStack.push_back(new VariableTable);
	}

	//Remove the last VariableTable
	void PopVariableTable(void) {
		if (this->VariableStack.size() == 0) return;
		delete this->VariableStack.back();
		this->VariableStack.pop_back();
	}

	//Find the llvm::AllocaInst* instance for the given name
	llvm::AllocaInst* FindLocalVariable(std::string Name) {
		for (auto TableIter = this->VariableStack.end() - 1; TableIter >= this->VariableStack.begin(); TableIter--) {
			auto PairIter = (**TableIter).find(Name);
			if (PairIter != (**TableIter).end())
				return PairIter->second;
		}
		return NULL;
	}

	//Find the llvm::GlobalVariable* instance for the given name
	llvm::GlobalVariable* FindGlobalVariable(std::string Name) {
		auto PairIter = (*this->GlobalVarTable).find(Name);
		if (PairIter != (*this->GlobalVarTable).end())
			return PairIter->second;
		return NULL;
	}

	//A combination of FindLocalVariable and FindGlobalVariable
	llvm::Value* FindVariable(std::string Name) {
		auto Ret1 = this->FindLocalVariable(Name);
		if (Ret1) return Ret1;
		auto Ret2 = this->FindGlobalVariable(Name);
		return Ret2;
	}

	//Add an item to the current variable symbol table
	//If an old value exists (i.e., conflict), return false
	bool AddVariable(std::string Name, llvm::Value* Variable) {
		//If the Variable stack is empty, add this variable to the global scope.
		//Otherwise, add this variable to the local scope.
		if (this->VariableStack.size() == 0) {
			auto& TopTable = *(this->GlobalVarTable);
			auto PairIter = TopTable.find(Name);
			if (PairIter != TopTable.end())
				return false;
			TopTable[Name] = (llvm::GlobalVariable*)Variable;
			return true;
		}
		else {
			auto& TopTable = *(this->VariableStack.back());
			auto PairIter = TopTable.find(Name);
			if (PairIter != TopTable.end())
				return false;
			TopTable[Name] = (llvm::AllocaInst*)Variable;
			return true;
		}
	}

	//Push a new function
	void PushFunction(llvm::Function* Func) {
		this->FuncStack.push_back(Func);
	}

	//Remove the last function
	void PopFunction(void) {
		if (this->FuncStack.size() == 0) return;
		this->FuncStack.pop_back();
	}

	//Get the current function
	llvm::Function* GetCurrentFunction(void) {
		if (this->FuncStack.size())
			return this->FuncStack.back();
		else
			return NULL;
	}

	//Called whenever entering a loop
	void EnterLoop(llvm::BasicBlock* ContinueBB, llvm::BasicBlock* BreakBB) {
		this->ContinueBlockStack.push_back(ContinueBB);
		this->BreakBlockStack.push_back(BreakBB);
	}

	//Called whenever leaving a loop
	void LeaveLoop(void) {
		if (this->ContinueBlockStack.size() == 0 || this->BreakBlockStack.size() == 0) return;
		this->ContinueBlockStack.pop_back();
		this->BreakBlockStack.pop_back();
	}

	//Get the destination block for "continue" statements
	llvm::BasicBlock* GetContinueBlock(void) {
		if (this->ContinueBlockStack.size())
			return this->ContinueBlockStack.back();
		else
			return NULL;
	}

	//Get the destination block for "break" statements
	llvm::BasicBlock* GetBreakBlock(void) {
		if (this->BreakBlockStack.size())
			return this->BreakBlockStack.back();
		else
			return NULL;
	}

	//Pass the root of the ast to this function and generate code
	void GenerateCode(AST::Program& Root) {
		//Main function
		/*
		std::vector<llvm::Type*> ArgTypes;
		llvm::FunctionType* MainFuncType = llvm::FunctionType::get(IRBuilder.getVoidTy(), ArgTypes, false);
		this->GlobalField = llvm::Function::Create(MainFuncType, llvm::GlobalValue::InternalLinkage, "global", *(this->Module));
		llvm::BasicBlock* EntryBlock = llvm::BasicBlock::Create(Context, "entry", this->GlobalField, 0);
		this->FuncStack.push(this->GlobalField);
		IRBuilder.SetInsertPoint(EntryBlock);
		//Create printf function
		ArgTypes.clear();
		ArgTypes.push_back(IRBuilder.getInt8PtrTy());
		llvm::FunctionType* PrintfType = llvm::FunctionType::get(IRBuilder.getInt32Ty(), ArgTypes, true);
		this->Printf = llvm::Function::Create(PrintfType, llvm::Function::ExternalLinkage, "printf", *(this->Module));
		this->Printf->setCallingConv(llvm::CallingConv::C);
		//Create scanf function
		llvm::FunctionType* ScanfType = llvm::FunctionType::get(IRBuilder.getInt32Ty(), true);
		this->Scanf = llvm::Function::Create(ScanfType, llvm::Function::ExternalLinkage, "scanf", *(this->Module));
		this->Scanf->setCallingConv(llvm::CallingConv::C);

		//Generate code
		Root.CodeGen(*this);

		//Return
		IRBuilder.CreateRetVoid();
		this->FuncStack.pop();*/
		
		//Initialize symbol table
		this->PushTypedefTable();
		this->GlobalVarTable = new GlobalVariableTable;

		//Generate code
		Root.CodeGen(*this);

		//Delete symbol table
		this->PopTypedefTable();
		delete this->GlobalVarTable;
		this->GlobalVarTable = NULL;

		//Print result
		this->Module->print(llvm::errs(), NULL);
	}
};