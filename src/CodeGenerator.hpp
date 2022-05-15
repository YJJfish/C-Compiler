/*
 ***************** CodeGenerator.h ******************
 * This file defines the CodeGenerator class,		*
 * along with some global variables needed for		*
 * code generation.									*
 ****************************************************
 */

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <exception>
#include <llvm/IR/Value.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/PassManager.h>
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
#include <llvm/Support/TargetSelect.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>


#include "AST.hpp"

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

//Since llvm's structs' members don't have names, we need to implement it manually.
//Our solution is creating a mapping from llvm::StructType* to AST::StructType*
using IdentifiedStructTypeTable = std::map<llvm::StructType*, AST::StructType*>;

//VariableTable class
//Since LLVM doesn't support visiting all variables explicitly,
//we need to implement it manually.
using VariableTable = std::map<std::string, llvm::Value*>;

//CodeGenerator class
class CodeGenerator {
public:
	llvm::Module* Module;
	llvm::DataLayout* DL;
private:
	llvm::Function* CurrFunction;						//Current function
	IdentifiedStructTypeTable* StructTypeTable;			//Struct type table
	std::vector<TypedefTable*> TypedefStack;			//Typedef symbol table
	std::vector<VariableTable*> VariableStack;			//Local variable symbol table
	std::vector<llvm::BasicBlock*> ContinueBlockStack;	//Store blocks for "continue" statement
	std::vector<llvm::BasicBlock*> BreakBlockStack;		//Store blocks for "break" statement
	llvm::BasicBlock* TmpBB;							//Temp block for global instruction code generation
	llvm::Function* TmpFunc;							//Temp function for global instruction code generation
public:
	//Constructor
	CodeGenerator(void) :
		Module(new llvm::Module("main", Context)),
		DL(new llvm::DataLayout(Module)),
		CurrFunction(NULL),
		StructTypeTable(NULL),
		TypedefStack(),
		VariableStack(),
		ContinueBlockStack(),
		BreakBlockStack(),
		TmpBB()
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

	//Find the AST::VarType* instance for the given name
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

	//Find variable
	llvm::Value* FindVariable(std::string Name) {
		for (auto TableIter = this->VariableStack.end() - 1; TableIter >= this->VariableStack.begin(); TableIter--) {
			auto PairIter = (**TableIter).find(Name);
			if (PairIter != (**TableIter).end())
				return PairIter->second;
		}
		return NULL;
	}

	//Add an item to the current variable symbol table
	//If an old value exists (i.e., conflict), return false
	bool AddVariable(std::string Name, llvm::Value* Variable) {
		if (this->VariableStack.size() == 0) return false;
		auto& TopTable = *(this->VariableStack.back());
		auto PairIter = TopTable.find(Name);
		if (PairIter != TopTable.end())
			return false;
		TopTable[Name] = Variable;
		return true;
	}

	AST::StructType* FindStructType(llvm::StructType* Ty1) {
		auto PairIter = this->StructTypeTable->find(Ty1);
		if (PairIter != this->StructTypeTable->end())
			return PairIter->second;
		return NULL;
	}

	bool AddStructType(llvm::StructType* Ty1, AST::StructType* Ty2) {
		auto PairIter = this->StructTypeTable->find(Ty1);
		if (PairIter != this->StructTypeTable->end())
			return false;
		(*this->StructTypeTable)[Ty1] = Ty2;
		return true;
	}

	//Set current function
	void EnterFunction(llvm::Function* Func) {
		this->CurrFunction = Func;
	}

	//Remove current function
	void LeaveFunction(void) {
		this->CurrFunction = NULL;
	}

	//Get the current function
	llvm::Function* GetCurrentFunction(void) {
		return this->CurrFunction;
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

	void XchgInsertPointWithTmpBB(void) {
		auto Tmp = IRBuilder.GetInsertBlock();
		IRBuilder.SetInsertPoint(this->TmpBB);
		this->TmpBB = Tmp;
	}

	//Pass the root of the ast to this function and generate code
	void GenerateCode(AST::Program& Root, const std::string& OptimizeLevel = "") {

		//Initialize symbol table
		this->StructTypeTable = new IdentifiedStructTypeTable;
		this->PushTypedefTable();
		this->PushVariableTable();

		//Create a temp function and a temp block for global instruction code generation
		this->TmpFunc = llvm::Function::Create(llvm::FunctionType::get(IRBuilder.getVoidTy(), false), llvm::GlobalValue::InternalLinkage, "0Tmp", this->Module);
		this->TmpBB = llvm::BasicBlock::Create(Context, "Temp", this->TmpFunc);

		//Generate code
		Root.CodeGen(*this);
		std::cout << "Gen Successfully" << std::endl;

		//Delete TmpBB and TmpFunc
		this->TmpBB->eraseFromParent();
		this->TmpFunc->eraseFromParent();

		//Delete symbol table
		this->PopTypedefTable();
		this->PopVariableTable();
		delete this->StructTypeTable; this->StructTypeTable = NULL;

		//Run optimization
		if (OptimizeLevel != "") {
			//Create the analysis managers.
			llvm::LoopAnalysisManager LAM;
			llvm::FunctionAnalysisManager FAM;
			llvm::CGSCCAnalysisManager CGAM;
			llvm::ModuleAnalysisManager MAM;
			//Create the new pass manager builder.
			llvm::PassBuilder PB;
			//Register all the basic analyses with the managers.
			PB.registerModuleAnalyses(MAM);
			PB.registerCGSCCAnalyses(CGAM);
			PB.registerFunctionAnalyses(FAM);
			PB.registerLoopAnalyses(LAM);
			PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
			//Create the pass manager.
			const llvm::OptimizationLevel* OptLevel;
			if (OptimizeLevel == "O0")
				OptLevel = &llvm::OptimizationLevel::O0;
			else if (OptimizeLevel == "O1")
				OptLevel = &llvm::OptimizationLevel::O1;
			else if (OptimizeLevel == "O2")
				OptLevel = &llvm::OptimizationLevel::O2;
			else if (OptimizeLevel == "O3")
				OptLevel = &llvm::OptimizationLevel::O3;
			else if (OptimizeLevel == "Os")
				OptLevel = &llvm::OptimizationLevel::Os;
			else if (OptimizeLevel == "Oz")
				OptLevel = &llvm::OptimizationLevel::Oz;
			else
				OptLevel = &llvm::OptimizationLevel::O0;
			llvm::ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(*OptLevel);
			//Optimize the IR
			MPM.run(*this->Module, MAM);
		}
	}

	void DumpIRCode(std::string FileName) {
		if (FileName == "") FileName = "-";
		std::error_code EC;
		llvm::raw_fd_ostream Out(FileName, EC);
		Out << "********************  IRCode  ********************\n";
		this->Module->print(Out, NULL);
		Out << "*****************  Verification  *****************\n";
		if (llvm::verifyModule(*this->Module, &Out) == 0)
			Out << "No errors.\n";
	}

	void GenExecutable(std::string FileName) {
		return;
	}
};