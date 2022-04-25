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

//CodeGenerator class
class CodeGenerator {
public:
	llvm::Module* Module;
	llvm::Function* GlobalField, * Printf, * Scanf;
	std::stack<llvm::Function*> FuncStack;
	unsigned int AddrSpace;
	
	//Constructor
	CodeGenerator(void) :
		Module(new llvm::Module("main", Context)),
		GlobalField(NULL),
		Printf(NULL),
		Scanf(NULL),
		AddrSpace(this->Module->getDataLayout().getAllocaAddrSpace())
	{}
	//Pass the root of the ast to this function and generate code
	void GenerateCode(AST::Program& Root) {
		//Main function
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
		this->FuncStack.pop();

		//Print result
		this->Module->print(llvm::errs(), NULL);
	}
};