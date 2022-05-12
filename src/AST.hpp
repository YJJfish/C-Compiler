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
#include <exception>
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

class CodeGenerator;

//Namespace containing all classes involved in the construction of Abstract Syntax Tree (AST)
namespace AST {

	//Forward declarations

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
			class GlobalString;
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

	//Pure virtual class for statement
	class Stmt : public Node {
	public:
		Stmt(void) {}
		~Stmt(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator) = 0;
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
		//At most one of them can be not NULL.
		Expr* _InitialExpr;
		Constant* _InitialConstant;

		VarInit(const std::string& __Name) :
			_Name(__Name), _InitialExpr(NULL), _InitialConstant(NULL) {}
		VarInit(const std::string& __Name, Expr* __InitialExpr) :
			_Name(__Name), _InitialExpr(__InitialExpr), _InitialConstant(NULL) {}
		VarInit(const std::string& __Name, Constant* __InitialConstant) :
			_Name(__Name), _InitialExpr(NULL), _InitialConstant(__InitialConstant) {}
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

		DefinedType(const std::string& __Name) : _Name(__Name) {}
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
	public:
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

		Arg(VarType* __VarType, const std::string& __Name = "") :
			_VarType(__VarType), _Name(__Name) {}
		~Arg(void) {}
		//Arg class don't need an actual CodeGen function
		llvm::Value* CodeGen(CodeGenerator& __Generator) { return NULL; }
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
		Expr(void) {}
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
	
	//Operator sizeof() in C
	class SizeOf : public Expr {
	public:
		Expr* _Arg1;
		VarType* _Arg2;
		std::string _Arg3;
		SizeOf(Expr* __Arg1) : _Arg1(__Arg1), _Arg2(NULL), _Arg3("") {}
		SizeOf(VarType* __Arg2) : _Arg1(NULL), _Arg2(__Arg2), _Arg3("") {}
		SizeOf(const std::string& __Arg3) : _Arg1(NULL), _Arg2(NULL), _Arg3(__Arg3) {}
		~SizeOf(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	//Function call
	class FunctionCall : public Expr {
	public:
		std::string _FuncName;
		ExprList* _ArgList;
		FunctionCall(const std::string& __FuncName, ExprList* __ArgList) : _FuncName(__FuncName), _ArgList(__ArgList) {}
		~FunctionCall(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class StructReference : public Expr {
	public:
		Expr* _Struct;
		std::string _MemName;
		StructReference(Expr* __Struct, const std::string& __MemName) : _Struct(__Struct), _MemName(__MemName) {}
		~StructReference(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class StructDereference : public Expr {
	public:
		Expr* _StructPtr;
		std::string _MemName;
		StructDereference(Expr* __StructPtr, const std::string& __MemName) : _StructPtr(__StructPtr), _MemName(__MemName) {}
		~StructDereference(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class UnaryPlus : public Expr {
	public:
		Expr* _Operand;
		UnaryPlus(Expr* __Operand) : _Operand(__Operand) {}
		~UnaryPlus(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class UnaryMinus : public Expr {
	public:
		Expr* _Operand;
		UnaryMinus(Expr* __Operand) : _Operand(__Operand) {}
		~UnaryMinus(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class TypeCast : public Expr {
	public:
		VarType* _VarType;
		Expr* _Operand;
		TypeCast(VarType* __VarType, Expr* __Operand) : _VarType(__VarType), _Operand(__Operand) {}
		~TypeCast(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class PrefixInc : public Expr {
	public:
		Expr* _Operand;
		PrefixInc(Expr* __Operand) : _Operand(__Operand) {}
		~PrefixInc(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class PostfixInc : public Expr {
	public:
		Expr* _Operand;
		PostfixInc(Expr* __Operand) : _Operand(__Operand) {}
		~PostfixInc(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class PrefixDec : public Expr {
	public:
		Expr* _Operand;
		PrefixDec(Expr* __Operand) : _Operand(__Operand) {}
		~PrefixDec(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class PostfixDec : public Expr {
	public:
		Expr* _Operand;
		PostfixDec(Expr* __Operand) : _Operand(__Operand) {}
		~PostfixDec(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class Indirection : public Expr {
	public:
		Expr* _Operand;
		Indirection(Expr* __Operand) : _Operand(__Operand) {}
		~Indirection(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class AddressOf : public Expr {
	public:
		Expr* _Operand;
		AddressOf(Expr* __Operand) : _Operand(__Operand) {}
		~AddressOf(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class LogicNot : public Expr {
	public:
		Expr* _Operand;
		LogicNot(Expr* __Operand) : _Operand(__Operand) {}
		~LogicNot(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class BitwiseNot : public Expr {
	public:
		Expr* _Operand;
		BitwiseNot(Expr* __Operand) : _Operand(__Operand) {}
		~BitwiseNot(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class Division : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		Division(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~Division(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class Multiplication : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		Multiplication(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~Multiplication(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class Modulo : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		Modulo(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~Modulo(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class Addition : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		Addition(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~Addition(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class Subtraction : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		Subtraction(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~Subtraction(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class LeftShift : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		LeftShift(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~LeftShift(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class RightShift : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		RightShift(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~RightShift(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class LogicGT : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		LogicGT(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~LogicGT(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class LogicGE : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		LogicGE(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~LogicGE(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class LogicLT : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		LogicLT(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~LogicLT(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class LogicLE : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		LogicLE(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~LogicLE(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class LogicEQ : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		LogicEQ(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~LogicEQ(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class LogicNEQ : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		LogicNEQ(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~LogicNEQ(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class BitwiseAND : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		BitwiseAND(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~BitwiseAND(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class BitwiseXOR : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		BitwiseXOR(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~BitwiseXOR(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class BitwiseOR : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		BitwiseOR(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~BitwiseOR(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class LogicAND : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		LogicAND(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~LogicAND(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class LogicOR : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		LogicOR(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~LogicOR(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class TernaryCondition : public Expr {
	public:
		Expr* _Condition;
		Expr* _Then;
		Expr* _Else;
		TernaryCondition(Expr* __Condition, Expr* __Then, Expr* __Else) : _Condition(__Condition), _Then(__Then), _Else(__Else) {}
		~TernaryCondition(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class DirectAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		DirectAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~DirectAssign(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class DivAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		DivAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~DivAssign(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class MulAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		MulAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~MulAssign(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class ModAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		ModAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~ModAssign(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class AddAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		AddAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~AddAssign(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class SubAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		SubAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~SubAssign(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class SHLAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		SHLAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~SHLAssign(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};
	class SHRAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		SHRAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~SHRAssign(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class BitwiseANDAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		BitwiseANDAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~BitwiseANDAssign(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class BitwiseXORAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		BitwiseXORAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~BitwiseXORAssign(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class BitwiseORAssign : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		BitwiseORAssign(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~BitwiseORAssign(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class CommaExpr : public Expr {
	public:
		Expr* _LHS;
		Expr* _RHS;
		CommaExpr(Expr* __LHS, Expr* __RHS) : _LHS(__LHS), _RHS(__RHS) {}
		~CommaExpr(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class Variable : public Expr {
	public:
		std::string _Name;
		Variable(const std::string& __Name) : _Name(__Name) {}
		~Variable(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class Constant : public Expr {
	public:
		BuiltInType::TypeID _Type;
		bool _Bool;
		char _Character;
		int _Integer;
		double _Real;
		Constant(bool __Bool) :
			_Type(BuiltInType::TypeID::_Bool), _Bool(__Bool), _Character('\0'), _Integer(0), _Real(0.0) {}
		Constant(char __Character) :
			_Type(BuiltInType::TypeID::_Char), _Bool(false), _Character(__Character), _Integer(0), _Real(0.0) {}
		Constant(int __Integer) :
			_Type(BuiltInType::TypeID::_Int), _Bool(false), _Character('\0'), _Integer(__Integer), _Real(0.0) {}
		Constant(double __Real) :
			_Type(BuiltInType::TypeID::_Double), _Bool(false), _Character('\0'), _Integer(0), _Real(__Real) {}
		~Constant(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};

	class GlobalString : public Constant {
	public:
		std::string _Content;
		GlobalString(const std::string& __Content) : Constant(0), _Content(__Content) {}
		~GlobalString(void) {}
		llvm::Value* CodeGen(CodeGenerator& __Generator);
		llvm::Value* CodeGenPtr(CodeGenerator& __Generator);
	};
}