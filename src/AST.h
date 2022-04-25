/*
 ********************** AST.h ***********************
 * This file defines the class types of	the	nodes 	*
 * in AST (Abstract Tree).							*
 * All classes are defined in the AST namespace.	*
 ****************************************************
 */

#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <llvm/IR/Value.h>
#include "CodeGenerator.h"

//Forward declarations
namespace AST {

	class Program;
	class Decl;
	using Decls = std::vector<Decl*>;
	class FuncDecl;
	class VarDecl;
	using VarDecls = std::vector<VarDecl*>;
	class TypeDecl;
	class VarType;
	class Stmt;
	using Stmts = std::vector<Stmt*>;
	class Block;
	class Arg;
	using ArgList = std::vector<Arg*>;
	class VarInit;
	using VarList = std::vector<VarInit*>;
	class Expr;
	class Enm;
	using EnmList = std::vector<EnmList*>;

}

//Class definitions
namespace AST {

	//Pure virtual class for AST node
	class Node {
	public:
		Node(void) {}
		~Node(void) {}
		virtual llvm::Value* CodeGen(CodeGenerator& Generator) = 0;
	};

	//The root node of AST
	class Program : public Node {
	public:
		//A program is composed of several declarations
		Decls* DeclList;

		Program(Decls* _Decls) :DeclList(_Decls) {}
		~Program(void) {}
		llvm::Value* CodeGen(CodeGenerator& Generator);
	};

	//Pure virtual class for Declaration node
	class Decl : public Node {
	public:
		Decl(void) {}
		~Decl(void) {}
		virtual llvm::Value* CodeGen(CodeGenerator& Generator) = 0;
	};

	//Function declaration
	class FuncDecl : public Decl {
	public:
		//The return type of the function
		VarType* RetType;
		//Its name
		std::string Name;
		//The argument list of the function
		ArgList* _ArgList;
		//The function body (its implementation)
		//If no block is provided, FuncBody is set to be NULL,
		//meaning that this is just a function prototype declaration.
		Block* FuncBody;

		FuncDecl(VarType* RetType, const std::string& Name, ArgList* _ArgList, Block* FuncBody = NULL) :
			RetType(RetType), Name(Name), _ArgList(_ArgList), FuncBody(FuncBody) {}
		~FuncDecl(void) {}
		llvm::Value* CodeGen(CodeGenerator& Generator);
	};

	//Variable declaration
	class VarDecl : public Decl {
	public:
		//The variable type
		VarType* _VarType;
		//The variable list
		VarList* _VarList;

		VarDecl(VarType* _VarType, VarList* _VarList) :
			_VarType(_VarType), _VarList(_VarList) {}
		~VarDecl(void) {}
		llvm::Value* CodeGen(CodeGenerator& Generator);
	};
	
	//A variable in one variable declaration line
	class VarInit : public Node {
	public:
		//The name of the variable
		std::string Name;
		//The initial value (if any) of this variable
		Expr* InitialValue;

		VarInit(const std::string& Name, Expr* InitialValue = NULL) :
			Name(Name), InitialValue(InitialValue) {}
		~VarInit(void) {}
		llvm::Value* CodeGen(CodeGenerator& Generator);
	};

	//Type declaration
	class TypeDecl : public Decl {
	public:
		//Variable type
		VarType* _VarType;
		//Its alias
		std::string Alias;

		TypeDecl(VarType* _VarType, const std::string& Alias) :
			_VarType(_VarType), Alias(Alias) {}
		~TypeDecl(void) {}
		llvm::Value* CodeGen(CodeGenerator& Generator);
	};

	//Base class for variable type
	class VarType : public Node {
	public:
		//Whether this type is const
		bool isConst;
		//Whether this type is an array type
		bool isArray;
		//Array length
		size_t Length;
		//Return a pointer to the corresponding instance of llvm::Type
		llvm::Type* GetLLVMType(void) const;
	};

	//Enumeration class
	class Enm : public Node {
	public:
		//Its name
		std::string Name;
		//Its value (if any)
		bool hasValue;
		int Value;

		Enm(const std::string& Name, bool hasValue = false, int Value = 0) :
			Name(Name), hasValue(hasValue), Value(Value) {}
		~Enm(void) {}
	};

	//Function argument
	class Arg : public Node {
	public:
		//Its type
		VarType* _VarType;
		//Its name (if any)
		std::string Name;

		Arg(VarType* _VarType, const std::string Name = "") :
			_VarType(_VarType), Name(Name) {}
		~Arg(void) {}
		llvm::Value* CodeGen(CodeGenerator& Generator);
	};

	//Pure virtual class for statement
	class Stmt : public Node {
	public:
		Stmt(void) {}
		~Stmt(void) {}
		llvm::Value* CodeGen(CodeGenerator& Generator) = 0;
	};

	//Statement block
	class Block : public Stmt {
	public:
		//Its content
		Stmts* Content;
		
		Block(Stmts* Content) :Content(Content) {}
		~Block(void) {}
		llvm::Value* CodeGen(CodeGenerator& Generator);
	};
}