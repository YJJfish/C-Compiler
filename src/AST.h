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
#include <llvm/IR/Value.h>
#include "CodeGenerator.h"

//Forward declarations
namespace AST {
	class Program;
	using Decls = std::vector<Decl*>;
	class Decl;
	class FuncDecl;
	class VarDecl;

}

//Class definitions
namespace AST {
	//Pure virtual class for AST node
	class Node {
		Node(void) {}
		~Node(void) {}
		virtual llvm::Value* CodeGen() = 0;
	};
}