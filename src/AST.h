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
#include "CodeGenerator.h"

//Forward declarations
namespace AST {

	class Program;
	class Decl;
	using Decls = std::vector<Decl*>;
	class FuncDecl;
	class FuncBody;
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
		virtual llvm::Value* CodeGen(CodeGenerator& __Generator) = 0;
	};

	//The root node of AST
	class Program : public Node {
	public:
		//A program is composed of several declarations
		Decls* _Decls;

		Program(Decls* _Decls) :_Decls(_Decls) {}
		~Program(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Pure virtual class for Declarations
	class Decl : public Stmt {
	public:
		Decl(void) {}
		~Decl(void) {}
		virtual llvm::Value* CodeGen(CodeGenerator& __Generator) = 0;
	};

	//Function declaration
	class FuncDecl : public Decl {
	public:
		//The return type of the function
		VarType* _RetType;
		//Its name
		std::string _Name;
		//The argument list of the function
		ArgList* _ArgList;
		//The function body (its implementation)
		//If no block is provided, FuncBody is set to be NULL,
		//meaning that this is just a function prototype declaration.
		FuncBody* _FuncBody;

		FuncDecl(VarType* __RetType, const std::string& __Name, ArgList* __ArgList, Block* __FuncBody = NULL) :
			_RetType(__RetType), _Name(__Name), _ArgList(__ArgList), _FuncBody(__FuncBody) {}
		~FuncDecl(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Function body
	class FuncBody : public Node {
	public:
		//Its content
		Stmts* _Content;
		//FuncBody needs to know the return type in order to do type casting
		llvm::Type* _RetType;
		FuncBody(Stmts* __Content) :_Content(__Content), _RetType(NULL) {}
		~FuncBody(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Variable declaration
	class VarDecl : public Decl {
	public:
		//The variable type
		VarType* _VarType;
		//The variable list
		VarList* _VarList;

		VarDecl(VarType* __VarType, VarList* __VarList) :
			_VarType(__VarType), _VarList(__VarList) {}
		~VarDecl(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};
	
	//A variable in one variable declaration line
	class VarInit : public Node {
	public:
		//The name of the variable
		std::string _Name;
		//The initial value (if any) of this variable
		Expr* _InitialValue;

		VarInit(const std::string& __Name, Expr* __InitialValue = NULL) :
			_Name(__Name), _InitialValue(__InitialValue) {}
		~VarInit(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Type declaration
	class TypeDecl : public Decl {
	public:
		//Variable type
		VarType* _VarType;
		//Its alias
		std::string _Alias;

		TypeDecl(VarType* __VarType, const std::string& __Alias) :
			_VarType(__VarType), _Alias(__Alias) {}
		~TypeDecl(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Base class for variable type
	class VarType : public Node {
	public:
		//Whether this type is const
		bool _isConst;
		//Whether this type is an array type
		bool _isArray;
		//Array length
		size_t _Length;
		//Return a pointer to the corresponding instance of llvm::Type
		llvm::Type* GetLLVMType(CodeGenerator& __Generator) const;
		//Vartype class don't need an actual CodeGen function
		llvm::Value* CodeGen(CodeGenerator& __Generator) { return NULL; }
	};

	//Enumeration class
	class Enm : public Node {
	public:
		//Its name
		std::string _Name;
		//Its value (if any)
		bool _hasValue;
		int _Value;

		Enm(const std::string& __Name, bool __hasValue = false, int __Value = 0) :
			_Name(__Name), _hasValue(__hasValue), _Value(__Value) {}
		~Enm(void) {}
	};

	//Function argument
	class Arg : public Node {
	public:
		//Its type
		VarType* _VarType;
		//Its name (if any)
		std::string _Name;

		Arg(VarType* __VarType, const std::string __Name = "") :
			_VarType(__VarType), _Name(__Name) {}
		~Arg(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Pure virtual class for statement
	class Stmt : public Node {
	public:
		Stmt(void) {}
		~Stmt(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator) = 0;
	};

	//Statement block
	class Block : public Stmt {
	public:
		//Its content
		Stmts* _Content;
		
		Block(Stmts* __Content) :_Content(__Content) {}
		~Block(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};
}