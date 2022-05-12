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
		std::cout << "\t-o: Specify output file (executable). DEFAULT: a.out(Linux) or a.exe(Windows)" << std::endl;
		std::cout << "\t-l: Specify where to dump llvm IR code. Use \" -l - \" to print IR code to the console. DEFAULT: NONE" << std::endl;
		std::cout << "\t-v: Specify where to dump visualization file. DEFAULT: NONE" << std::endl;
		return 1;
	}
	//Parse arguments
	std::string InputFile;			//-i
	std::string OutputExecutable;	//-o
	std::string LLVMIRCodeFile;		//-l
	std::string VisualizationFile;	//-v
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
	if (!ArgParser.TryGetArgment("o", OutputExecutable))
#if defined(_WIN32) || defined(_WIN64)
		//Windows platform
		OutputExecutable = "a.exe";
#elif defined(__unix__)
		//Unix platform
		OutputExecutable = "a.out";
#endif
	//Get IRCode file
	if (!ArgParser.TryGetArgment("l", LLVMIRCodeFile))
		LLVMIRCodeFile = "";
	//Get visualization file
	if (!ArgParser.TryGetArgment("v", VisualizationFile))
		VisualizationFile = "";
	//Start parsing input
	yyparse();
	//Generating code
	CodeGenerator Gen;
	try {
		Gen.GenerateCode(*Root);
	}
	catch (std::exception& e) {
		std::cout << "\033[31m" << e.what() << std::endl;
		return 1;
	}
	//Generate executable
	Gen.GenExecutable(OutputExecutable);
	//Dump IR code
	if (LLVMIRCodeFile != "")
		Gen.DumpIRCode(LLVMIRCodeFile);
	//Visualization
	if (VisualizationFile != "") {
		//To be implemented
	}
}
