#include <exception>
#include "AST.hpp"
#include "CodeGenerator.hpp"
#include "Parser.hpp"
#include "ArgsParser.hpp"


extern int yyparse();
extern AST::Program* Root;

int main(int argc, const char* argv[]) {
	if (argc == 1) {
		std::cout << "[" << argv[0] << "] Usage:" << std::endl;
		std::cout << "\t-i: Specify input file (source code). REQUIRED" << std::endl;
		std::cout << "\t-o: Specify output file (target code). DEFAULT: a.o" << std::endl;
		std::cout << "\t-l: Specify where to dump llvm IR code. If \"-l\" is used but no file is specified, IR code will be printed to the console." << std::endl;
		std::cout << "\t-v: Specify where to dump visualization file." << std::endl;
		std::cout << "\t-O: Specify the level of optimization. Supported: -O0, -O1, -O2, -O3, -Oz, -Os." << std::endl;
		return 1;
	}
	//Parse arguments
	std::string InputFile;							//-i
	std::string OutputExecutable;					//-o
	std::string LLVMIRCodeFile;	bool GenIR;			//-l
	std::string VisualizationFile; bool GenVis;		//-v
	std::string OptimizeLevel;						//-O
	ArgsParser ArgParser(argc, argv);
	//Get input file
	if (!ArgParser.TryGetArgment("i", InputFile)) {
#if defined(_WIN32) || defined(_WIN64)
		//Windows platform
		std::cout << "Fatal error: no input file." << std::endl;
#elif defined(__unix__)
		//Unix platform
		std::cout << "\033[31m" << "Fatal error: no input file." << std::endl;
#endif
		return 1;
	}
	freopen(InputFile.c_str(), "r", stdin);
	//Get output file
	if (!ArgParser.TryGetArgment("o", OutputExecutable) || OutputExecutable == "") {
		OutputExecutable = "a.o";
	}
	else if (OutputExecutable.length() <= 2 || OutputExecutable.substr(OutputExecutable.length() - 2) != ".o") {
		OutputExecutable = OutputExecutable + ".o";
	}
	//Get IRCode file
	GenIR = ArgParser.TryGetArgment("l", LLVMIRCodeFile);
	//Get visualization file
	GenVis = ArgParser.TryGetArgment("v", VisualizationFile);
	//Get optimization option
	if (ArgParser.TryGetArgment("O0", OptimizeLevel))
		OptimizeLevel = "O0";
	else if (ArgParser.TryGetArgment("O1", OptimizeLevel))
		OptimizeLevel = "O1";
	else if (ArgParser.TryGetArgment("O2", OptimizeLevel))
		OptimizeLevel = "O2";
	else if (ArgParser.TryGetArgment("O3", OptimizeLevel))
		OptimizeLevel = "O3";
	else if (ArgParser.TryGetArgment("Os", OptimizeLevel))
		OptimizeLevel = "Os";
	else if (ArgParser.TryGetArgment("Oz", OptimizeLevel))
		OptimizeLevel = "Oz";
	else OptimizeLevel = "";
	//Start parsing input
	yyparse();
	//Generating code
	CodeGenerator Gen;
	try {
		Gen.GenerateCode(*Root, OptimizeLevel);
	}
	catch (std::exception& e) {
#if defined(_WIN32) || defined(_WIN64)
		//Windows platform
		std::cout << e.what() << std::endl;
#elif defined(__unix__)
		//Unix platform
		std::cout << "\033[31m" << e.what() << std::endl;
#endif
		return 1;
	}
	//Generate executable
	Gen.GenExecutable(OutputExecutable);
	//Dump IR code
	if (GenIR)
		Gen.DumpIRCode(LLVMIRCodeFile);
	//Visualization
	if (GenVis) {
		//To be implemented
	}
}
