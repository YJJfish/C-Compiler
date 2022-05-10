#include "AST.hpp"
#include "CodeGenerator.hpp"
#include "Parser.hpp"
#include <exception>

extern int yyparse();
extern AST::Program* Root;

int main() {
	freopen("E:\\Compiler-Principle\\build\\x64\\Release\\Test.txt", "r", stdin);
	yyparse();
	CodeGenerator Gen;
	try {
		Gen.GenerateCode(*Root);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}