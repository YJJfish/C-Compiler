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

	/*** Root ***/
	class Program;
	
	/*** Declarations ***/
	class Decl;
	using Decls = std::vector<Decl*>;
		class FuncDecl;
			class Arg;
			using ArgList = std::vector<Arg*>;
			class FuncBody;
		class VarDecl;
			class VarInit;
			using VarList = std::vector<VarInit*>;
		class TypeDecl;

	/*** Variable Types ***/
	class VarType;
		class DefinedType;
		class PointerType;
		class ArrayType;
		class StructType;
			class FieldDecl;
			using FieldDecls = std::vector<FieldDecl*>;
			using MemList = std::vector<std::string>;
		class EnumType;
			class Enm;
			using EnmList = std::vector<Enm*>;

	/*** Statements ***/
	class Stmt;
	using Stmts = std::vector<Stmt*>;
		class IfStmt;
		class ForStmt;
		class WhileStmt;
		class DoStmt;
		class SwitchStmt;
			class CaseStmt;
			using CaseList = std::vector<CaseStmt*>;
		class BreakStmt;
		class ContinueStmt;
		class ReturnStmt;
		class Block;
		
	/*** Expressions ***/
	class Expr;
		class Subscript;
		class SizeOf;
		class FunctionCall;
			using ExprList = std::vector<Expr*>;
		class StructReference;
		class StructDereference;
		class UnaryPlus;
		class UnaryMinus;
		class TypeCast;
		class PrefixInc;
		class PostfixInc;
		class PrefixDec;
		class PostfixDec;
		class Indirection;
		class AddressOf;
		class LogicNot;
		class BitwiseNot;
		class Division;
		class Multiplication;
		class Modulo;
		class Addition;
		class Subtraction;
		class LeftShift;
		class RightShift;
		class LogicGT;
		class LogicGE;
		class LogicLT;
		class LogicLE;
		class LogicEQ;
		class LogicNEQ;
		class BitwiseAND;
		class BitwiseXOR;
		class BitwiseOR;
		class LogicAND;
		class LogicOR;
		class TernaryCondition;
		class DirectAssign;
		class DivAssign;
		class MulAssign;
		class ModAssign;
		class AddAssign;
		class SubAssign;
		class SHLAssign;
		class SHRAssign;
		class BitwiseANDAssign;
		class BitwiseXORAssign;
		class BitwiseORAssign;
		class CommaExpr;
		class Variable;
		class Constant;
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

		FuncDecl(VarType* __RetType, const std::string& __Name, ArgList* __ArgList, FuncBody* __FuncBody = NULL) :
			_RetType(__RetType), _Name(__Name), _ArgList(__ArgList), _FuncBody(__FuncBody) {}
		~FuncDecl(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Function body
	class FuncBody : public Node {
	public:
		//Its content
		Stmts* _Content;

		FuncBody(Stmts* __Content) :_Content(__Content) {}
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
	
	//A variable in one variable declaration
	class VarInit : public Node {
	public:
		//The name of the variable
		std::string _Name;
		//The initial value (if any) of this variable
		Expr* _InitialValue;

		VarInit(const std::string& __Name, Expr* __InitialValue = NULL) :
			_Name(__Name), _InitialValue(__InitialValue) {}
		~VarInit(void) {}
		//VarInit class don't need an actual CodeGen function
		llvm::Value* CodeGen(CodeGenerator& __Generator) { return NULL; }
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
		//Its LLVM type. It is initialized as NULL, and generated by function GetLLVMType.
		llvm::Type* _LLVMType;

		VarType(void) : _isConst(false), _LLVMType(NULL) {}
		~VarType(void) {}
		//Set this variable type to be constant.
		void SetConst(void) {
			this->_isConst = true;
		}
		//Return the corresponding instance of llvm::Type*.
		//Meanwhile, it will update _LLVMType.
		virtual llvm::Type* GetLLVMType(CodeGenerator& __Generator) = 0;
		//VarType class don't need an actual CodeGen function
		llvm::Value* CodeGen(CodeGenerator& __Generator) { return NULL; }
	};

	//Built-in type
	class BuiltInType : public VarType {
	public:
		//Enum of built-in type
		enum TypeID {
			_Bool,
			_Short,
			_Int,
			_Long,
			_Char,
			_Float,
			_Double,
			_Void
		};
		//Type ID
		TypeID _Type;

		BuiltInType(TypeID __Type) : _Type(__Type) {}
		~BuiltInType(void) {}
		//Return the corresponding instance of llvm::Type*.
		//Meanwhile, it will update _LLVMType.
		llvm::Type* GetLLVMType(CodeGenerator& __Generator);
	};

	//Defined type. Use this class when only an identifier is given.
	class DefinedType : public VarType {
	public:
		//Its name.
		std::string _Name;

		DefinedType(std::string __Name) : _Name(__Name) {}
		~DefinedType(void) {}
		//Return the corresponding instance of llvm::Type*.
		//Meanwhile, it will update _LLVMType.
		llvm::Type* GetLLVMType(CodeGenerator& __Generator);
	};

	//Pointer type.
	class PointerType : public VarType {
	public:
		//Base type.
		//For example, the base type for "int ptr" is "int"
		VarType* _BaseType;

		PointerType(VarType* __BaseType) : _BaseType(__BaseType) {}
		~PointerType(void) {}
		//Return the corresponding instance of llvm::Type*.
		//Meanwhile, it will update _LLVMType.
		llvm::Type* GetLLVMType(CodeGenerator& __Generator);
	};

	//Array Type
	class ArrayType : public VarType {
	public:
		//Base type and array length
		//For example, the base type and array length for "int[20]"
		//is "int" and 20, respectively.
		VarType* _BaseType;
		size_t _Length;

		ArrayType(VarType* __BaseType, size_t __Length) : _BaseType(__BaseType), _Length(__Length) {}
		~ArrayType(void) {}
		//Return the corresponding instance of llvm::Type*.
		//Meanwhile, it will update _LLVMType.
		llvm::Type* GetLLVMType(CodeGenerator& __Generator);
	};

	//Struct Type
	class StructType : public VarType {
	public:
		//Struct body
		FieldDecls* _StructBody;

		StructType(FieldDecls* __StructBody) : _StructBody(__StructBody) {}
		~StructType(void) {}
		//Return the corresponding instance of llvm::Type*.
		//Meanwhile, it will update _LLVMType.
		llvm::Type* GetLLVMType(CodeGenerator& __Generator);
	};

	//Field declaration for struct type
	class FieldDecl : public Decl {
	public:
		//The variable type for this declaration
		VarType* _VarType;
		//The list of variable names for this declaration
		MemList* _MemList;

		FieldDecl(VarType* __VarType, MemList* __MemList) :_VarType(__VarType), _MemList(__MemList) {}
		~FieldDecl(void) {}
		//FieldDecl class don't need an actual CodeGen function
		llvm::Value* CodeGen(CodeGenerator& __Generator) { return NULL; }
	};

	//Enum Type
	class EnumType : public VarType {
		//The list of member names for this enum type
		EnmList* _EnmList;

		EnumType(EnmList* __EnmList) : _EnmList(__EnmList) {}
		~EnumType(void) {}
		//Return the corresponding instance of llvm::Type*.
		//Meanwhile, it will update _LLVMType.
		llvm::Type* GetLLVMType(CodeGenerator& __Generator);
	};

	//A single enum member
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
		//Enm class don't need an actual CodeGen function
		llvm::Value* CodeGen(CodeGenerator& __Generator) { return NULL; }
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
		//Arg class don't need an actual CodeGen function
		llvm::Value* CodeGen(CodeGenerator& __Generator) { return NULL; }
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

	//If statement
	class IfStmt : public Stmt {
	public:
		//Branch condition, then-clause and else-clause
		Expr* _Condition;
		Stmt* _Then;
		Stmt* _Else;

		IfStmt(Expr* __Condition, Stmt* __Then, Stmt* __Else = NULL) : _Condition(__Condition), _Then(__Then), _Else(__Else) {}
		~IfStmt(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//While statement
	class WhileStmt : public Stmt {
	public:
		//Loop condition and loop body
		Expr* _Condition;
		Stmt* _LoopBody;

		WhileStmt(Expr* __Condition, Stmt* __LoopBody) : _Condition(__Condition), _LoopBody(__LoopBody) {}
		~WhileStmt(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Do statement
	class DoStmt : public Stmt {
	public:
		//Loop condition and loop body
		Stmt* _LoopBody;
		Expr* _Condition;

		DoStmt(Stmt* __LoopBody, Expr* __Condition) : _LoopBody(__LoopBody), _Condition(__Condition) {}
		~DoStmt(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//For statement
	class ForStmt : public Stmt {
	public:
		//For-initial, loop condition, for-tail and loop body
		Stmt* _Initial;
		Expr* _Condition;
		Expr* _Tail;
		Stmt* _LoopBody;

		ForStmt(Stmt* __Initial, Expr* __Condition, Expr* __Tail, Stmt* __LoopBody) :
			_Initial(__Initial), _Condition(__Condition), _Tail(__Tail), _LoopBody(__LoopBody) {}
		~ForStmt(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Switch statement
	class SwitchStmt : public Stmt {
	public:
		//Switch-matcher and case statement list
		Expr* _Matcher;
		CaseList* _CaseList;

		SwitchStmt(Expr* __Matcher, CaseList* __CaseList) : _Matcher(__Matcher), _CaseList(__CaseList) {}
		~SwitchStmt(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Case statement in switch statement
	class CaseStmt : public Stmt {
	public:
		//Case condition. Set NULL if the condition is "default".
		Expr* _Condition;
		//The statements to be excuted.
		Stmts* _Content;
		
		CaseStmt(Expr* __Condition, Stmts* __Content) : _Condition(__Condition), _Content(__Content) {}
		~CaseStmt(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Continue statement
	class ContinueStmt : public Stmt {
	public:
		ContinueStmt(void) {}
		~ContinueStmt(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Break statement
	class BreakStmt : public Stmt {
	public:
		BreakStmt(void) {}
		~BreakStmt(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Return statement
	class ReturnStmt : public Stmt {
	public:
		//The expression to be returned (if any)
		Expr* _RetVal;
		ReturnStmt(Expr* __RetVal = NULL) : _RetVal(__RetVal) {}
		~ReturnStmt(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
	};

	//Pure virtual class for expression
	class Expr : public Stmt {
	public:
		//to determine whether CodeGen will return llvm::Constant*
		//instead of llvm::Value*
		bool _IsConstant;
		//to determine whether CodeGen will return llvm::AllocaInst* or llvm::GlobalVariable*
		//instead of llvm::Value*
		bool _IsLeftValue;

		Expr(void) : _IsConstant(false), _IsLeftValue(false) {}
		~Expr(void) {}
		//This function is used to get the "value" of the expression.
		virtual llvm::Value* CodeGen(CodeGenerator& __Generator) = 0;
		//This function is used to get the "pointer" of the instance.
		//It is used to implement the "left value" in C language,
		//e.g., the LHS of the assignment.
		virtual llvm::Value* CodeGenPtr(CodeGenerator& __Generator) = 0;
	};

	//Subscript, e.g. a[10]
	class Subscript : public Expr {
	public:
		Expr* _Array;
		Expr* _Index;
		Subscript(Expr* __Array, Expr* __Index) : _Array(__Array), _Index(__Index) {}
		~Subscript(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};
	class SizeOf : public Expr {
	public:
		Expr* _Arg;
	};
	class FunctionCall : public Expr {
	public:
		std::string _FuncName;
		ExprList* _ArgList;
	};
	class StructReference : public Expr {
	public:
		Expr* _Struct;
		std::string _MemName;
	};
	class StructDereference : public Expr {
	public:
		Expr* _StructPtr;
		std::string _MemName;
	};
	class UnaryPlus : public Expr {
	public:
		Expr* _Operand;
	};
	class UnaryMinus : public Expr {
	public:
		Expr* _Operand;
	};
	class TypeCast : public Expr {
	public:
		VarType* _VarType;
		Expr* _Operand;
	};
	class PrefixInc : public Expr {
	public:
		Expr* _Operand;
	};
	class PostfixInc : public Expr {
	public:
		Expr* _Operand;
	};
	class PrefixDec : public Expr {
	public:
		Expr* _Operand;
	};
	class PostfixDec : public Expr {
	public:
		Expr* _Operand;
	};
	class Indirection : public Expr {
	public:
		Expr* _Operand;
	};
	class AddressOf : public Expr {
	public:
		Expr* _Operand;
	};
	class LogicNot : public Expr {
	public:
		Expr* _Operand;
	};
	class BitwiseNot : public Expr {
	public:
		Expr* _Operand;
	};
	class Division : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class Multiplication : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class Modulo : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class Addition : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class Subtraction : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class LeftShift : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class RightShift : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class LogicGT : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class LogicGE : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class LogicLT : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class LogicLE : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class LogicEQ : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class LogicNEQ : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class BitwiseAND : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class BitwiseXOR : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class BitwiseOR : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class LogicAND : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class LogicOR : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class TernaryCondition : public Expr {
	public:
		Expr* _Condition;
		Expr* _Then;
		Expr* _Else;
	};
	class DirectAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class DivAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class MulAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class ModAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class AddAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class SubAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class SHLAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class SHRAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class BitwiseANDAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class BitwiseXORAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class BitwiseORAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class CommaExpr : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
	};
	class Variable : public Expr {
	public:
		std::string _Name;
	};
	class Constant : public Expr {
	public:
		BuiltInType::TypeID _Type;
		std::string _String;
		char _Character;
		int _Integer;
		double _Real;
	};
}