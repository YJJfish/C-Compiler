/*
 ***************** CodeGenerator.h ******************
 * This file implements the CodeGenerator class,	*
 * and initialize the global variables needed for	*
 * code generation.									*
 ****************************************************
 */

#include "CodeGenerator.hpp"

//The global context.
llvm::LLVMContext Context;

//A helper object that makes it easy to generate LLVM instructions.
//It keeps track of the current place to insert instructions and
//has methods to create new instructions.
llvm::IRBuilder<> IRBuilder(Context);

//Implementation of CodeGenerator class
//Constructor
CodeGenerator::CodeGenerator(void) :
	Module(new llvm::Module("main", Context)),
	DL(new llvm::DataLayout(Module)),
	CurrFunction(NULL),
	StructTyTable(NULL),
	SymbolTableStack(),
	ContinueBlockStack(),
	BreakBlockStack(),
	TmpBB(NULL),
	TmpFunc(NULL)
{}

//Sizeof()
llvm::TypeSize CodeGenerator::GetTypeSize(llvm::Type* Type) {
	return this->DL->getTypeAllocSize(Type);
}

//Create and push an empty symbol table
void CodeGenerator::PushSymbolTable(void) {
	this->SymbolTableStack.push_back(new SymbolTable);
}

//Remove the last symbol table
void CodeGenerator::PopSymbolTable(void) {
	if (this->SymbolTableStack.size() == 0) return;
	delete this->SymbolTableStack.back();
	this->SymbolTableStack.pop_back();
}

//Find the llvm::Function* instance for the given name
llvm::Function* CodeGenerator::FindFunction(std::string Name) {
	if (this->SymbolTableStack.size() == 0) return NULL;
	for (auto TableIter = this->SymbolTableStack.end() - 1; TableIter >= this->SymbolTableStack.begin(); TableIter--) {
		auto PairIter = (**TableIter).find(Name);
		if (PairIter != (**TableIter).end())
			return PairIter->second.GetFunction();
	}
	return NULL;
}

//Add a function to the current symbol table
//If an old value exists (i.e., conflict), return false
bool CodeGenerator::AddFunction(std::string Name, llvm::Function* Function) {
	if (this->SymbolTableStack.size() == 0) return false;
	auto& TopTable = *(this->SymbolTableStack.back());
	auto PairIter = TopTable.find(Name);
	if (PairIter != TopTable.end())
		return false;
	TopTable[Name] = Symbol(Function);
	return true;
}

//Find the llvm::Type* instance for the given name
llvm::Type* CodeGenerator::FindType(std::string Name) {
	if (this->SymbolTableStack.size() == 0) return NULL;
	for (auto TableIter = this->SymbolTableStack.end() - 1; TableIter >= this->SymbolTableStack.begin(); TableIter--) {
		auto PairIter = (**TableIter).find(Name);
		if (PairIter != (**TableIter).end())
			return PairIter->second.GetType();
	}
	return NULL;
}

//Add a type to the current symbol table
//If an old value exists (i.e., conflict), return false
bool CodeGenerator::AddType(std::string Name, llvm::Type* Type) {
	if (this->SymbolTableStack.size() == 0) return false;
	auto& TopTable = *(this->SymbolTableStack.back());
	auto PairIter = TopTable.find(Name);
	if (PairIter != TopTable.end())
		return false;
	TopTable[Name] = Symbol(Type);
	return true;
}

//Find variable
llvm::Value* CodeGenerator::FindVariable(std::string Name) {
	if (this->SymbolTableStack.size() == 0) return NULL;
	for (auto TableIter = this->SymbolTableStack.end() - 1; TableIter >= this->SymbolTableStack.begin(); TableIter--) {
		auto PairIter = (**TableIter).find(Name);
		if (PairIter != (**TableIter).end())
			return PairIter->second.GetVariable();
	}
	return NULL;
}

//Add a variable to the current symbol table
//If an old value exists (i.e., conflict), return false
bool CodeGenerator::AddVariable(std::string Name, llvm::Value* Variable) {
	if (this->SymbolTableStack.size() == 0) return false;
	auto& TopTable = *(this->SymbolTableStack.back());
	auto PairIter = TopTable.find(Name);
	if (PairIter != TopTable.end())
		return false;
	TopTable[Name] = Symbol(Variable, false);
	return true;
}

//Find constant
llvm::Value* CodeGenerator::FindConstant(std::string Name) {
	if (this->SymbolTableStack.size() == 0) return NULL;
	for (auto TableIter = this->SymbolTableStack.end() - 1; TableIter >= this->SymbolTableStack.begin(); TableIter--) {
		auto PairIter = (**TableIter).find(Name);
		if (PairIter != (**TableIter).end())
			return PairIter->second.GetConstant();
	}
	return NULL;
}

//Add a constant to the current symbol table
//If an old value exists (i.e., conflict), return false
bool CodeGenerator::AddConstant(std::string Name, llvm::Value* Constant) {
	if (this->SymbolTableStack.size() == 0) return false;
	auto& TopTable = *(this->SymbolTableStack.back());
	auto PairIter = TopTable.find(Name);
	if (PairIter != TopTable.end())
		return false;
	TopTable[Name] = Symbol(Constant, true);
	return true;
}

//Find the AST::StructType* instance according to the llvm::StructType* instance
AST::StructType* CodeGenerator::FindStructType(llvm::StructType* Ty1) {
	auto PairIter = this->StructTyTable->find(Ty1);
	if (PairIter != this->StructTyTable->end())
		return PairIter->second;
	return NULL;
}

//Add a <llvm::StructType*, AST::StructType*> mapping
bool CodeGenerator::AddStructType(llvm::StructType* Ty1, AST::StructType* Ty2) {
	auto PairIter = this->StructTyTable->find(Ty1);
	if (PairIter != this->StructTyTable->end())
		return false;
	(*this->StructTyTable)[Ty1] = Ty2;
	return true;
}

//Find the AST::UnionType* instance according to the llvm::StructType* instance
AST::UnionType* CodeGenerator::FindUnionType(llvm::StructType* Ty1) {
	auto PairIter = this->UnionTyTable->find(Ty1);
	if (PairIter != this->UnionTyTable->end())
		return PairIter->second;
	return NULL;
}

//Add a <llvm::StructType*, AST::UnionType*> mapping
bool CodeGenerator::AddUnionType(llvm::StructType* Ty1, AST::UnionType* Ty2) {
	auto PairIter = this->UnionTyTable->find(Ty1);
	if (PairIter != this->UnionTyTable->end())
		return false;
	(*this->UnionTyTable)[Ty1] = Ty2;
	return true;
}

//Set current function
void CodeGenerator::EnterFunction(llvm::Function* Func) {
	this->CurrFunction = Func;
}

//Remove current function
void CodeGenerator::LeaveFunction(void) {
	this->CurrFunction = NULL;
}

//Get the current function
llvm::Function* CodeGenerator::GetCurrentFunction(void) {
	return this->CurrFunction;
}

//Called whenever entering a loop
void CodeGenerator::EnterLoop(llvm::BasicBlock* ContinueBB, llvm::BasicBlock* BreakBB) {
	this->ContinueBlockStack.push_back(ContinueBB);
	this->BreakBlockStack.push_back(BreakBB);
}

//Called whenever leaving a loop
void CodeGenerator::LeaveLoop(void) {
	if (this->ContinueBlockStack.size() == 0 || this->BreakBlockStack.size() == 0) return;
	this->ContinueBlockStack.pop_back();
	this->BreakBlockStack.pop_back();
}

//Get the destination block for "continue" statements
llvm::BasicBlock* CodeGenerator::GetContinueBlock(void) {
	if (this->ContinueBlockStack.size())
		return this->ContinueBlockStack.back();
	else
		return NULL;
}

//Get the destination block for "break" statements
llvm::BasicBlock* CodeGenerator::GetBreakBlock(void) {
	if (this->BreakBlockStack.size())
		return this->BreakBlockStack.back();
	else
		return NULL;
}

//Exchange the current insert point with TmpBB
void CodeGenerator::XchgInsertPointWithTmpBB(void) {
	auto Tmp = IRBuilder.GetInsertBlock();
	IRBuilder.SetInsertPoint(this->TmpBB);
	this->TmpBB = Tmp;
}

//Pass the root of the ast to this function and generate code
void CodeGenerator::GenerateCode(AST::Program& Root, const std::string& OptimizeLevel) {

	//Initialize symbol table
	this->StructTyTable = new StructTypeTable;
	this->UnionTyTable = new UnionTypeTable;
	this->PushSymbolTable();

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
	this->PopSymbolTable();
	delete this->StructTyTable; this->StructTyTable = NULL;
	delete this->UnionTyTable; this->UnionTyTable = NULL;

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

//Dump the LLVM-IR to the specified file
void CodeGenerator::DumpIRCode(std::string FileName) {
	if (FileName == "") FileName = "-";
	std::error_code EC;
	llvm::raw_fd_ostream Out(FileName, EC);
	Out << "********************  IRCode  ********************\n";
	this->Module->print(Out, NULL);
	Out << "*****************  Verification  *****************\n";
	if (llvm::verifyModule(*this->Module, &Out) == 0)
		Out << "No errors.\n";
}

//Generate object code
void CodeGenerator::GenObjectCode(std::string FileName) {
	auto TargetTriple = llvm::sys::getDefaultTargetTriple();
	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();
	std::string Error;
	auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
	if (!Target) {
		throw std::runtime_error(Error);
		return;
	}
	auto CPU = "generic";
	auto Features = "";
	llvm::TargetOptions opt;
	auto RM = llvm::Optional<llvm::Reloc::Model>();
	auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);
	Module->setDataLayout(TargetMachine->createDataLayout());
	Module->setTargetTriple(TargetTriple);
	std::error_code EC;
	llvm::raw_fd_ostream Dest(FileName, EC, llvm::sys::fs::OF_None);
	if (EC) {
		throw std::runtime_error("Could not open file: " + EC.message());
		return;
	}
	auto FileType = llvm::CGFT_ObjectFile;
	llvm::legacy::PassManager PM;
	if (TargetMachine->addPassesToEmitFile(PM, Dest, nullptr, FileType)) {
		throw std::runtime_error("TargetMachine can't emit a file of this type");
		return;
	}
	PM.run(*Module);
	Dest.flush();
}

//Generate visualization file (.html)
void CodeGenerator::GenHTML(std::string FileName, AST::Program& Root) {
	extern const char* Html;
	std::string OutputString = Html;
	std::string Json = Root.astJson();
	std::string Target = "${ASTJson}";
	auto Pos = OutputString.find(Target);
	OutputString.replace(Pos, Target.length(), Json.c_str());
	std::ofstream HTMLFile(FileName);
	HTMLFile << OutputString;
}