# Compiler-Principle Course Project

This project implements a compiler that can compile a C-like language into binary codes. It is based on flex, yacc, LLVM.

## Dependency

1. LLVM
    -   On Windows: We recommend using VS2019 on Windows. For instructions on installing LLVM on Windows using Visual Studio, refer to [Getting Started with the LLVM System using Microsoft Visual Studio](https://llvm.org/docs/GettingStartedVS.html).
    -   On Ubuntu: You can directly use `sudo apt install llvm` to install LLVM on your computer. (Maybe you need to add some PPAs and update packages beforehand.)
2. flex&yacc
    -   



## Grammar

Due to the complexity of C language, to simplify our project task, we design a C-like language which, to some extent, is the subset of the original C language by introducing several restrictions:

1. No macros (Commands including `#include`, `#define` and other macros are not allowed. Functions like `printf` and `scanf` should be directly used without `#include <stdio.h>`).

2. All codes should be placed in one single source file.

3. Pointer type should be declared using `ptr` instead of `*`, because the syntax parser cannot tell whether `a * b` means "`a` multiplies `b`" or "variable `b` is a pointer pointing to a memory location of type `a`".

    ```C
    //In C language, the type of p is "int*", the type of q is "int"
    int * p, q;		//Illegal in our language.
    
    //In our language, the type of both p and q is "int ptr"
    int ptr p, q;	//Legal.
    ```

4. For simplicity, we rewrite the grammar of array definition. In C, arrays are defined as follows:

    ```C
    int a[20], b[10], c[20];
    ```

    In our language, arrays are defined as follow:

    ```C
    int[20] a, c;
    int[10] b;
    ```

    The following code shows how to define arrays of float pointers:

    ```C
    float ptr[10] FPtr1D;
    float ptr[5][5] FPtr2D;
    ```

    The following code shows how to define an array of 20 pointers pointing to bool arrays:

    ```C
    bool[10]ptr[20] ArrayPtr1D;
    ```

    The advantage of this grammar rule is that, in variable declarations, types and names can be split into different places, thus enabling the parser to deal with them separately.

5. Initialize variables of complex types when they are declared are not allowed, because to support the following declaration:

    ```C
    int[2][2] a = {{1,2}, {3,4}};
    ```

    , we need a grammar that can recognize the following language:

    -   On the left hand side of "=", there is a string consisting of matched bracket pairs, and there should not be any nesting.
    -   On the right hand side of "=", there is a string consisting of matched nested brace pairs.
    -   The number of bracket pairs equals to the nesting level of brace pairs

    Unfortunately, this language is not context-free. Therefore, our language only support initializations of simple type variables.

    ```C
    int a = 1;						//Legal
    int[2][2] b;					//Legal
    int[2][2] c = {{1,2}, {3,4}};	//Illegal
    int[2] d = {1,2};				//Illegal
    int[2] e = 1;					//Illegal
    ```

6. For simplicity, in our language, a semicolon should be added to the end of a pair of braces. In C, there is no need to write semicolons after braces.

    ```C
    int abs(int x){
    	if (x > 0){
    		return x;
    	};
    	else{
    		return -x;
    	};
    };
    ```

    



In conclusion, The grammar of our language is:

- Terminals:

  ```
  COMMA			","
DOT				"."
QUOTE			"\""
SEMI			";"
LPAREN			"("
RPAREN			")"
LBRACKET		"["
RBRACKET		"]"
LBRACE			"{"
RBRACE			"}"
SHLEQ			"<<="
SHL				"<<"
SHREQ			">>="
SHR				">>"
EQ				"=="
GE				">="
GT				">"
LE				"<="
LT				"<"
NEQ				"!="
NOT				"!"
ASSIGN			"="
AND				"&&"
BANDEQ			"&="
BAND			"&"
OR				"||"
BOREQ			"|="
BOR				"|"
ARW				"->"
BXOREQ			"^="
BXOR			"^"
BNOT			"~"
DADD			"++"
ADDEQ			"+="
ADD				"+"
DSUB			"--"
SUBEQ			"-="
SUB				"-"
MULEQ			"*="
MUL				"*"
DIVEQ			"/="
DIV				"/"
MODEQ			"%="
MOD				"%"
QUES			"?"
COLON			":"
STRUCT			"struct"
TYPEDEF			"typedef"
CONST			"const"
ENUM			"enum"
PTR				"ptr"
IF				"if"
ELSE			"else"
FOR				"for"
WHILE			"while"
DO				"do"
SWITCH			"switch"
CASE			"case"
DEFAULT			"default"
RETURN			"return"
IDENTIFIER		[a-zA-Z_][a-zA-Z0-9_]*
REAL			[0-9]+\.[0-9]+ 
INTERGER		[0-9]+
  
  
  ```

- Rules:

  ```
  Program ->	Decls

Decls ->	Decls Decl SEMI| ε

Decl ->		FuncDecl | VarDecl | TypeDecl

FuncDecl ->	VarType IDENTIFIER LPAREN ArgList RPAREN |
			VarType IDENTIFIER LPAREN ArgList RPAREN Block

VarDecl ->	VarType VarList

VarList ->	_VarList COMMA VarInit | VarInit | ε

_VarList ->	_VarList COMMA VarInit | VarInit

VarInit ->	IDENTIFIER |
			IDENTIFIER ASSIGN Expr

TypeDecl ->	TYPEDEF VarType IDENTIFIER

VarType ->	_VarType |
			CONST _VarType

_VarType ->	IDENTIFIER |
			STRUCT LBRACE VarDecls RBRACE |
			ENUM LBRACE EnmList RBRACE
			_VarType PTR |
			_VarType LBRACKET INTEGER RBRACKET

VarDecls ->	VarDecls VarDecl SEMI | ε

EnmList ->	_EnmList COMMA Enm | Enm | ε

_EnmList ->	_EnmList COMMA Enm | Enm

Enm ->		IDENTIFIER |
			IDENTIFIER ASSIGN INTEGER

ArgList ->	_ArgList COMMA Arg | Arg | ε

_ArgList ->	_ArgList COMMA Arg | Arg

Arg ->		VarType IDENTIFIER | VarType

Block ->	LBRACE Stmts RBRACE

Stmts ->	Stmts Stmt SEMI | ε

Stmt ->		Decl | Expr | IfStmt | ForStmt | WhileStmt | DoStmt | SwitchStmt | ReturnStmt | Block

IfStmt ->	IF LPAREN Expr RPAREN Stmt |
			IF LPAREN Expr RPAREN Stmt ELSE Stmt

ForStmt ->	FOR LPAREN Expr SEMI Expr SEMI Expr LPAREN Stmt

WhileStmt -> WHILE LPAREN Expr RPAREN Stmt

DoStmt ->	DO Stmt WHILE LPAREN Expr RPAREN

SwitchStmt->SWITCH LPAREN Expr RPAREN LBRACE CaseList RBRACE

CaseList ->	CaseList CaseStmt | ε

CaseStmt ->	CASE Expr Stmts | DEFAULT Stmts

ReturnStmt -> RETURN Expr

Expr ->		Expr LBRACKET Expr RBRACKET | 
			IDENTIFIER LPAREN Expr RPAREN | 
			Expr DOT IDENTIFIER |
			Expr ARW IDENTIFIER |
			ADD Expr |
			SUB Expr |
			LPAREN VarType RPAREN Expr |
			DADD Expr |
			Expr DADD |
			DSUB Expr |
			Expr DSUB |
			MUL Expr |
			BAND Expr |
			NOT Expr |
			BNOT Expr |
			Expr DIV Expr |
			Expr MUL Expr |
			Expr MOD Expr |
			Expr ADD Expr |
			Expr SUB Expr |
			Expr SHL Expr |
			Expr SHR Expr |
			Expr GT Expr |
			Expr GE Expr |
			Expr LT Expr |
			Expr LE Expr |
			Expr EQ Expr |
			Expr NEQ Expr |
			Expr BAND Expr |
			Expr BXOR Expr |
			Expr BOR Expr |
			Expr AND Expr |
			Expr OR Expr |
			Expr QUES Expr COLON Expr |
			Expr ASSIGN Expr |
			Expr DIVEQ Expr |
			Expr MULEQ Expr |
			Expr MODEQ Expr |
			Expr ADDEQ Expr |
			Expr SUBEQ Expr |
			Expr SHLEQ Expr |
			Expr SHREQ Expr |
			Expr BANDEQ Expr |
			Expr BXOREQ Expr |
			Expr BOREQ Expr |
			Expr COMMA Expr |
			QUOTE IDENTIFIER QUOTE | 
			INTEGER |
			REAL
  ```

  

