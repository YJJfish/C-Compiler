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
		for (auto ArgType : *(this->_ArgList)) {
			llvm::Type* LLVMType = ArgType->_VarType->GetLLVMType(__Generator);
			if (!LLVMType) {
				std::cout << "Defining a function using unknown type(s)." << std::endl;
				return NULL;
			}
			//In C, when the function argument type is an array type, we don't pass the entire array.
			//Instead, we just pass a pointer pointing to the array.
			if (LLVMType->isArrayTy())
				LLVMType = LLVMType->getPointerTo();
			ArgTypes.push_back(LLVMType);
		}
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
				//If the argument is an array, just use its pointer.
				//Otherwise, create an alloca.
				if (this->_ArgList->at(Index)->_VarType->GetLLVMType(__Generator)->isArrayTy()) {
					__Generator.AddVariable(this->_ArgList->at(Index)->_Name, ArgIter);
				}
				else {
					//Create alloca
					auto Alloc = CreateEntryBlockAlloca(Func, this->_ArgList->at(Index)->_Name, ArgTypes[Index]);
					//Assign the value by "store" instruction
					IRBuilder.CreateStore(ArgIter, Alloc);
					//Add to the symbol table
					__Generator.AddVariable(this->_ArgList->at(Index)->_Name, Alloc);
				}

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

	//Subscript, e.g. a[10]
	llvm::Value* Subscript::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* Subscript::CodeGenPtr(CodeGenerator& __Generator) {
		//Get the pointer pointing to the array
		llvm::Value* ArrayPtr = this->_Array->CodeGenPtr(__Generator);
		if (!ArrayPtr) {
			std::cout << "Subscription must be used to a left-value." << std::endl;
			return NULL;
		}
		//Get the index value
		llvm::Value* Subspt = this->_Index->CodeGen(__Generator);
		if (!(Subspt->getType()->isIntegerTy())) {
			std::cout << "Subscription should be an integer." << std::endl;
			return NULL;
		}
		//If "ArrayPtr" points to an array, use CreateGEP.
		//Otherwise, use pointer addition.
		if (ArrayPtr->getType()->getNonOpaquePointerElementType()->isArrayTy()) {
			std::vector<llvm::Value*> Index;
			Index.push_back(IRBuilder.getInt32(0));
			Index.push_back(Subspt);
			return IRBuilder.CreateGEP(
				ArrayPtr->getType()->getNonOpaquePointerElementType(),
				ArrayPtr,
				Index
			);
		}
		else {
			return CreateAdd(ArrayPtr, Subspt, __Generator);
		}
	}

	//Operator sizeof() in C
	llvm::Value* SizeOf::CodeGen(CodeGenerator& __Generator) {
		if (this->_Arg1)
			return IRBuilder.getInt64(__Generator.DL->getTypeAllocSize(this->_Arg1->CodeGen(__Generator)->getType()));
		else
			return IRBuilder.getInt64(__Generator.DL->getTypeAllocSize(this->_Arg2->GetLLVMType(__Generator)));
	}
	llvm::Value* SizeOf::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Sizeof() only returns right-values.");
		return NULL;
	}

	//Function call
	llvm::Value* FunctionCall::CodeGen(CodeGenerator& __Generator) {
		//Get the function. Throw exception if the function doesn't exist.
		llvm::Function* Func = __Generator.Module->getFunction(this->_FuncName);
		if (Func == NULL) {
			throw std::domain_error(this->_FuncName + "is not a defined function.");
			return NULL;
		}
		//Check the number of args. If Func took a different number of args, reject.
		if (Func->arg_size() != this->_ArgList->size()) {
			throw std::invalid_argument("#Args doesn't match when calling function " + this->_FuncName + ". Expected " + std::to_string(Func->arg_size()) + ", got " + std::to_string(this->_ArgList->size()));
			return NULL;
		}
		//Check arg types. If Func took different different arg types, reject.
		std::vector<llvm::Value*> ArgList;
		size_t Index = 0;
		for (auto ArgIter = Func->arg_begin(); ArgIter < Func->arg_end(); ArgIter++, Index++) {
			llvm::Value* Arg = this->_ArgList->at(Index)->CodeGen(__Generator);
			Arg = TypeCasting(Arg, ArgIter->getType());
			if (Arg == NULL) {
				throw std::invalid_argument(std::to_string(Index) + "-th arg type doesn't match when calling function " + this->_FuncName + ".");
				return NULL;
			}
			ArgList.push_back(Arg);
		}
		//Create function call.
		return IRBuilder.CreateCall(Func, ArgList);
	}
	llvm::Value* FunctionCall::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Function call only returns right-values.");
		return NULL;
	}

	//Structure reference, e.g. a.x, a.y
	llvm::Value* StructReference::CodeGen(CodeGenerator& __Generator) {
		return NULL;
	}
	llvm::Value* StructReference::CodeGenPtr(CodeGenerator& __Generator) {
		return NULL;
	}

	//Structure dereference, e.g. a->x, a->y
	llvm::Value* StructDereference::CodeGen(CodeGenerator& __Generator) {
		return NULL;
	}
	llvm::Value* StructDereference::CodeGenPtr(CodeGenerator& __Generator) {
		return NULL;
	}

	//Unary plus, e.g. +i, +j, +123
	llvm::Value* UnaryPlus::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* Operand = this->_Operand->CodeGen(__Generator);
		if (!(
			Operand->getType()->isIntegerTy() ||
			Operand->getType()->isFloatingPointTy())
			)
			throw std::logic_error("Unary plus must be applied to integers or floating-point numbers.");
		return Operand;
	}
	llvm::Value* UnaryPlus::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Unary plus only returns right-values.");
		return NULL;
	}

	//Unary minus, e.g. -i, -j, -123
	llvm::Value* UnaryMinus::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* Operand = this->_Operand->CodeGen(__Generator);
		if (!(
			Operand->getType()->isIntegerTy() ||
			Operand->getType()->isFloatingPointTy())
			)
			throw std::logic_error("Unary minus must be applied to integers or floating-point numbers.");
		if (Operand->getType()->isIntegerTy())
			return IRBuilder.CreateSub(llvm::ConstantInt::get(Operand->getType(), 0, true), Operand);
		else
			return IRBuilder.CreateFSub(llvm::ConstantFP::get(Operand->getType(), 0.0), Operand);
	}
	llvm::Value* UnaryMinus::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Unary minus only returns right-values.");
		return NULL;
	}

	//Type cast, e.g. (float)n, (int)1.0
	llvm::Value* TypeCast::CodeGen(CodeGenerator& __Generator) {
		return TypeCasting(this->_Operand->CodeGen(__Generator), this->_VarType->GetLLVMType(__Generator));
	}
	llvm::Value* TypeCast::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Type casting only returns right-values.");
		return NULL;
	}

	//Prefix increment, e.g. ++i
	llvm::Value* PrefixInc::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* PrefixInc::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* Operand = this->_Operand->CodeGenPtr(__Generator);
		llvm::Value* OpValue = IRBuilder.CreateLoad(Operand->getType()->getNonOpaquePointerElementType(), Operand);
		if (!(
			OpValue->getType()->isIntegerTy() ||
			OpValue->getType()->isFloatingPointTy() ||
			OpValue->getType()->isPointerTy())
			)
			throw std::logic_error("Prefix increment must be applied to integers, floating-point numbers or pointers.");
		llvm::Value* OpValuePlus = CreateAdd(OpValue, IRBuilder.getInt1(1), __Generator);
		IRBuilder.CreateStore(OpValuePlus, Operand);
		return Operand;
	}

	//Postfix increment, e.g. i++
	llvm::Value* PostfixInc::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* Operand = this->_Operand->CodeGenPtr(__Generator);
		llvm::Value* OpValue = IRBuilder.CreateLoad(Operand->getType()->getNonOpaquePointerElementType(), Operand);
		if (!(
			OpValue->getType()->isIntegerTy() ||
			OpValue->getType()->isFloatingPointTy() ||
			OpValue->getType()->isPointerTy())
			)
			throw std::logic_error("Postfix increment must be applied to integers, floating-point numbers or pointers.");
		llvm::Value* OpValuePlus = CreateAdd(OpValue, IRBuilder.getInt1(1), __Generator);
		IRBuilder.CreateStore(OpValuePlus, Operand);
		return OpValue;
	}
	llvm::Value* PostfixInc::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Postfix increment only returns right-values.");
		return NULL;
	}

	//Prefix decrement, e.g. --i
	llvm::Value* PrefixDec::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* PrefixDec::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* Operand = this->_Operand->CodeGenPtr(__Generator);
		llvm::Value* OpValue = IRBuilder.CreateLoad(Operand->getType()->getNonOpaquePointerElementType(), Operand);
		if (!(
			OpValue->getType()->isIntegerTy() ||
			OpValue->getType()->isFloatingPointTy() ||
			OpValue->getType()->isPointerTy())
			)
			throw std::logic_error("Prefix decrement must be applied to integers, floating-point numbers or pointers.");
		llvm::Value* OpValueMinus = CreateSub(OpValue, IRBuilder.getInt1(1), __Generator);
		IRBuilder.CreateStore(OpValueMinus, Operand);
		return Operand;
	}

	//Postfix decrement, e.g. i--
	llvm::Value* PostfixDec::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* Operand = this->_Operand->CodeGenPtr(__Generator);
		llvm::Value* OpValue = IRBuilder.CreateLoad(Operand->getType()->getNonOpaquePointerElementType(), Operand);
		if (!(
			OpValue->getType()->isIntegerTy() ||
			OpValue->getType()->isFloatingPointTy() ||
			OpValue->getType()->isPointerTy())
			)
			throw std::logic_error("Postfix decrement must be applied to integers, floating-point numbers or pointers.");
		llvm::Value* OpValueMinus = CreateSub(OpValue, IRBuilder.getInt1(1), __Generator);
		IRBuilder.CreateStore(OpValueMinus, Operand);
		return OpValue;
	}
	llvm::Value* PostfixDec::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Postfix decrement only returns right-values.");
		return NULL;
	}

	//Indirection, e.g. *ptr
	llvm::Value* Indirection::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* Indirection::CodeGenPtr(CodeGenerator& __Generator) {
		return this->_Operand->CodeGenPtr(__Generator);
	}

	//Fetch address, e.g. &i
	llvm::Value* AddressOf::CodeGen(CodeGenerator& __Generator) {
		return this->CodeGenPtr(__Generator);
	}
	llvm::Value* AddressOf::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Address operator \"&\" only returns right-values.");
		return NULL;
	}

	//Logic NOT, e.g. !x
	llvm::Value* LogicNot::CodeGen(CodeGenerator& __Generator) {
		return IRBuilder.CreateNeg(Cast2I1(this->_Operand->CodeGen(__Generator)));
	}
	llvm::Value* LogicNot::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Logic NOT operator \"!\" only returns right-values.");
		return NULL;
	}

	//Bitwise NOT, e.g. ~x
	llvm::Value* BitwiseNot::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* Operand = this->_Operand->CodeGen(__Generator);
		if (!Operand->getType()->isIntegerTy()) {
			throw std::logic_error("Bitwise NOT operator \"~\" must be applied to integers.");
			return NULL;
		}
		return IRBuilder.CreateNot(Operand);
	}
	llvm::Value* BitwiseNot::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Bitwise NOT operator \"~\" only returns right-values.");
		return NULL;
	}

	//Division, e.g. x/y
	llvm::Value* Division::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		if (TypeUpgrading(LHS, RHS)) {
			if (LHS->getType()->isIntegerTy())
				return IRBuilder.CreateSDiv(LHS, RHS);
			else
				return IRBuilder.CreateFDiv(LHS, RHS);
		}
		else {
			throw std::logic_error("Division operator \"/\" must only be applied to integers or floating-point numbers.");
			return NULL;
		}
	}
	llvm::Value* Division::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Division operator \"/\" only returns right-values.");
		return NULL;
	}

	//Multiplication, e.g. x*y
	llvm::Value* Multiplication::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		if (TypeUpgrading(LHS, RHS)) {
			if (LHS->getType()->isIntegerTy())
				return IRBuilder.CreateMul(LHS, RHS);
			else
				return IRBuilder.CreateFMul(LHS, RHS);
		}
		else {
			throw std::logic_error("Multiplication operator \"*\" must only be applied to integers or floating-point numbers.");
			return NULL;
		}
	}
	llvm::Value* Multiplication::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Multiplication operator \"*\" only returns right-values.");
		return NULL;
	}

	//Modulo, e.g. x%y
	llvm::Value* Modulo::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		if (!(LHS->getType()->isIntegerTy() && RHS->getType()->isIntegerTy())) {
			throw std::domain_error("Modulo operator \"%\" must be applied to 2 integers.");
			return NULL;
		}
		TypeUpgrading(LHS, RHS);
		return IRBuilder.CreateSRem(LHS, RHS);
	}
	llvm::Value* Modulo::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Modulo operator \"%\" only returns right-values.");
		return NULL;
	}

	//Addition, e.g. x+y
	llvm::Value* Addition::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateAdd(LHS, RHS, __Generator);
	}
	llvm::Value* Addition::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Addition operator \"+\" only returns right-values.");
		return NULL;
	}

	//Subtraction, e.g. x-y
	llvm::Value* Subtraction::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateSub(LHS, RHS, __Generator);
	}
	llvm::Value* Subtraction::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Subtraction operator \"-\" only returns right-values.");
		return NULL;
	}

	//LeftShift, e.g. x<<y
	llvm::Value* LeftShift::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		if (!(LHS->getType()->isIntegerTy() && RHS->getType()->isIntegerTy())) {
			throw std::domain_error("Left shifting operator \"<<\" must be applied to 2 integers.");
			return NULL;
		}
		TypeUpgrading(LHS, RHS);
		return IRBuilder.CreateShl(LHS, RHS);
	}
	llvm::Value* LeftShift::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Left shifting operator \"<<\" only returns right-values.");
		return NULL;
	}

	//RightShift, e.g. x>>y
	llvm::Value* RightShift::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		if (!(LHS->getType()->isIntegerTy() && RHS->getType()->isIntegerTy())) {
			throw std::domain_error("Left shifting operator \"<<\" must be applied to 2 integers.");
			return NULL;
		}
		TypeUpgrading(LHS, RHS);
		return IRBuilder.CreateAShr(LHS, RHS);
	}
	llvm::Value* RightShift::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Right shifting operator \">>\" only returns right-values.");
		return NULL;
	}

	//LogicGT, e.g. x>y
	llvm::Value* LogicGT::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		//Arithmatic compare
		if (TypeUpgrading(LHS, RHS)) {
			if (LHS->getType()->isIntegerTy())
				return IRBuilder.CreateICmpSGT(LHS, RHS);
			else
				return IRBuilder.CreateFCmpOGT(LHS, RHS);
		}
		//Pointer compare
		if (LHS->getType()->isPointerTy() && LHS->getType() == RHS->getType()) {
			return IRBuilder.CreateICmpSGT(
				IRBuilder.CreatePtrDiff(LHS->getType()->getNonOpaquePointerElementType(), LHS, RHS),
				IRBuilder.getInt64(0)
			);
		}
		throw std::domain_error("Comparison \">\" using unsupported type combination.");
	}
	llvm::Value* LogicGT::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Comparison operator \">\" only returns right-values.");
		return NULL;
	}

	//LogicGE, e.g. x>=y
	llvm::Value* LogicGE::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		//Arithmatic compare
		if (TypeUpgrading(LHS, RHS)) {
			if (LHS->getType()->isIntegerTy())
				return IRBuilder.CreateICmpSGE(LHS, RHS);
			else
				return IRBuilder.CreateFCmpOGE(LHS, RHS);
		}
		//Pointer compare
		if (LHS->getType()->isPointerTy() && LHS->getType() == RHS->getType()) {
			return IRBuilder.CreateICmpSGE(
				IRBuilder.CreatePtrDiff(LHS->getType()->getNonOpaquePointerElementType(), LHS, RHS),
				IRBuilder.getInt64(0)
			);
		}
		throw std::domain_error("Comparison \">=\" using unsupported type combination.");
	}
	llvm::Value* LogicGE::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Comparison operator \">=\" only returns right-values.");
		return NULL;
	}

	//LogicLT, e.g. x<y
	llvm::Value* LogicLT::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		//Arithmatic compare
		if (TypeUpgrading(LHS, RHS)) {
			if (LHS->getType()->isIntegerTy())
				return IRBuilder.CreateICmpSLT(LHS, RHS);
			else
				return IRBuilder.CreateFCmpOLT(LHS, RHS);
		}
		//Pointer compare
		if (LHS->getType()->isPointerTy() && LHS->getType() == RHS->getType()) {
			return IRBuilder.CreateICmpSLT(
				IRBuilder.CreatePtrDiff(LHS->getType()->getNonOpaquePointerElementType(), LHS, RHS),
				IRBuilder.getInt64(0)
			);
		}
		throw std::domain_error("Comparison \"<\" using unsupported type combination.");
	}
	llvm::Value* LogicLT::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Comparison operator \"<\" only returns right-values.");
		return NULL;
	}

	//LogicLE, e.g. x<=y
	llvm::Value* LogicLE::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		//Arithmatic compare
		if (TypeUpgrading(LHS, RHS)) {
			if (LHS->getType()->isIntegerTy())
				return IRBuilder.CreateICmpSLE(LHS, RHS);
			else
				return IRBuilder.CreateFCmpOLE(LHS, RHS);
		}
		//Pointer compare
		if (LHS->getType()->isPointerTy() && LHS->getType() == RHS->getType()) {
			return IRBuilder.CreateICmpSLE(
				IRBuilder.CreatePtrDiff(LHS->getType()->getNonOpaquePointerElementType(), LHS, RHS),
				IRBuilder.getInt64(0)
			);
		}
		throw std::domain_error("Comparison \"<=\" using unsupported type combination.");
	}
	llvm::Value* LogicLE::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Comparison operator \"<=\" only returns right-values.");
		return NULL;
	}

	//LogicEQ, e.g. x==y
	llvm::Value* LogicEQ::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateCmpEQ(LHS, RHS);
	}
	llvm::Value* LogicEQ::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Comparison operator \"==\" only returns right-values.");
		return NULL;
	}

	//LogicNEQ, e.g. x!=y
	llvm::Value* LogicNEQ::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		//Arithmatic compare
		if (TypeUpgrading(LHS, RHS)) {
			if (LHS->getType()->isIntegerTy())
				return IRBuilder.CreateICmpNE(LHS, RHS);
			else
				return IRBuilder.CreateFCmpONE(LHS, RHS);
		}
		//Pointer compare
		if (LHS->getType()->isPointerTy() && LHS->getType() == RHS->getType()) {
			return IRBuilder.CreateICmpNE(
				IRBuilder.CreatePtrDiff(LHS->getType()->getNonOpaquePointerElementType(), LHS, RHS),
				IRBuilder.getInt64(0)
			);
		}
		throw std::domain_error("Comparison \"!=\" using unsupported type combination.");
	}
	llvm::Value* LogicNEQ::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Comparison operator \"!=\" only returns right-values.");
		return NULL;
	}

	//BitwiseAND, e.g. x&y
	llvm::Value* BitwiseAND::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		if (!(LHS->getType()->isIntegerTy() && RHS->getType()->isIntegerTy())) {
			throw std::domain_error("Bitwise AND operator \"&\" must be applied to 2 integers.");
			return NULL;
		}
		TypeUpgrading(LHS, RHS);
		return IRBuilder.CreateAnd(LHS, RHS);
	}
	llvm::Value* BitwiseAND::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Bitwise AND operator \"&\" only returns right-values.");
		return NULL;
	}

	//BitwiseXOR, e.g. x^y
	llvm::Value* BitwiseXOR::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		if (!(LHS->getType()->isIntegerTy() && RHS->getType()->isIntegerTy())) {
			throw std::domain_error("Bitwise XOR operator \"^\" must be applied to 2 integers.");
			return NULL;
		}
		TypeUpgrading(LHS, RHS);
		return IRBuilder.CreateXor(LHS, RHS);
	}
	llvm::Value* BitwiseXOR::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Bitwise XOR operator \"^\" only returns right-values.");
		return NULL;
	}

	//BitwiseOR, e.g. x|y
	llvm::Value* BitwiseOR::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		if (!(LHS->getType()->isIntegerTy() && RHS->getType()->isIntegerTy())) {
			throw std::domain_error("Bitwise OR operator \"|\" must be applied to 2 integers.");
			return NULL;
		}
		TypeUpgrading(LHS, RHS);
		return IRBuilder.CreateOr(LHS, RHS);
	}
	llvm::Value* BitwiseOR::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Bitwise OR operator \"|\" only returns right-values.");
		return NULL;
	}

	//LogicAND, e.g. x&&y
	llvm::Value* LogicAND::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		LHS = Cast2I1(LHS);
		if (LHS == NULL) {
			throw std::domain_error("Logic AND operator \"&&\" must be applied to 2 expressions that can be cast to boolean.");
			return NULL;
		}
		RHS = Cast2I1(RHS);
		if (RHS == NULL) {
			throw std::domain_error("Logic AND operator \"&&\" must be applied to 2 expressions that can be cast to boolean.");
			return NULL;
		}
		return IRBuilder.CreateLogicalAnd(LHS, RHS);
	}
	llvm::Value* LogicAND::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Logic AND operator \"&&\" only returns right-values.");
		return NULL;
	}

	//LogicOR, e.g. x||y
	llvm::Value* LogicOR::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		LHS = Cast2I1(LHS);
		if (LHS == NULL) {
			throw std::domain_error("Logic OR operator \"||\" must be applied to 2 expressions that can be cast to boolean.");
			return NULL;
		}
		RHS = Cast2I1(RHS);
		if (RHS == NULL) {
			throw std::domain_error("Logic OR operator \"||\" must be applied to 2 expressions that can be cast to boolean.");
			return NULL;
		}
		return IRBuilder.CreateLogicalOr(LHS, RHS);
	}
	llvm::Value* LogicOR::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Logic OR operator \"||\" only returns right-values.");
		return NULL;
	}

	//TernaryCondition, e.g. (cond)?x:y
	llvm::Value* TernaryCondition::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* Condition = Cast2I1(this->_Condition->CodeGen(__Generator));
		if (Condition == NULL) {
			throw std::logic_error("The first operand of thernary operand \" ? : \" must be able to be cast to boolean.");
			return NULL;
		}
		llvm::Value* True = this->_Then->CodeGen(__Generator);
		llvm::Value* False = this->_Else->CodeGen(__Generator);
		//Arithmatic (c)*x+(!c)*y
		if (TypeUpgrading(True, False)) {
			llvm::Value* One = TypeCasting(Condition, True->getType());
			llvm::Value* Zero = TypeCasting(IRBuilder.CreateNeg(Condition), True->getType());
			if (True->getType()->isIntegerTy()) {
				return IRBuilder.CreateAdd(
					IRBuilder.CreateMul(One, True),
					IRBuilder.CreateMul(Zero, False)
				);
			}
			else {
				return IRBuilder.CreateFAdd(
					IRBuilder.CreateFMul(One, True),
					IRBuilder.CreateFMul(Zero, False)
				);
			}
		}
		//Pointer (c)*p1+(!c)*p2
		else if (True->getType()->isPointerTy() && True->getType() == False->getType()) {
			llvm::Value* One = TypeCasting(Condition, IRBuilder.getInt64Ty());
			llvm::Value* Zero = TypeCasting(IRBuilder.CreateNeg(Condition), IRBuilder.getInt64Ty());
			return IRBuilder.CreateIntToPtr(
				IRBuilder.CreateAdd(
					IRBuilder.CreateMul(One, IRBuilder.CreatePtrToInt(True, IRBuilder.getInt64Ty())),
					IRBuilder.CreateMul(Zero, IRBuilder.CreatePtrToInt(False, IRBuilder.getInt64Ty()))
				), True->getType());
		}
		throw std::domain_error("Thernary operand \" ? : \" using unsupported type combination.");
		return NULL;
	}
	llvm::Value* TernaryCondition::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* Condition = Cast2I1(this->_Condition->CodeGen(__Generator));
		if (Condition == NULL) {
			throw std::logic_error("The first operand of thernary operand \" ? : \" must be able to be cast to boolean.");
			return NULL;
		}
		llvm::Value* True = this->_Then->CodeGenPtr(__Generator);
		llvm::Value* False = this->_Else->CodeGenPtr(__Generator);
		if (True->getType() == False->getType()) {
			llvm::Value* One = TypeCasting(Condition, IRBuilder.getInt64Ty());
			llvm::Value* Zero = TypeCasting(IRBuilder.CreateNeg(Condition), IRBuilder.getInt64Ty());
			return IRBuilder.CreateIntToPtr(
				IRBuilder.CreateAdd(
					IRBuilder.CreateMul(One, IRBuilder.CreatePtrToInt(True, IRBuilder.getInt64Ty())),
					IRBuilder.CreateMul(Zero, IRBuilder.CreatePtrToInt(False, IRBuilder.getInt64Ty()))
				), True->getType());
		}
		throw std::domain_error("When using thernary expressions \" ? : \" as left-values, the latter two operands must be of the same type.");
		return NULL;
	}

	//DirectAssign, e.g. x=y
	llvm::Value* DirectAssign::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* DirectAssign::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGenPtr(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		RHS = TypeCasting(RHS, LHS->getType()->getNonOpaquePointerElementType());
		if (RHS == NULL) {
			throw std::domain_error("Assignment with values that cannot be cast to the target type.");
			return NULL;
		}
		IRBuilder.CreateStore(RHS, LHS);
		return LHS;
	}

	//DivAssign, e.g. x/=y
	llvm::Value* DivAssign::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* DivAssign::CodeGenPtr(CodeGenerator& __Generator) {
		Division Div(this->_LHS, this->_RHS);
		DirectAssign Ass(this->_LHS, &Div);
		return Ass.CodeGenPtr(__Generator);
	}

	//MulAssign, e.g. x*=y
	llvm::Value* MulAssign::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* MulAssign::CodeGenPtr(CodeGenerator& __Generator) {
		Multiplication Mul(this->_LHS, this->_RHS);
		DirectAssign Ass(this->_LHS, &Mul);
		return Ass.CodeGenPtr(__Generator);
	}

	//ModAssign, e.g. x%=y
	llvm::Value* ModAssign::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* ModAssign::CodeGenPtr(CodeGenerator& __Generator) {
		Modulo Mod(this->_LHS, this->_RHS);
		DirectAssign Ass(this->_LHS, &Mod);
		return Ass.CodeGenPtr(__Generator);
	}

	//AddAssign, e.g. x+=y
	llvm::Value* AddAssign::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* AddAssign::CodeGenPtr(CodeGenerator& __Generator) {
		Addition Add(this->_LHS, this->_RHS);
		DirectAssign Ass(this->_LHS, &Add);
		return Ass.CodeGenPtr(__Generator);
	}

	//SubAssign, e.g. x-=y
	llvm::Value* SubAssign::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* SubAssign::CodeGenPtr(CodeGenerator& __Generator) {
		Subtraction Sub(this->_LHS, this->_RHS);
		DirectAssign Ass(this->_LHS, &Sub);
		return Ass.CodeGenPtr(__Generator);
	}

	//SHLAssign, e.g. x<<=y
	llvm::Value* SHLAssign::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* SHLAssign::CodeGenPtr(CodeGenerator& __Generator) {
		LeftShift SHL(this->_LHS, this->_RHS);
		DirectAssign Ass(this->_LHS, &SHL);
		return Ass.CodeGenPtr(__Generator);
	}

	//SHRAssign, e.g. x>>=y
	llvm::Value* SHRAssign::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* SHRAssign::CodeGenPtr(CodeGenerator& __Generator) {
		RightShift SHR(this->_LHS, this->_RHS);
		DirectAssign Ass(this->_LHS, &SHR);
		return Ass.CodeGenPtr(__Generator);
	}

	//BitwiseANDAssign, e.g. x&=y
	llvm::Value* BitwiseANDAssign::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* BitwiseANDAssign::CodeGenPtr(CodeGenerator& __Generator) {
		BitwiseAND BAND(this->_LHS, this->_RHS);
		DirectAssign Ass(this->_LHS, &BAND);
		return Ass.CodeGenPtr(__Generator);
	}

	//BitwiseXORAssign, e.g. x^=y
	llvm::Value* BitwiseXORAssign::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* BitwiseXORAssign::CodeGenPtr(CodeGenerator& __Generator) {
		BitwiseXOR BXOR(this->_LHS, this->_RHS);
		DirectAssign Ass(this->_LHS, &BXOR);
		return Ass.CodeGenPtr(__Generator);
	}

	//BitwiseORAssign, e.g. x|=y
	llvm::Value* BitwiseORAssign::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* BitwiseORAssign::CodeGenPtr(CodeGenerator& __Generator) {
		BitwiseOR BOR(this->_LHS, this->_RHS);
		DirectAssign Ass(this->_LHS, &BOR);
		return Ass.CodeGenPtr(__Generator);
	}

	//CommaExpr, e.g. x,y,z,w
	llvm::Value* CommaExpr::CodeGen(CodeGenerator& __Generator) {
		this->_LHS->CodeGen(__Generator);
		return this->_RHS->CodeGen(__Generator);
	}
	llvm::Value* CommaExpr::CodeGenPtr(CodeGenerator& __Generator) {
		this->_LHS->CodeGen(__Generator);
		return this->_RHS->CodeGenPtr(__Generator);
	}

	//Variable, e.g. x
	llvm::Value* Variable::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LValue = this->CodeGenPtr(__Generator);
		//For array types, just return its pointer directly
		if (LValue->getType()->getNonOpaquePointerElementType()->isArrayTy())
			return LValue;
		else
			return IRBuilder.CreateLoad(LValue->getType()->getNonOpaquePointerElementType(), LValue);
	}
	llvm::Value* Variable::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* VarPtr = __Generator.FindVariable(this->_Name);
		if (VarPtr == NULL) {
			throw std::logic_error("Variable " + this->_Name + " is not defined in this scope.");
			return NULL;
		}
		return VarPtr;
	}

	//Constant, e.g. 1, 1.0, 'c', true, false
	llvm::Value* Constant::CodeGen(CodeGenerator& __Generator) {
		switch (this->_Type) {
		case BuiltInType::TypeID::_Bool:
			return IRBuilder.getInt1(this->_Bool);
		case BuiltInType::TypeID::_Char:
			return IRBuilder.getInt8(this->_Character);
		case BuiltInType::TypeID::_Int:
			return IRBuilder.getInt32(this->_Integer);
		case BuiltInType::TypeID::_Double:
			return llvm::ConstantFP::get(IRBuilder.getDoubleTy(), this->_Real);
		}
	}
	llvm::Value* Constant::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Constant is a right-value.");
		return NULL;
	}
}