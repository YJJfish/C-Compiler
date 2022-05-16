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
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>


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
private:
	llvm::DataLayout* DL;								//Data layout
	llvm::Function* CurrFunction;						//Current function
	IdentifiedStructTypeTable* StructTypeTable;			//Struct type table
	std::vector<TypedefTable*> TypedefStack;			//Typedef symbol table
	std::vector<VariableTable*> VariableStack;			//Variable symbol table
	std::vector<llvm::BasicBlock*> ContinueBlockStack;	//Store blocks for "continue" statement
	std::vector<llvm::BasicBlock*> BreakBlockStack;		//Store blocks for "break" statement
	llvm::BasicBlock* TmpBB;							//Temp block for global instruction code generation
	llvm::Function* TmpFunc;							//Temp function for global instruction code generation
public:
	//Constructor
	CodeGenerator(void);

	//Sizeof()
	llvm::TypeSize GetTypeSize(llvm::Type* Type);

	//Create and push an empty TypedefTable
	void PushTypedefTable(void);

	//Remove the last TypedefTable
	void PopTypedefTable(void);

	//Find the AST::VarType* instance for the given name
	llvm::Type* FindType(std::string Name);

	//Add an item to the current typedef symbol table
	//If an old value exists (i.e., conflict), return false
	bool AddType(std::string Name, llvm::Type* Type);

	//Create and push an empty VariableTable
	void PushVariableTable(void);

	//Remove the last VariableTable
	void PopVariableTable(void);

	//Find variable
	llvm::Value* FindVariable(std::string Name);

	//Add an item to the current variable symbol table
	//If an old value exists (i.e., conflict), return false
	bool AddVariable(std::string Name, llvm::Value* Variable);

	//Find the AST::StructType* instance according to the llvm::StructType* instance
	AST::StructType* FindStructType(llvm::StructType* Ty1);

	//Add a <llvm::StructType*, AST::StructType*> mapping
	bool AddStructType(llvm::StructType* Ty1, AST::StructType* Ty2);

	//Set current function
	void EnterFunction(llvm::Function* Func);

	//Remove current function
	void LeaveFunction(void);

	//Get the current function
	llvm::Function* GetCurrentFunction(void);

	//Called whenever entering a loop
	void EnterLoop(llvm::BasicBlock* ContinueBB, llvm::BasicBlock* BreakBB);

	//Called whenever leaving a loop
	void LeaveLoop(void);

	//Get the destination block for "continue" statements
	llvm::BasicBlock* GetContinueBlock(void);

	//Get the destination block for "break" statements
	llvm::BasicBlock* GetBreakBlock(void);

	//Exchange the current insert point with TmpBB
	void XchgInsertPointWithTmpBB(void);

	//Pass the root of the ast to this function and generate code
	void GenerateCode(AST::Program& Root, const std::string& OptimizeLevel = "");

	//Dump the LLVM-IR to the specified file
	void DumpIRCode(std::string FileName);

	//Generate object code
	void GenObjectCode(std::string FileName);
};