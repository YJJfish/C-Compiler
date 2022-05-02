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
			//Create allocated space for arguments.
			__Generator.PushVariableTable();	//This variable table is only used to store the arguments of the function
			size_t Index = 0;
			for (auto ArgIter = Func->arg_begin(); ArgIter < Func->arg_end(); ArgIter++, Index++) {
				//Create alloca
				auto Alloc = CreateEntryBlockAlloca(Func, this->_ArgList->at(Index)->_Name, ArgTypes[Index]);
				//Assign the value by "store" instruction
				IRBuilder.CreateStore(ArgIter, Alloc);
				//Add to the symbol table
				__Generator.AddVariable(this->_ArgList->at(Index)->_Name, Alloc);
			}
			//Generate code of the function body
			__Generator.PushFunction(Func);
			__Generator.PushTypedefTable();
			__Generator.PushVariableTable();
			this->_FuncBody->CodeGen(__Generator);
			__Generator.PopVariableTable();
			__Generator.PopTypedefTable();
			__Generator.PopFunction();
			__Generator.PopVariableTable();	//We need to pop out an extra variable table.
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
				//Create an alloca.
				auto Alloc = CreateEntryBlockAlloca(__Generator.GetCurrentFunction(), NewVar->_Name, VarType);
				if (!__Generator.AddVariable(NewVar->_Name, Alloc)) {
					std::cout << "Redefinition of local variable " << NewVar->_Name << "." << std::endl;
					Alloc->eraseFromParent();
				}
				//Assign the initial value by "store" instruction.
				if (NewVar->_InitialValue)
					IRBuilder.CreateStore(NewVar->_InitialValue->CodeGen(__Generator), Alloc);
				//TODO: llvm::AllocaInst doesn't has the "constant" attribute, so we need to implement it manually.
				//Unfortunately, I haven't worked out a graceful solution, and the only way I can do is to add a "const"
				//label to the corresponding entry in the symbol table.
			}
			else {
				//create a global variable.
				//Create the constant initializer
				llvm::Constant* Initializer = NULL;
				if (NewVar->_InitialValue)
					if (!NewVar->_InitialValue->_IsConstant) {
						//Global variable must be initialized (if any) by a constant.
						std::cout << "[CodeGen] Initializing global variable " << NewVar->_Name << " with non-constant value.\n";
						return NULL;
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
				if (!__Generator.AddVariable(NewVar->_Name, Alloc)) {
					std::cout << "Redefinition of global variable " << NewVar->_Name << "." << std::endl;
					Alloc->eraseFromParent();
				}
			}
		}
		return NULL;
	}

	//Type declaration
	llvm::Value* TypeDecl::CodeGen(CodeGenerator& __Generator) {
		//Add an item to the current typedef symbol table
		//If an old value exists (i.e., conflict), raise an error
		llvm::Type* LLVMType = this->_VarType->GetLLVMType(__Generator);
		if (!LLVMType) {
			std::cout << "Typedef " << this->_Alias << " using undefined types." << std::endl;
			return NULL;
		}
		if (__Generator.AddType(this->_Alias, LLVMType))
			std::cout << "Redefinition of typename " << this->_Alias << std::endl;
		return NULL;
	}

	//Built-in type
	llvm::Type* BuiltInType::GetLLVMType(CodeGenerator& __Generator) {
		if (this->_LLVMType)
			return this->_LLVMType;
		switch (this->_Type) {
		case _Bool: this->_LLVMType = IRBuilder.getInt1Ty(); break;
		case _Short: this->_LLVMType = IRBuilder.getInt16Ty(); break;
		case _Int: this->_LLVMType = IRBuilder.getInt32Ty(); break;
		case _Long: this->_LLVMType = IRBuilder.getInt64Ty(); break;
		case _Char: this->_LLVMType = IRBuilder.getInt8Ty(); break;
		case _Float: this->_LLVMType = IRBuilder.getFloatTy(); break;
		case _Double: this->_LLVMType = IRBuilder.getDoubleTy(); break;
		case _Void: this->_LLVMType = IRBuilder.getVoidTy(); break;
		default: break;
		}
		return this->_LLVMType;
	}

	//Defined type. Use this class when only an identifier is given.
	llvm::Type* DefinedType::GetLLVMType(CodeGenerator& __Generator) {
		if (this->_LLVMType)
			return this->_LLVMType;
		return this->_LLVMType = __Generator.FindType(this->_Name);
	}

	//Pointer type.
	llvm::Type* PointerType::GetLLVMType(CodeGenerator& __Generator) {
		if (this->_LLVMType)
			return this->_LLVMType;
		llvm::Type* BaseType = this->_BaseType->GetLLVMType(__Generator);
		return this->_LLVMType = llvm::PointerType::get(BaseType, 0U);
	}

	//Array type.
	llvm::Type* ArrayType::GetLLVMType(CodeGenerator& __Generator) {
		if (this->_LLVMType)
			return this->_LLVMType;
		llvm::Type* BaseType = this->_BaseType->GetLLVMType(__Generator);
		return this->_LLVMType = llvm::ArrayType::get(BaseType, this->_Length);
	}

	//Struct type.
	llvm::Type* StructType::GetLLVMType(CodeGenerator& __Generator) {
		if (this->_LLVMType)
			return this->_LLVMType;
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
			return this->_LLVMType;
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
		//Generate the statements in Block, one by one.
		for (auto& Stmt : *(this->_Content))
			//If the current block already has a terminator,
			//i.e. a "break" statement is generated, stop;
			//Otherwise, continue generating.
			if (IRBuilder.GetInsertBlock()->getTerminator())
				break;
			else if (Stmt)	//We allow empty-statement which is represented by NULL pointer.
				Stmt->CodeGen(__Generator);
		__Generator.PopVariableTable();
		__Generator.PopTypedefTable();
		return NULL;
	}

	//If statement
	llvm::Value* IfStmt::CodeGen(CodeGenerator& __Generator) {
		//Evaluate condition
		//Since we don't allow variable declarations in if-condition (because we only allow expressions there),
		//we don't need to push a symbol table
		llvm::Value* Condition = this->_Condition->CodeGen(__Generator);
		//Cast the type to i1
		if (!(Condition = Cast2I1(Condition))) {
			std::cout << "The condition value of if-statement must be either an integer, or a floating-point number, or a pointer." << std::endl;
			return NULL;
		}
		//Create "Then", "Else" and "Merge" block
		llvm::Function* CurrentFunc = __Generator.GetCurrentFunction();
		llvm::BasicBlock* ThenBB = llvm::BasicBlock::Create(Context, "Then");
		llvm::BasicBlock* ElseBB = llvm::BasicBlock::Create(Context, "Else");
		llvm::BasicBlock* MergeBB = llvm::BasicBlock::Create(Context, "Merge");
		//Create a branch instruction corresponding to this if statement
		IRBuilder.CreateCondBr(Condition, ThenBB, ElseBB);
		//Generate code in the "Then" block
		CurrentFunc->getBasicBlockList().push_back(ThenBB);
		IRBuilder.SetInsertPoint(ThenBB);
		if (this->_Then) {
			__Generator.PushTypedefTable();
			__Generator.PushVariableTable();
			this->_Then->CodeGen(__Generator);
			__Generator.PopVariableTable();
			__Generator.PopTypedefTable();
		}
		TerminateBlockByBr(MergeBB);
		//Generate code in the "Else" block
		CurrentFunc->getBasicBlockList().push_back(ElseBB);
		IRBuilder.SetInsertPoint(ElseBB);
		if (this->_Else) {
			__Generator.PushTypedefTable();
			__Generator.PushVariableTable();
			this->_Else->CodeGen(__Generator);
			__Generator.PopVariableTable();
			__Generator.PopTypedefTable();
		}
		TerminateBlockByBr(MergeBB);
		//Finish "Merge" block
		CurrentFunc->getBasicBlockList().push_back(MergeBB);
		IRBuilder.SetInsertPoint(MergeBB);
		return NULL;
	}

	//While statement
	llvm::Value* WhileStmt::CodeGen(CodeGenerator& __Generator) {
		//Create "WhileCond", "WhileLoop" and "WhileEnd"
		llvm::Function* CurrentFunc = __Generator.GetCurrentFunction();
		llvm::BasicBlock* WhileCondBB = llvm::BasicBlock::Create(Context, "WhileCond");
		llvm::BasicBlock* WhileLoopBB = llvm::BasicBlock::Create(Context, "WhileLoop");
		llvm::BasicBlock* WhileEndBB = llvm::BasicBlock::Create(Context, "WhileEnd");
		//Create an unconditional branch, jump to "WhileCond" block.
		IRBuilder.CreateBr(WhileCondBB);
		//Evaluate the loop condition (cast the type to i1 if necessary).
		//Since we don't allow variable declarations in if-condition (because we only allow expressions there),
		//we don't need to push a symbol table
		CurrentFunc->getBasicBlockList().push_back(WhileCondBB);
		IRBuilder.SetInsertPoint(WhileCondBB);
		llvm::Value* Condition = this->_Condition->CodeGen(__Generator);
		if (!(Condition = Cast2I1(Condition))) {
			std::cout << "The condition value of while-statement must be either an integer, or a floating-point number, or a pointer." << std::endl;
			return NULL;
		}
		IRBuilder.CreateCondBr(Condition, WhileLoopBB, WhileEndBB);
		//Generate code in the "WhileLoop" block
		CurrentFunc->getBasicBlockList().push_back(WhileLoopBB);
		IRBuilder.SetInsertPoint(WhileLoopBB);
		if (this->_LoopBody) {
			__Generator.EnterLoop(WhileCondBB, WhileEndBB);	//Don't forget to call "EnterLoop"
			__Generator.PushTypedefTable();
			__Generator.PushVariableTable();
			this->_LoopBody->CodeGen(__Generator);
			__Generator.PopVariableTable();
			__Generator.PopTypedefTable();
			__Generator.LeaveLoop();						//Don't forget to call "LeaveLoop"
		}
		TerminateBlockByBr(WhileCondBB);
		//Finish "WhileEnd" block
		CurrentFunc->getBasicBlockList().push_back(WhileEndBB);
		IRBuilder.SetInsertPoint(WhileEndBB);
		return NULL;
	}

	//Do statement
	llvm::Value* DoStmt::CodeGen(CodeGenerator& __Generator) {
		//Create "DoLoop", "DoCond" and "DoEnd"
		llvm::Function* CurrentFunc = __Generator.GetCurrentFunction();
		llvm::BasicBlock* DoLoopBB = llvm::BasicBlock::Create(Context, "DoLoop");
		llvm::BasicBlock* DoCondBB = llvm::BasicBlock::Create(Context, "DoCond");
		llvm::BasicBlock* DoEndBB = llvm::BasicBlock::Create(Context, "DoEnd");
		//Create an unconditional branch, jump to "DoLoop" block.
		IRBuilder.CreateBr(DoLoopBB);
		//Generate code in the "DoLoop" block
		CurrentFunc->getBasicBlockList().push_back(DoLoopBB);
		IRBuilder.SetInsertPoint(DoLoopBB);
		if (this->_LoopBody) {
			__Generator.EnterLoop(DoCondBB, DoEndBB);		//Don't forget to call "EnterLoop"
			__Generator.PushTypedefTable();
			__Generator.PushVariableTable();
			this->_LoopBody->CodeGen(__Generator);
			__Generator.PopVariableTable();
			__Generator.PopTypedefTable();
			__Generator.LeaveLoop();						//Don't forget to call "LeaveLoop"
		}
		TerminateBlockByBr(DoCondBB);
		//Evaluate the loop condition (cast the type to i1 if necessary).
		//Since we don't allow variable declarations in if-condition (because we only allow expressions there),
		//we don't need to push a symbol table
		CurrentFunc->getBasicBlockList().push_back(DoCondBB);
		IRBuilder.SetInsertPoint(DoCondBB);
		llvm::Value* Condition = this->_Condition->CodeGen(__Generator);
		if (!(Condition = Cast2I1(Condition))) {
			std::cout << "The condition value of do-statement must be either an integer, or a floating-point number, or a pointer." << std::endl;
			return NULL;
		}
		IRBuilder.CreateCondBr(Condition, DoLoopBB, DoEndBB);
		//Finish "DoEnd" block
		CurrentFunc->getBasicBlockList().push_back(DoEndBB);
		IRBuilder.SetInsertPoint(DoEndBB);
		return NULL;
	}

	//For statement
	llvm::Value* ForStmt::CodeGen(CodeGenerator& __Generator) {
		//Create "ForCond", "ForLoop", "ForTail" and "ForEnd"
		llvm::Function* CurrentFunc = __Generator.GetCurrentFunction();
		llvm::BasicBlock* ForCondBB = llvm::BasicBlock::Create(Context, "ForCond");
		llvm::BasicBlock* ForLoopBB = llvm::BasicBlock::Create(Context, "ForLoop");
		llvm::BasicBlock* ForTailBB = llvm::BasicBlock::Create(Context, "ForTail");
		llvm::BasicBlock* ForEndBB = llvm::BasicBlock::Create(Context, "ForEnd");
		//Evaluate the initial statement, and create an unconditional branch to "ForCond" block
		//Since we allow variable declarations here, we need to push a new symbol table
		//e.g., for (int i = 0; ...; ...) { ... }
		if (this->_Initial) {
			__Generator.PushTypedefTable();
			__Generator.PushVariableTable();
			this->_Initial->CodeGen(__Generator);
		}
		TerminateBlockByBr(ForCondBB);
		//Generate code in the "ForCond" block
		CurrentFunc->getBasicBlockList().push_back(ForCondBB);
		IRBuilder.SetInsertPoint(ForCondBB);
		if (this->_Condition) {
			//If it has a loop condition, evaluate it (cast the type to i1 if necessary).
			llvm::Value* Condition = this->_Condition->CodeGen(__Generator);
			if (!(Condition = Cast2I1(Condition))) {
				std::cout << "The condition value of for-statement must be either an integer, or a floating-point number, or a pointer." << std::endl;
				return NULL;
			}
			IRBuilder.CreateCondBr(Condition, ForLoopBB, ForEndBB);
		}
		else {
			//Otherwise, it is an unconditional loop condition (always returns true)
			IRBuilder.CreateBr(ForLoopBB);
		}
		//Generate code in the "ForLoop" block
		CurrentFunc->getBasicBlockList().push_back(ForLoopBB);
		IRBuilder.SetInsertPoint(ForLoopBB);
		if (this->_LoopBody) {
			__Generator.EnterLoop(ForTailBB, ForEndBB);		//Don't forget to call "EnterLoop"
			__Generator.PushTypedefTable();
			__Generator.PushVariableTable();
			this->_LoopBody->CodeGen(__Generator);
			__Generator.PopVariableTable();
			__Generator.PopTypedefTable();
			__Generator.LeaveLoop();						//Don't forget to call "LeaveLoop"
		}
		//If not terminated, jump to "ForTail"
		TerminateBlockByBr(ForTailBB);
		//Generate code in the "ForTail" block
		CurrentFunc->getBasicBlockList().push_back(ForTailBB);
		IRBuilder.SetInsertPoint(ForTailBB);
		if (this->_Tail)
			this->_Tail->CodeGen(__Generator);
		IRBuilder.CreateBr(ForCondBB);
		//Finish "ForEnd" block
		CurrentFunc->getBasicBlockList().push_back(ForEndBB);
		IRBuilder.SetInsertPoint(ForEndBB);
		if (this->_Initial) {
			__Generator.PopVariableTable();
			__Generator.PopTypedefTable();
		}
		return NULL;
	}

	//Switch statement
	llvm::Value* SwitchStmt::CodeGen(CodeGenerator& __Generator) {
		llvm::Function* CurrentFunc = __Generator.GetCurrentFunction();
		//Evaluate condition
		//Since we don't allow variable declarations in switch-matcher (because we only allow expressions there),
		//we don't need to push a symbol table.
		llvm::Value* Matcher = this->_Matcher->CodeGen(__Generator);
		//Create one block for each case statement.
		std::vector<llvm::BasicBlock*> CaseBB;
		for (int i = 0; i < this->_CaseList->size(); i++)
			CaseBB.push_back(llvm::BasicBlock::Create(Context, "Case" + std::to_string(i)));
		//Create an extra block for SwitchEnd
		CaseBB.push_back(llvm::BasicBlock::Create(Context, "SwitchEnd"));
		//Create one block for each comparison.
		std::vector<llvm::BasicBlock*> ComparisonBB;
		ComparisonBB.push_back(IRBuilder.GetInsertBlock());
		for (int i = 1; i < this->_CaseList->size(); i++)
			ComparisonBB.push_back(llvm::BasicBlock::Create(Context, "Comparison" + std::to_string(i)));
		ComparisonBB.push_back(CaseBB.back());
		//Generate branches
		for (int i = 0; i < this->_CaseList->size(); i++) {
			if (i) {
				CurrentFunc->getBasicBlockList().push_back(ComparisonBB[i]);
				IRBuilder.SetInsertPoint(ComparisonBB[i]);
			}
			if (this->_CaseList->at(i)->_Condition)	//Have condition
				IRBuilder.CreateCondBr(
					CreateCmpEQ(Matcher, this->_CaseList->at(i)->_Condition->CodeGen(__Generator)),
					CaseBB[i],
					ComparisonBB[i + 1]
				);
			else									//Default
				IRBuilder.CreateBr(CaseBB[i]);
		}
		//Generate code for each case statement
		__Generator.PushTypedefTable();
		__Generator.PushVariableTable();
		for (int i = 0; i < this->_CaseList->size(); i++) {
			CurrentFunc->getBasicBlockList().push_back(CaseBB[i]);
			IRBuilder.SetInsertPoint(CaseBB[i]);
			__Generator.EnterLoop(CaseBB[i + 1], CaseBB.back());
			this->_CaseList->at(i)->CodeGen(__Generator);
			__Generator.LeaveLoop();
		}
		__Generator.PopVariableTable();
		__Generator.PopTypedefTable();
		//Finish "SwitchEnd" block
		CurrentFunc->getBasicBlockList().push_back(CaseBB.back());
		IRBuilder.SetInsertPoint(CaseBB.back());
		return NULL;
	}

	//Case statement in switch statement
	llvm::Value* CaseStmt::CodeGen(CodeGenerator& __Generator) {
		//Generate the statements, one by one.
		for (auto& Stmt : *(this->_Content))
			//If the current block already has a terminator,
			//i.e. a "break" statement is generated, stop;
			//Otherwise, continue generating.
			if (IRBuilder.GetInsertBlock()->getTerminator())
				break;
			else if (Stmt)	//We allow empty-statement which is represented by NULL pointer.
				Stmt->CodeGen(__Generator);
		//If not terminated, jump to the next case block
		TerminateBlockByBr(__Generator.GetContinueBlock());
		return NULL;
	}

	//Continue statement
	llvm::Value* ContinueStmt::CodeGen(CodeGenerator& __Generator) {
		llvm::BasicBlock* ContinueTarget = __Generator.GetContinueBlock();
		if (ContinueTarget)
			IRBuilder.CreateBr(ContinueTarget);
		else
			std::cout << "Continue statement should only be used in loops or switch statements." << std::endl;
		return NULL;
	}

	//Break statement
	llvm::Value* BreakStmt::CodeGen(CodeGenerator& __Generator) {
		llvm::BasicBlock* BreakTarget = __Generator.GetBreakBlock();
		if (BreakTarget)
			IRBuilder.CreateBr(BreakTarget);
		else
			std::cout << "Break statement should only be used in loops or switch statements." << std::endl;
		return NULL;
	}

	//Return statement
	llvm::Value* ReturnStmt::CodeGen(CodeGenerator& __Generator) {
		llvm::Function* Func = __Generator.GetCurrentFunction();
		if (!Func) {
			std::cout << "Return statement should only be used in function bodies." << std::endl;
			return NULL;
		}
		if (this->_RetVal == NULL) {
			if (Func->getReturnType()->isVoidTy())
				IRBuilder.CreateRetVoid();
			else {
				std::cout << "Expected an expression after return statement." << std::endl;
				return NULL;
			}
		}
		else {
			llvm::Value* RetVal = TypeCasting(this->_RetVal->CodeGen(__Generator), Func->getReturnType());
			if (!RetVal) {
				std::cout << "The type of return value doesn't match and cannot be cast to the return type." << std::endl;
				return NULL;
			}
			IRBuilder.CreateRet(RetVal);
		}
	}
}