/*
 ********************* AST.cpp **********************
 * This file implements the methods of the node		*
 * classes in the AST.								*
 * All classes are defined in the AST namespace.	*
 ****************************************************
 */

#include "CodeGenerator.hpp"
#include "Utils.hpp"

 //Namespace containing all classes involved in the construction of Abstract Syntax Tree (AST)
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
		bool ContainVoidTy = false;
		for (auto ArgType : *(this->_ArgList)) {
			llvm::Type* LLVMType = ArgType->_VarType->GetLLVMType(__Generator);
			if (!LLVMType) {
				throw std::logic_error("Defining a function " + this->_Name + " using unknown type(s).");
				return NULL;
			}
			//Check if it is a "void" type
			if (LLVMType->isVoidTy())
				ContainVoidTy = true;
			//In C, when the function argument type is an array type, we don't pass the entire array.
			//Instead, we just pass a pointer pointing to its elements
			if (LLVMType->isArrayTy())
				LLVMType = LLVMType->getArrayElementType()->getPointerTo();
			ArgTypes.push_back(LLVMType);
		}
		//Throw an exception if #args >= 2 and the function has a "void" argument.
		if (ArgTypes.size() >= 2 && ContainVoidTy) {
			throw std::logic_error("Function " + this->_Name + " has invalid number of arguments with type \"void\".");
			return NULL;
		}
		//Clear the arg list of the function only has one "void" arg.
		if (ArgTypes.size() == 1 && ContainVoidTy)
			ArgTypes.clear();
		//Get return type
		llvm::Type* RetTy = this->_RetType->GetLLVMType(__Generator);
		if (RetTy->isArrayTy()) {
			throw std::logic_error("Defining Function " + this->_Name + " whose return type is array type.");
			return NULL;
		}
		//Get function type
		llvm::FunctionType* FuncType = llvm::FunctionType::get(RetTy, ArgTypes, this->_ArgList->_VarArg);
		//Create function
		llvm::Function* Func = llvm::Function::Create(FuncType, llvm::GlobalValue::ExternalLinkage, this->_Name, __Generator.Module);
		__Generator.AddFunction(this->_Name, Func);
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
				throw std::logic_error("Redefining function " + this->_Name);
				return NULL;
			}
			if (Func->getFunctionType() != FuncType) {
				throw std::logic_error("Redefining function " + this->_Name + " with different arg types.");
				return NULL;
			}
		}
		//If this function has a body, generate its body's code.
		if (this->_FuncBody) {
			//Create a new basic block to start insertion into.
			llvm::BasicBlock* FuncBlock = llvm::BasicBlock::Create(Context, "entry", Func);
			IRBuilder.SetInsertPoint(FuncBlock);
			//Create allocated space for arguments.
			__Generator.PushSymbolTable();	//This variable table is only used to store the arguments of the function
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
			__Generator.EnterFunction(Func);
			__Generator.PushSymbolTable();
			this->_FuncBody->CodeGen(__Generator);
			__Generator.PopSymbolTable();
			__Generator.LeaveFunction();
			__Generator.PopSymbolTable();	//We need to pop out an extra variable table.
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
			else
				Stmt->CodeGen(__Generator);
		//If the function doesn't have a "return" at the end of its body, create a default one.
		if (!IRBuilder.GetInsertBlock()->getTerminator()) {
			llvm::Type* RetTy = __Generator.GetCurrentFunction()->getReturnType();
			if (RetTy->isVoidTy())
				IRBuilder.CreateRetVoid();
			else
				IRBuilder.CreateRet(llvm::UndefValue::get(RetTy));
		}
		return NULL;
	}

	//Variable declaration
	llvm::Value* VarDecl::CodeGen(CodeGenerator& __Generator) {
		//Get the llvm type
		llvm::Type* VarType = this->_VarType->GetLLVMType(__Generator);
		if (VarType == NULL) {
			throw std::logic_error("Defining variables with unknown type.");
			return NULL;
		}
		if (VarType->isVoidTy()) {
			throw std::logic_error("Cannot define \"void\" variables.");
			return NULL;
		}
		//Create variables one by one.
		for (auto& NewVar : *(this->_VarList)) {
			//Determine whether the declaration is inside a function.
			//If so, create an alloca;
			//Otherwise, create a global variable.
			if (__Generator.GetCurrentFunction()) {
				//Create an alloca.
				auto Alloc = CreateEntryBlockAlloca(__Generator.GetCurrentFunction(), NewVar->_Name, VarType);
				if (!__Generator.AddVariable(NewVar->_Name, Alloc)) {
					throw std::logic_error("Redefining local variable " + NewVar->_Name + ".");
					Alloc->eraseFromParent();
					return NULL;
				}
				//Assign the initial value by "store" instruction.
				if (NewVar->_InitialExpr) {
					llvm::Value* Initializer = TypeCasting(NewVar->_InitialExpr->CodeGen(__Generator), VarType);
					if (Initializer == NULL) {
						throw std::logic_error("Initializing variable " + NewVar->_Name + " with value of different type.");
						return NULL;
					}
					IRBuilder.CreateStore(Initializer, Alloc);
				}
				//TODO: llvm::AllocaInst doesn't has the "constant" attribute, so we need to implement it manually.
				//Unfortunately, I haven't worked out a graceful solution, and the only way I can do is to add a "const"
				//label to the corresponding entry in the symbol table.
			}
			else {
				//create a global variable.
				//Create the constant initializer
				llvm::Constant* Initializer = NULL;
				if (NewVar->_InitialExpr) {
					//Global variable must be initialized (if any) by a constant.
					__Generator.XchgInsertPointWithTmpBB();
					auto TmpBBSize = IRBuilder.GetInsertBlock()->size();
					llvm::Value* InitialExpr = TypeCasting(NewVar->_InitialExpr->CodeGen(__Generator), VarType);
					if (IRBuilder.GetInsertBlock()->size() != TmpBBSize) {
						throw std::logic_error("Initializing global variable " + NewVar->_Name + " with non-constant value.");
						return NULL;
					}
					if (InitialExpr == NULL) {
						throw std::logic_error("Initializing variable " + NewVar->_Name + " with value of different type.");
						return NULL;
					}
					__Generator.XchgInsertPointWithTmpBB();
					Initializer = (llvm::Constant*)InitialExpr;
				}
				else {
					//We must create an undef value manually. If no initializer is given,
					//this global value will be recognized as "extern" by llvm.
					Initializer = llvm::UndefValue::get(VarType);
				}
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
					throw std::logic_error("Redefining global variable " + NewVar->_Name + ".");
					Alloc->eraseFromParent();
					return NULL;
				}
			}
		}
		return NULL;
	}

	//Type declaration
	llvm::Value* TypeDecl::CodeGen(CodeGenerator& __Generator) {
		//Add an item to the current typedef symbol table
		//If an old value exists (i.e., conflict), raise an error
		llvm::Type* LLVMType;
		//For struct or union types, firstly we just need to get an opaque struct type
		if (this->_VarType->isStructType())
			LLVMType = ((AST::StructType*)this->_VarType)->GenerateLLVMTypeHead(__Generator, this->_Alias);
		else if (this->_VarType->isUnionType())
			LLVMType = ((AST::UnionType*)this->_VarType)->GenerateLLVMTypeHead(__Generator, this->_Alias);
		else
			LLVMType = this->_VarType->GetLLVMType(__Generator);
		if (!LLVMType) {
			throw std::logic_error("Typedef " + this->_Alias + " using undefined types.");
			return NULL;
		}
		if (!__Generator.AddType(this->_Alias, LLVMType))
			throw std::logic_error("Redefinition of typename " + this->_Alias);
		//For struct or union types, we need to generate its body
		if (this->_VarType->isStructType())
			((AST::StructType*)this->_VarType)->GenerateLLVMTypeBody(__Generator);
		else if (this->_VarType->isUnionType())
			((AST::UnionType*)this->_VarType)->GenerateLLVMTypeBody(__Generator);
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
		this->_LLVMType = __Generator.FindType(this->_Name);
		if (this->_LLVMType == NULL) {
			throw std::logic_error("\"" + this->_Name + "\" is an undefined type.");
			return NULL;
		}
		else return this->_LLVMType;
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
		if (BaseType->isVoidTy()) {
			throw std::logic_error("The base type of array cannot be void.");
			return NULL;
		}
		return this->_LLVMType = llvm::ArrayType::get(BaseType, this->_Length);
	}

	//Struct type.
	llvm::Type* StructType::GetLLVMType(CodeGenerator& __Generator) {
		if (this->_LLVMType)
			return this->_LLVMType;
		//Create an anonymous identified struct type
		this->GenerateLLVMTypeHead(__Generator);
		return this->GenerateLLVMTypeBody(__Generator);
	}
	llvm::Type* StructType::GenerateLLVMTypeHead(CodeGenerator& __Generator, const std::string& __Name) {
		//Firstly, generate an empty identified struct type
		auto LLVMType = llvm::StructType::create(Context, "struct." + __Name);
		//Add to the struct table
		__Generator.AddStructType(LLVMType, this);
		return this->_LLVMType = LLVMType;
	}
	llvm::Type* StructType::GenerateLLVMTypeBody(CodeGenerator& __Generator) {
		//Secondly, generate its body
		std::vector<llvm::Type*> Members;
		for (auto FDecl : *(this->_StructBody))
			if (FDecl->_VarType->GetLLVMType(__Generator)->isVoidTy()) {
				throw std::logic_error("The member type of struct cannot be void.");
				return NULL;
			}
			else
				Members.insert(Members.end(), FDecl->_MemList->size(), FDecl->_VarType->GetLLVMType(__Generator));
		((llvm::StructType*)this->_LLVMType)->setBody(Members);
		return this->_LLVMType;
	}
	size_t StructType::GetElementIndex(const std::string& __MemName) {
		size_t Index = 0;
		for (auto FDecl : *(this->_StructBody))
			for (auto& MemName : *(FDecl->_MemList))
				if (MemName == __MemName)
					return Index;
				else Index++;
		return -1;
	}

	//Union type.
	llvm::Type* UnionType::GetLLVMType(CodeGenerator& __Generator) {
		if (this->_LLVMType)
			return this->_LLVMType;
		//Create an anonymous identified struct type
		this->GenerateLLVMTypeHead(__Generator);
		return this->GenerateLLVMTypeBody(__Generator);
	}
	llvm::Type* UnionType::GenerateLLVMTypeHead(CodeGenerator& __Generator, const std::string& __Name) {
		//Firstly, generate an empty identified struct type
		auto LLVMType = llvm::StructType::create(Context, "union." + __Name);
		//Add to the union table
		__Generator.AddUnionType(LLVMType, this);
		return this->_LLVMType = LLVMType;
	}
	llvm::Type* UnionType::GenerateLLVMTypeBody(CodeGenerator& __Generator) {
		//Secondly, generate its body
		if (this->_UnionBody->size() == 0) return this->_LLVMType;
		//Find the member of the max size
		size_t MaxSize = 0;
		llvm::Type* MaxSizeType = NULL;
		for (auto FDecl : *(this->_UnionBody))
			if (FDecl->_VarType->GetLLVMType(__Generator)->isVoidTy()) {
				throw std::logic_error("The member type of union cannot be void.");
				return NULL;
			}
			else if (__Generator.GetTypeSize(FDecl->_VarType->GetLLVMType(__Generator)) > MaxSize) {
				MaxSizeType = FDecl->_VarType->GetLLVMType(__Generator);
				MaxSize = __Generator.GetTypeSize(MaxSizeType);
			}
		((llvm::StructType*)this->_LLVMType)->setBody(std::vector<llvm::Type*>{MaxSizeType});
		return this->_LLVMType;
	}
	llvm::Type* UnionType::GetElementType(const std::string& __MemName, CodeGenerator& __Generator) {
		for (auto FDecl : *(this->_UnionBody))
			for (auto& MemName : *(FDecl->_MemList))
				if (MemName == __MemName)
					return FDecl->_VarType->GetLLVMType(__Generator);
		return NULL;
	}

	//Enum type
	llvm::Type* EnumType::GetLLVMType(CodeGenerator& __Generator) {
		if (this->_LLVMType)
			return this->_LLVMType;
		//Generate the body of the enum type
		int LastVal = -1;
		for (auto Mem : *(this->_EnmList))
			if (Mem->_hasValue) {
				LastVal = Mem->_Value;
			}
			else {
				Mem->_Value = ++LastVal;
			}
		//Add constants to the symbol table
		for (auto Mem : *(this->_EnmList))
			if (!__Generator.AddConstant(Mem->_Name, IRBuilder.getInt32(Mem->_Value))) {
				throw std::logic_error("Redefining symbol \"" + Mem->_Name + "\".");
				return NULL;
			}
		//Enum type is actually an int32 type.
		return llvm::IntegerType::getInt32Ty(Context);
	}

	//Statement block
	llvm::Value* Block::CodeGen(CodeGenerator& __Generator) {
		__Generator.PushSymbolTable();
		//Generate the statements in Block, one by one.
		for (auto& Stmt : *(this->_Content))
			//If the current block already has a terminator,
			//i.e. a "break" statement is generated, stop;
			//Otherwise, continue generating.
			if (IRBuilder.GetInsertBlock()->getTerminator())
				break;
			else if (Stmt)	//We allow empty-statement which is represented by NULL pointer.
				Stmt->CodeGen(__Generator);
		__Generator.PopSymbolTable();
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
			throw std::logic_error("The condition value of if-statement must be either an integer, or a floating-point number, or a pointer.");
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
			__Generator.PushSymbolTable();
			this->_Then->CodeGen(__Generator);
			__Generator.PopSymbolTable();
		}
		TerminateBlockByBr(MergeBB);
		//Generate code in the "Else" block
		CurrentFunc->getBasicBlockList().push_back(ElseBB);
		IRBuilder.SetInsertPoint(ElseBB);
		if (this->_Else) {
			__Generator.PushSymbolTable();
			this->_Else->CodeGen(__Generator);
			__Generator.PopSymbolTable();
		}
		TerminateBlockByBr(MergeBB);
		//Finish "Merge" block
		if (MergeBB->hasNPredecessorsOrMore(1)) {
			CurrentFunc->getBasicBlockList().push_back(MergeBB);
			IRBuilder.SetInsertPoint(MergeBB);
		}
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
			throw std::logic_error("The condition value of while-statement must be either an integer, or a floating-point number, or a pointer.");
			return NULL;
		}
		IRBuilder.CreateCondBr(Condition, WhileLoopBB, WhileEndBB);
		//Generate code in the "WhileLoop" block
		CurrentFunc->getBasicBlockList().push_back(WhileLoopBB);
		IRBuilder.SetInsertPoint(WhileLoopBB);
		if (this->_LoopBody) {
			__Generator.EnterLoop(WhileCondBB, WhileEndBB);	//Don't forget to call "EnterLoop"
			__Generator.PushSymbolTable();
			this->_LoopBody->CodeGen(__Generator);
			__Generator.PopSymbolTable();
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
			__Generator.PushSymbolTable();
			this->_LoopBody->CodeGen(__Generator);
			__Generator.PopSymbolTable();
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
			throw std::logic_error("The condition value of do-statement must be either an integer, or a floating-point number, or a pointer.");
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
			__Generator.PushSymbolTable();
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
				throw std::logic_error("The condition value of for-statement must be either an integer, or a floating-point number, or a pointer.");
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
			__Generator.PushSymbolTable();
			this->_LoopBody->CodeGen(__Generator);
			__Generator.PopSymbolTable();
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
			__Generator.PopSymbolTable();
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
		__Generator.PushSymbolTable();
		for (int i = 0; i < this->_CaseList->size(); i++) {
			CurrentFunc->getBasicBlockList().push_back(CaseBB[i]);
			IRBuilder.SetInsertPoint(CaseBB[i]);
			__Generator.EnterLoop(CaseBB[i + 1], CaseBB.back());
			this->_CaseList->at(i)->CodeGen(__Generator);
			__Generator.LeaveLoop();
		}
		__Generator.PopSymbolTable();
		//Finish "SwitchEnd" block
		if (CaseBB.back()->hasNPredecessorsOrMore(1)) {
			CurrentFunc->getBasicBlockList().push_back(CaseBB.back());
			IRBuilder.SetInsertPoint(CaseBB.back());
		}
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
			throw std::logic_error("Continue statement should only be used in loops or switch statements.");
		return NULL;
	}

	//Break statement
	llvm::Value* BreakStmt::CodeGen(CodeGenerator& __Generator) {
		llvm::BasicBlock* BreakTarget = __Generator.GetBreakBlock();
		if (BreakTarget)
			IRBuilder.CreateBr(BreakTarget);
		else
			throw std::logic_error("Break statement should only be used in loops or switch statements.");
		return NULL;
	}

	//Return statement
	llvm::Value* ReturnStmt::CodeGen(CodeGenerator& __Generator) {
		llvm::Function* Func = __Generator.GetCurrentFunction();
		if (!Func) {
			throw std::logic_error("Return statement should only be used in function bodies.");
			return NULL;
		}
		if (this->_RetVal == NULL) {
			if (Func->getReturnType()->isVoidTy())
				IRBuilder.CreateRetVoid();
			else {
				throw std::logic_error("Expected an expression after return statement.");
				return NULL;
			}
		}
		else {
			llvm::Value* RetVal = TypeCasting(this->_RetVal->CodeGen(__Generator), Func->getReturnType());
			if (!RetVal) {
				throw std::logic_error("The type of return value doesn't match and cannot be cast to the return type.");
				return NULL;
			}
			IRBuilder.CreateRet(RetVal);
		}
	}

	//Subscript, e.g. a[10], b[2][3]
	llvm::Value* Subscript::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* Subscript::CodeGenPtr(CodeGenerator& __Generator) {
		//Get the pointer
		llvm::Value* ArrayPtr = this->_Array->CodeGen(__Generator);
		if (!ArrayPtr->getType()->isPointerTy()) {
			throw std::logic_error("Subscript operator \"[]\" must be applied to pointers or arrays.");
			return NULL;
		}
		//Get the index value
		llvm::Value* Subspt = this->_Index->CodeGen(__Generator);
		if (!(Subspt->getType()->isIntegerTy())) {
			throw std::logic_error("Subscription should be an integer.");
			return NULL;
		}
		//Return pointer addition
		return CreateAdd(ArrayPtr, Subspt, __Generator);
	}

	//Operator sizeof() in C
	llvm::Value* SizeOf::CodeGen(CodeGenerator& __Generator) {
		if (this->_Arg1)//Expression
			return IRBuilder.getInt64(__Generator.GetTypeSize(this->_Arg1->CodeGen(__Generator)->getType()));
		else if (this->_Arg2)//VarType
			return IRBuilder.getInt64(__Generator.GetTypeSize(this->_Arg2->GetLLVMType(__Generator)));
		else {//Single identifier
			llvm::Type* Type = __Generator.FindType(this->_Arg3);
			if (Type) {
				this->_Arg2 = new DefinedType(this->_Arg3);
				return IRBuilder.getInt64(__Generator.GetTypeSize(Type));
			}
			llvm::Value* Var = __Generator.FindVariable(this->_Arg3);
			if (Var) {
				this->_Arg1 = new Variable(this->_Arg3);
				return IRBuilder.getInt64(__Generator.GetTypeSize(Var->getType()->getNonOpaquePointerElementType()));
			}
			throw std::logic_error(this->_Arg3 + " is neither a type nor a variable.");
			return NULL;
		}
	}
	llvm::Value* SizeOf::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Sizeof() only returns right-values.");
		return NULL;
	}

	//Function call
	llvm::Value* FunctionCall::CodeGen(CodeGenerator& __Generator) {
		//Get the function. Throw exception if the function doesn't exist.
		llvm::Function* Func = __Generator.FindFunction(this->_FuncName);
		if (Func == NULL) {
			throw std::domain_error(this->_FuncName + " is not a defined function.");
			return NULL;
		}
		//Check the number of args. If Func took a different number of args, reject.
		if (Func->isVarArg() && this->_ArgList->size() < Func->arg_size() ||
			!Func->isVarArg() && this->_ArgList->size() != Func->arg_size()) {
			throw std::invalid_argument("Args doesn't match when calling function " + this->_FuncName + ". Expected " + std::to_string(Func->arg_size()) + ", got " + std::to_string(this->_ArgList->size()));
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
		//Continue to push arguments if this function takes a variable number of arguments
		//According to the C standard, bool/char/short should be extended to int, and float should be extended to double
		if (Func->isVarArg())
			for (; Index < this->_ArgList->size(); Index++) {
				llvm::Value* Arg = this->_ArgList->at(Index)->CodeGen(__Generator);
				if (Arg->getType()->isIntegerTy())
					Arg = TypeUpgrading(Arg, IRBuilder.getInt32Ty());
				else if (Arg->getType()->isFloatingPointTy())
					Arg = TypeUpgrading(Arg, IRBuilder.getDoubleTy());
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
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* StructReference::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* StructPtr = this->_Struct->CodeGenPtr(__Generator);
		if (!StructPtr->getType()->isPointerTy() || !StructPtr->getType()->getNonOpaquePointerElementType()->isStructTy()) {
			throw std::logic_error("Reference operator \".\" must be apply to structs or unions.");
			return NULL;
		}
		//Since C language uses name instead of index to fetch the element inside a struct,
		//we need to fetch the AST::StructType* instance according to the llvm::StructType* instance.
		//And it is the same with union types.
		AST::StructType* StructType = __Generator.FindStructType((llvm::StructType*)StructPtr->getType()->getNonOpaquePointerElementType());
		if (StructType) {
			int MemIndex = StructType->GetElementIndex(this->_MemName);
			if (MemIndex == -1) {
				throw std::logic_error("The struct doesn't have a member whose name is \"" + this->_MemName + "\".");
				return NULL;
			}
			std::vector<llvm::Value*> Indices;
			Indices.push_back(IRBuilder.getInt32(0));
			Indices.push_back(IRBuilder.getInt32(MemIndex));
			return IRBuilder.CreateGEP(StructPtr->getType()->getNonOpaquePointerElementType(), StructPtr, Indices);
		}
		AST::UnionType* UnionType = __Generator.FindUnionType((llvm::StructType*)StructPtr->getType()->getNonOpaquePointerElementType());
		if (UnionType) {
			llvm::Type* MemType = UnionType->GetElementType(this->_MemName, __Generator);
			if (MemType == NULL) {
				throw std::logic_error("The union doesn't have a member whose name is \"" + this->_MemName + "\".");
				return NULL;
			}
			return IRBuilder.CreatePointerCast(StructPtr, MemType->getPointerTo());
		}
		throw std::logic_error("Compiler internal error. Maybe the designer forgets to update StructTypeTable or UnionTypeTable");
		return NULL;
	}

	//Structure dereference, e.g. a->x, a->y
	llvm::Value* StructDereference::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* StructDereference::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* StructPtr = this->_StructPtr->CodeGen(__Generator);
		if (!StructPtr->getType()->isPointerTy() || !StructPtr->getType()->getNonOpaquePointerElementType()->isStructTy()) {
			throw std::logic_error("Dereference operator \"->\" must be apply to struct or union pointers.");
			return NULL;
		}
		//Since C language uses name instead of index to fetch the element inside a struct,
		//we need to fetch the AST::StructType* instance according to the llvm::StructType* instance.
		//And it is the same with union types.
		AST::StructType* StructType = __Generator.FindStructType((llvm::StructType*)StructPtr->getType()->getNonOpaquePointerElementType());
		if (StructType) {
			int MemIndex = StructType->GetElementIndex(this->_MemName);
			if (MemIndex == -1) {
				throw std::logic_error("The struct doesn't have a member whose name is \"" + this->_MemName + "\".");
				return NULL;
			}
			std::vector<llvm::Value*> Indices;
			Indices.push_back(IRBuilder.getInt32(0));
			Indices.push_back(IRBuilder.getInt32(MemIndex));
			return IRBuilder.CreateGEP(StructPtr->getType()->getNonOpaquePointerElementType(), StructPtr, Indices);
		}
		AST::UnionType* UnionType = __Generator.FindUnionType((llvm::StructType*)StructPtr->getType()->getNonOpaquePointerElementType());
		if (UnionType) {
			llvm::Type* MemType = UnionType->GetElementType(this->_MemName, __Generator);
			if (MemType == NULL) {
				throw std::logic_error("The union doesn't have a member whose name is \"" + this->_MemName + "\".");
				return NULL;
			}
			return IRBuilder.CreatePointerCast(StructPtr, MemType->getPointerTo());
		}
		throw std::logic_error("Compiler internal error. Maybe the designer forgets to update StructTypeTable or UnionTypeTable");
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
			return IRBuilder.CreateNeg(Operand);
		else
			return IRBuilder.CreateFNeg(Operand);
	}
	llvm::Value* UnaryMinus::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Unary minus only returns right-values.");
		return NULL;
	}

	//Type cast, e.g. (float)n, (int)1.0
	llvm::Value* TypeCast::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* Ret = TypeCasting(this->_Operand->CodeGen(__Generator), this->_VarType->GetLLVMType(__Generator));
		if (Ret == NULL) {
			throw std::logic_error("Unable to do type casting.");
			return NULL;
		}
		return Ret;
	}
	llvm::Value* TypeCast::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Type casting only returns right-values.");
		return NULL;
	}

	//Prefix increment, e.g. ++i
	llvm::Value* PrefixInc::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
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
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
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
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* Indirection::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* Ptr = this->_Operand->CodeGen(__Generator);
		if (!Ptr->getType()->isPointerTy()) {
			throw std::logic_error("Indirection operator \"*\" only applies on pointers or arrays.");
			return NULL;
		}
		return Ptr;
	}

	//Fetch address, e.g. &i
	llvm::Value* AddressOf::CodeGen(CodeGenerator& __Generator) {
		return this->_Operand->CodeGenPtr(__Generator);
	}
	llvm::Value* AddressOf::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Address operator \"&\" only returns right-values.");
		return NULL;
	}

	//Logic NOT, e.g. !x
	llvm::Value* LogicNot::CodeGen(CodeGenerator& __Generator) {
		return IRBuilder.CreateICmpEQ(Cast2I1(this->_Operand->CodeGen(__Generator)), IRBuilder.getInt1(false));
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
		return CreateDiv(LHS, RHS, __Generator);
	}
	llvm::Value* Division::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Division operator \"/\" only returns right-values.");
		return NULL;
	}

	//Multiplication, e.g. x*y
	llvm::Value* Multiplication::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateMul(LHS, RHS, __Generator);
	}
	llvm::Value* Multiplication::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Multiplication operator \"*\" only returns right-values.");
		return NULL;
	}

	//Modulo, e.g. x%y
	llvm::Value* Modulo::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateMod(LHS, RHS, __Generator);
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
		return CreateShl(LHS, RHS, __Generator);
	}
	llvm::Value* LeftShift::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Left shifting operator \"<<\" only returns right-values.");
		return NULL;
	}

	//RightShift, e.g. x>>y
	llvm::Value* RightShift::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateShr(LHS, RHS, __Generator);
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
			return IRBuilder.CreateICmpUGT(
				IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
				IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
			);
		}
		else if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
			return IRBuilder.CreateICmpUGT(
				IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
				TypeUpgrading(RHS, IRBuilder.getInt64Ty())
			);
		}
		else if (LHS->getType()->isIntegerTy() && RHS->getType()->isPointerTy()) {
			return IRBuilder.CreateICmpUGT(
				TypeUpgrading(LHS, IRBuilder.getInt64Ty()),
				IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
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
			return IRBuilder.CreateICmpUGE(
				IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
				IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
			);
		}
		else if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
			return IRBuilder.CreateICmpUGE(
				IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
				TypeUpgrading(RHS, IRBuilder.getInt64Ty())
			);
		}
		else if (LHS->getType()->isIntegerTy() && RHS->getType()->isPointerTy()) {
			return IRBuilder.CreateICmpUGE(
				TypeUpgrading(LHS, IRBuilder.getInt64Ty()),
				IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
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
			return IRBuilder.CreateICmpULT(
				IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
				IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
			);
		}
		else if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
			return IRBuilder.CreateICmpULT(
				IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
				TypeUpgrading(RHS, IRBuilder.getInt64Ty())
			);
		}
		else if (LHS->getType()->isIntegerTy() && RHS->getType()->isPointerTy()) {
			return IRBuilder.CreateICmpULT(
				TypeUpgrading(LHS, IRBuilder.getInt64Ty()),
				IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
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
			return IRBuilder.CreateICmpULE(
				IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
				IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
			);
		}
		else if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
			return IRBuilder.CreateICmpULE(
				IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
				TypeUpgrading(RHS, IRBuilder.getInt64Ty())
			);
		}
		else if (LHS->getType()->isIntegerTy() && RHS->getType()->isPointerTy()) {
			return IRBuilder.CreateICmpULE(
				TypeUpgrading(LHS, IRBuilder.getInt64Ty()),
				IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
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
				IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
				IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
			);
		}
		else if (LHS->getType()->isPointerTy() && RHS->getType()->isIntegerTy()) {
			return IRBuilder.CreateICmpNE(
				IRBuilder.CreatePtrToInt(LHS, IRBuilder.getInt64Ty()),
				TypeUpgrading(RHS, IRBuilder.getInt64Ty())
			);
		}
		else if (LHS->getType()->isIntegerTy() && RHS->getType()->isPointerTy()) {
			return IRBuilder.CreateICmpNE(
				TypeUpgrading(LHS, IRBuilder.getInt64Ty()),
				IRBuilder.CreatePtrToInt(RHS, IRBuilder.getInt64Ty())
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
		return CreateBitwiseAND(LHS, RHS, __Generator);
	}
	llvm::Value* BitwiseAND::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Bitwise AND operator \"&\" only returns right-values.");
		return NULL;
	}

	//BitwiseXOR, e.g. x^y
	llvm::Value* BitwiseXOR::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateBitwiseXOR(LHS, RHS, __Generator);
	}
	llvm::Value* BitwiseXOR::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Bitwise XOR operator \"^\" only returns right-values.");
		return NULL;
	}

	//BitwiseOR, e.g. x|y
	llvm::Value* BitwiseOR::CodeGen(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGen(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateBitwiseOR(LHS, RHS, __Generator);
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
		if (True->getType() == False->getType() || TypeUpgrading(True, False)) {
			return IRBuilder.CreateSelect(Condition, True, False);
		}
		else {
			throw std::domain_error("Thernary operand \" ? : \" using unsupported type combination.");
			return NULL;
		}
	}
	llvm::Value* TernaryCondition::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* Condition = Cast2I1(this->_Condition->CodeGen(__Generator));
		if (Condition == NULL) {
			throw std::logic_error("The first operand of thernary operand \" ? : \" must be able to be cast to boolean.");
			return NULL;
		}
		llvm::Value* True = this->_Then->CodeGenPtr(__Generator);
		llvm::Value* False = this->_Else->CodeGenPtr(__Generator);
		if (True->getType() != False->getType()) {
			throw std::domain_error("When using thernary expressions \" ? : \" as left-values, the latter two operands must be of the same type.");
			return NULL;
		}
		return IRBuilder.CreateSelect(Condition, True, False);
	}

	//DirectAssign, e.g. x=y
	llvm::Value* DirectAssign::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* DirectAssign::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGenPtr(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateAssignment(LHS, RHS, __Generator);
	}

	//DivAssign, e.g. x/=y
	llvm::Value* DivAssign::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* DivAssign::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGenPtr(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateAssignment(LHS,
			CreateDiv(
				IRBuilder.CreateLoad(
					LHS->getType()->getNonOpaquePointerElementType(),
					LHS),
				RHS,
				__Generator),
			__Generator
		);
	}

	//MulAssign, e.g. x*=y
	llvm::Value* MulAssign::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* MulAssign::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGenPtr(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateAssignment(LHS,
			CreateMul(
				IRBuilder.CreateLoad(
					LHS->getType()->getNonOpaquePointerElementType(),
					LHS),
				RHS,
				__Generator),
			__Generator
		);
	}

	//ModAssign, e.g. x%=y
	llvm::Value* ModAssign::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* ModAssign::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGenPtr(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateAssignment(LHS,
			CreateMod(
				IRBuilder.CreateLoad(
					LHS->getType()->getNonOpaquePointerElementType(),
					LHS),
				RHS,
				__Generator),
			__Generator
		);
	}

	//AddAssign, e.g. x+=y
	llvm::Value* AddAssign::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* AddAssign::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGenPtr(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateAssignment(LHS,
			CreateAdd(
				IRBuilder.CreateLoad(
					LHS->getType()->getNonOpaquePointerElementType(),
					LHS),
				RHS,
				__Generator),
			__Generator
		);
	}

	//SubAssign, e.g. x-=y
	llvm::Value* SubAssign::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* SubAssign::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGenPtr(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateAssignment(LHS,
			CreateSub(
				IRBuilder.CreateLoad(
					LHS->getType()->getNonOpaquePointerElementType(),
					LHS),
				RHS,
				__Generator),
			__Generator
		);
	}

	//SHLAssign, e.g. x<<=y
	llvm::Value* SHLAssign::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* SHLAssign::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGenPtr(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateAssignment(LHS,
			CreateShl(
				IRBuilder.CreateLoad(
					LHS->getType()->getNonOpaquePointerElementType(),
					LHS),
				RHS,
				__Generator),
			__Generator
		);
	}

	//SHRAssign, e.g. x>>=y
	llvm::Value* SHRAssign::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* SHRAssign::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGenPtr(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateAssignment(LHS,
			CreateShr(
				IRBuilder.CreateLoad(
					LHS->getType()->getNonOpaquePointerElementType(),
					LHS),
				RHS,
				__Generator),
			__Generator
		);
	}

	//BitwiseANDAssign, e.g. x&=y
	llvm::Value* BitwiseANDAssign::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* BitwiseANDAssign::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGenPtr(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateAssignment(LHS,
			CreateBitwiseAND(
				IRBuilder.CreateLoad(
					LHS->getType()->getNonOpaquePointerElementType(),
					LHS),
				RHS,
				__Generator),
			__Generator
		);
	}

	//BitwiseXORAssign, e.g. x^=y
	llvm::Value* BitwiseXORAssign::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* BitwiseXORAssign::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGenPtr(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateAssignment(LHS,
			CreateBitwiseXOR(
				IRBuilder.CreateLoad(
					LHS->getType()->getNonOpaquePointerElementType(),
					LHS),
				RHS,
				__Generator),
			__Generator
		);
	}

	//BitwiseORAssign, e.g. x|=y
	llvm::Value* BitwiseORAssign::CodeGen(CodeGenerator& __Generator) {
		return CreateLoad(this->CodeGenPtr(__Generator), __Generator);
	}
	llvm::Value* BitwiseORAssign::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* LHS = this->_LHS->CodeGenPtr(__Generator);
		llvm::Value* RHS = this->_RHS->CodeGen(__Generator);
		return CreateAssignment(LHS,
			CreateBitwiseOR(
				IRBuilder.CreateLoad(
					LHS->getType()->getNonOpaquePointerElementType(),
					LHS),
				RHS,
				__Generator),
			__Generator
		);
	}

	//Comma expression, e.g. a,b,c,1
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
		llvm::Value* VarPtr = __Generator.FindVariable(this->_Name);
		if (VarPtr) return CreateLoad(VarPtr, __Generator);
		VarPtr = __Generator.FindConstant(this->_Name);
		if (VarPtr) return VarPtr;
		throw std::logic_error("Identifier \"" + this->_Name + "\" is neither a variable nor a constant.");
		return NULL;
	}
	llvm::Value* Variable::CodeGenPtr(CodeGenerator& __Generator) {
		llvm::Value* VarPtr = __Generator.FindVariable(this->_Name);
		if (VarPtr) return VarPtr;
		VarPtr = __Generator.FindConstant(this->_Name);
		if (VarPtr) {
			throw std::logic_error("\"" + this->_Name + "\" is an immutable constant, not a left-value.");
			return NULL;
		}
		throw std::logic_error("Identifier \"" + this->_Name + "\" is neither a variable nor a constant.");
		return NULL;
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

	//Global string, e.g. "123", "3\"124\t\n"
	llvm::Value* GlobalString::CodeGen(CodeGenerator& __Generator) {
		return IRBuilder.CreateGlobalStringPtr(this->_Content.c_str());
	}
	llvm::Value* GlobalString::CodeGenPtr(CodeGenerator& __Generator) {
		throw std::logic_error("Global string (constant) is a right-value.");
		return NULL;
	}
}