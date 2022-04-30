/*
 ***************** CodeGenerator.h ******************
 * This file implements the CodeGenerator class,	*
 * and initialize the global variables needed for	*
 * code generation.									*
 ****************************************************
 */

#include "CodeGenerator.h"

//The global context.
llvm::LLVMContext Context;

//A helper object that makes it easy to generate LLVM instructions.
//It keeps track of the current place to insert instructions and
//has methods to create new instructions.
llvm::IRBuilder<> IRBuilder(Context);