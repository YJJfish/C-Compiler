# Compiler-Principle Course Project

This project implements a compiler that can compile a C-like language into binary codes. It is based on flex, bison, LLVM.

## Dependency

1. Flex & Bison

    The lexer and parser of this project are implemented using flex and bison, respectively.

2. LLVM-14

    The code generation of this project is implemented using the C++ API of LLVM (version 14).

3. CMake

    We use CMake to build our project.

## Installation

1. Windows

   - Install Flex

     Flex can be downloaded from the GnuWin32.

     Link: http://gnuwin32.sourceforge.net/packages/flex.htm

     **Make sure that there is no space characters in your installation path.**

   - Install Bison

     Bison can be downloaded from the GnuWin32.

     http://gnuwin32.sourceforge.net/packages/bison.htm

     **Make sure that there is no space characters in your installation path.**

   - Add `<GNU Flex&Bison installation path>\GnuWin32\bin` to `PATH` environment variable.

   - Install LLVM C++ API

     We recommend using VS2019 on Windows. For instructions on installing LLVM on Windows using Visual Studio, refer to [Getting Started with the LLVM System using Microsoft Visual Studio](https://llvm.org/docs/GettingStartedVS.html). Please make sure that your LLVM's version is 14.

2. Ubuntu

   - Install Flex & Bison

     `sudo apt-get update && sudo apt-get upgrade && sudo apt-get install flex bison`

   - Install LLVM C++ API

     We build our project on LLVM-14. You can directly use `sudo apt-get install llvm-14` to install LLVM-14 on your computer.

## Build & Compile

1. Clone this repository to your local computer:

   `cd <Your working directory>`

   `git clone git@github.com:YJJfish/C-Compiler.git`

   `cd ./Compiler-Principle`

2. Use CMake to construct the project:

   `cmake -S . -B ./build `

   `cd ./build`

   This step will automatically execute `flex` and `bison` command to generate `Lexer.cpp`, `Parser.hpp` and `Parser.cpp` in directory `Compiler-Principle/src`.

   If CMake throws an error due to "zlib" package, please install it on your computer:

   `sudo apt install zlib1g-dev`

3. If you are using Windows and have installed Visual Studio on your computer, by default CMake will construct a Visual Studio solution `Compiler-Principle.sln` in the `build` directory. Open the solution and change compilation mode to `Release x64`, then you can compile the codes using Visual Studio.

   If you are using Ubuntu, by default CMake will construct a project with `Makefile`. Using `make` command to compile the code.

## Grammar

Due to the complexity of C language, to simplify our project task, we design a C-like language which, to some extent, is the subset of the original C language by introducing several restrictions:

1. No macros. Commands including `#include`, `#define` and other macros are not allowed. Functions like `printf` and `scanf` should be declared and then directly used without `#include <stdio.h>`.

    For example:

    ```C
    /*This is an example that prints "Hello World!"*/
    int printf(char ptr, ...);
    int main(void){
    	printf("Hello World!\n");
    	return 0;
    }
    ```

2. All codes should be placed in one single source file. (Since we don't allow any macro including `#include`)

3. Pointer type should be declared using `ptr` instead of `*`, because the syntax parser cannot tell whether `a * b` means "`a` multiplies `b`" or "variable `b` is a pointer pointing to a memory location of type `a`".

    ```C
    typedef int a;	//"a" is an alias for type "int"
    float a;		//"a" is a variable of type "float"
    a * b;			//What does this mean? Expression "a * b" or declaration "int* b"?
    ```

    So, in our language, pointer types should be declared this way:

    ```C
    //In C language, the type of both p and q is "int ptr"
    int * p, * q;	//Illegal in our language.
    
    //In our language, the type of both p and q is "int ptr"
    int ptr p, q;	//Legal.
    ```

4. Expressions with a single variable name and empty variable declarations will result in reduce-reduce conflicts. For example:

    ```C
    typedef int a;	//"a" is an alias for type "int"
    float a;		//"a" is a variable of type "float"
    a;				//What does this mean? An expression or an empty declaration "int;"?
    ```

    We resolve this conflict by make parser prefer empty variable declarations to expressions with a single variable name.

    ```C++
    typedef int a;	//"a" is an alias for type "int"
    float a;		//"a" is a variable of type "float"
    a;				//OK. This is an empty declaration equivalent to "int;"
    ```

5. The array definition in C is very complicated. For example, to define an array of integer pointers and an integer array pointer, we should write:

    ```C
    int* a[20];		//an array of integer pointers
    int (*a)[20];	//an integer array pointer
    ```

    For simplicity, we rewrite the grammar of array definition. In our language, arrays are defined as follows:

    ```C
    int ptr array(20) a;	//an array of integer pointers
    int array(20) ptr a;	//an integer array pointer
    ```

    Here are more examples:

    ```C
    int ptr array(5) array(5) a;		//a 2D array of integer pointers
    int array(5) ptr array(5) b;		//a 1D array of 1D integer array pointers
    int array(5) array(5) ptr c;		//a 2D integer array pointer
    struct {int x, y;} ptr array(10) d;	//a 1D array of struct pointers
    ```

    The advantage of this grammar rule is that, in variable declarations, types and names can be split into different places, thus enabling the parser to deal with them separately. Also, the context-free grammar for variable types is simplified, because the parser only needs to process tokens from left to right, one by one.

6. Initialize variables of complex types when they are declared are not allowed, because to support declarations like:

    ```C
    struct {double array(3) norm; double curve;} array(2) array(2) a
        = {{{{1,2,3}, 4}, {{5,6,7}, 8}}, {{{1,2,3}, 4}, {{5,6,7}, 8}}};
    ```

    , the compiler needs to do a lot of complex things to determine whether the initialization is valid. Therefore, our language only support initializations of simple type variables.

    ```C
    int a = 1;									//Legal
    double r = 5;								//Legal, integer 5 will be cast to double
    int array(2) array(2) b;					//Legal
    int array(2) array(2) c = {{1,2}, {3,4}};	//Illegal
    int array(2) d = {1,2};						//Illegal
    int array(2) e = 1;							//Illegal
    struct {int x, y;} p = {1, 2};				//Illegal
    ```

    If you want to initialize variables of complex types like arrays, please use loop statements.

7. In our language, expressions are a special type of statements. Not all statements have return values, but expressions must have return values (including `void` value).

    Variable declarations, function declarations are statements, but not expressions. This means they do not have return values, and should not exist in places where expressions are expected.

    For example, `for` statement expects one statement, two expressions and one statement:

    ```C
    for (statement; expression; expression) statement;
    ```

    The following code shows some legal and illegal usage of `for` statement:

    ```C
    for (int i = 0; i < n; i++) sum += i;	//Legal
    
    int i; for (i = 0; foo1(i); foo2(i)) foo3(i);	//Legal
    
    for (int i = 0; int j = i; i++);		//"int j = i" is illegal, because it is not an expression
    
    for (int i = 0; i < n; int j = i++);	//"int j = i++" is illegal, because it is not an expression
    
    for (int i = 0; i < n; i++){			//Legal
        int i = 10;	//Legal. We allow redefining variables in the loop body
    };
    ```

8. Like C, We adopt name equivalence for variable types. The following code won't be compiled because the two `struct{int x, y;}` types are not equivalence:

    ```C
    struct {int x, y;} test(void){
    	struct {int x, y;} a;
    	struct {int x, y;} b;
    	a = b;		//Error
    	return a;	//Error
    }
    ```

    To correct this piece of code, you should use `typedef` to declare a structure type at first:

    ```C
    typedef struct {int x, y;} PointTy;
    PointTy test(void){
    	PointTy a;
    	PointTy b;
    	a = b;		//OK
    	return a;	//OK
    }
    ```

    Recursive structure types must be declared using `typedef`:

    ```C
    typedef struct {
    	int Value;
    	Node ptr Next;
    } Node;
    ```

9. Array types in C are very complex and confusing. Let's look at the following two examples:

    ```C
    void foo1(void) {
    	int a[2];
    	a[0] = 1;
    	// %1 = getelementptr inbounds [2 x i32], [2 x i32]* %0, i32 0, i32 0
    	// store i32 1, i32* %1
    }
    void foo2(int a[]) {
    	a[0] = 1;
    	// store i32* %0, i32** %3, align 8
    	// %4 = load i32*, i32** %3, align 8
    	// %5 = getelementptr inbounds i32, i32* %4, i64 0
    	// store i32 1, i32* %5, align 4
    }
    ```

    Although `a` is an array in both functions, the IR codes totally different. In the first example, `a` is a locally defined array. Therefore, the type of `a` is an array type. In the second example, `a` is a parameter. Therefore, the type of `a` is `int*` according to the C standard.

    In our language, to simplify this problem and comply with the C standard simultaneously, when `a` whose type is `int array(n)` is passed as an parameter, instead of treating it as a `int*` pointer, we will treat it as `int array(n) ptr`. This way, it will be treated as an array as if it is defined locally, except that modifying `a[0]` won't result in the modification of the local stack, but rather the modification of the passed array (This complies with the C standard).

In conclusion, The grammar of our language is:

- Terminals:

  ```
  COMMA			","
  ELLIPSES		"..."
  DOT				"."
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
  ARRAY			"array"
  IF				"if"
  ELSE			"else"
  FOR				"for"
  WHILE			"while"
  DO				"do"
  SWITCH			"switch"
  CASE			"case"
  DEFAULT			"default"
  CONTINUE		"continue"
  BREAK			"break"
  RETURN			"return"
  SIZEOF			"sizeof"
  BOOL			"real"
  SHORT			"short"
  INT				"int"
  LONG			"long"
  CHAR			"char"
  FLOAT			"float"
  DOUBLE			"double"
  VOID			"void"
  CHARACTER		"\'\\"."\'" | "\'"[^\\']"\'"
  SQUOTE			"\'"
  STRING			"\""(\\.|[^"\\])*"\""
  DQUOTE			"\""
  IDENTIFIER		[a-zA-Z_][a-zA-Z0-9_]*
  REAL			[0-9]+\.[0-9]+
  INTERGER		[0-9]+
  ```

- Rules:

  ```
  Program ->		Decls
  
  Decls ->		Decls Decl | ε
  
  Decl ->			FuncDecl | VarDecl | TypeDecl | ε
  
  FuncDecl ->		VarType IDENTIFIER LPAREN ArgList RPAREN SEMI |
  				VarType IDENTIFIER LPAREN ArgList RPAREN FuncBody
  
  FuncBody ->		LBRACE Stmts RBRACE
  
  VarDecl ->		VarType VarList SEMI
  
  VarList ->		_VarList COMMA VarInit | VarInit | ε
  
  _VarList ->		_VarList COMMA VarInit | VarInit
  
  VarInit ->		IDENTIFIER |
  				IDENTIFIER ASSIGN Expr
  
  TypeDecl ->		TYPEDEF VarType IDENTIFIER SEMI
  
  VarType ->		_VarType |
  				CONST _VarType
  
  _VarType ->		BuiltInType |
  				STRUCT LBRACE FieldDecls RBRACE |
  				ENUM LBRACE EnmList RBRACE
  				_VarType PTR |
  				_VarType ARRAY LPAREN INTEGER RPAREN |
  				IDENTIFIER
  
  BuiltInType ->	BOOL | SHORT | INT | LONG | CHAR | FLOAT | DOUBLE | VOID
  
  FieldDecls ->	FieldDecls FieldDecl | ε
  
  FieldDecl ->	VarType MemList SEMI | SEMI
  
  MemList ->		_MemList COMMA IDENTIFIER | IDENTIFIER | ε
  
  _MemList ->		_MemList COMMA IDENTIFIER | IDENTIFIER
  
  EnmList ->		_EnmList COMMA Enm | Enm | ε
  
  _EnmList ->		_EnmList COMMA Enm | Enm
  
  Enm ->			IDENTIFIER |
  				IDENTIFIER ASSIGN INTEGER
  
  ArgList ->		_ArgList COMMA Arg |
  				_ArgList COMMA ELLIPSES |
  				Arg |
  				ELLIPSES |
  				ε
  
  _ArgList ->		_ArgList COMMA Arg | Arg
  
  Arg ->			VarType IDENTIFIER | VarType
  
  Block ->		LBRACE Stmts RBRACE
  
  Stmts ->		Stmts Stmt | ε
  
  Stmt ->			Expr SEMI |
  				IfStmt |
  				ForStmt |
  				WhileStmt |
  				DoStmt |
  				SwitchStmt |
  				BreakStmt |
  				ContinueStmt |
  				ReturnStmt |
  				Block |
  				VarDecl |
  				TypeDecl |
  				SEMI
  
  IfStmt ->		IF LPAREN Expr RPAREN Stmt ELSE Stmt |
  				IF LPAREN Expr RPAREN Stmt
  
  ForStmt ->		FOR LPAREN Expr SEMI Expr SEMI Expr RPAREN Stmt |
  				FOR LPAREN VarDecl Expr SEMI Expr RPAREN Stmt
  
  WhileStmt ->	WHILE LPAREN Expr RPAREN Stmt |
  
  DoStmt ->		DO Stmt WHILE LPAREN Expr RPAREN
  
  SwitchStmt->	SWITCH LPAREN Expr RPAREN LBRACE CaseList RBRACE
  
  CaseList ->		CaseList CaseStmt | ε
  
  CaseStmt ->		CASE Expr COLON Stmts |
  				DEFAULT COLON Stmts
  
  ContinueStmt ->	CONTINUE SEMI
  
  BreakStmt ->	BREAK SEMI
  
  ReturnStmt->	RETURN SEMI | RETURN Expr SEMI
  
  Expr ->			LPAREN Expr RPAREN |
  				Expr LBRACKET Expr RBRACKET |
  				SIZEOF LPAREN Expr RPAREN |
  				SIZEOF LPAREN VarType RPAREN |
  				IDENTIFIER LPAREN ExprList RPAREN |
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
  				IDENTIFIER |
  				Constant
  
  ExprList ->		_ExprList COMMA Expr | Expr | ε
  
  _ExprList ->	_ExprList COMMA Expr | Expr
  
  Constant ->		TRUE |
  				FALSE |
  				CHARACTER |
  				INTEGER |
  				REAL |
  				STRING
  ```
  

## Usage

`-i`: Specify input file (source code). REQUIRED.
`-o`: Specify output file (target code). DEFAULT: `a.o`.
`-l`: Specify where to dump llvm IR code. If \"`-l`\" is used but no file is specified, IR code will be printed to the console.
`-v`: Specify where to dump visualization file.
`-O`: Specify the level of optimization. Supported: `-O0`, `-O1`, `-O2`, `-O3`, `-Oz`, `-Os`.

For example, if you have successfully compiled our code and get an executable whose name is `C-Compiler` or `C-Compiler.exe`. In the same directory, there is a test code whose name is `Test.c`. Execute the following command:

`./C-Compiler -i ./Test.c -o Test.o -O3 -l`

After compiling `Test.c` with optimization level `O3` successfully, the IR code will be printed to the console, and the binary code will be written to `Test.o`.

To run the target code on your machine, you can use any linker to transform `Test.o` into an executable. For example:

`gcc ./Test.o`

`./a.exe`(Windows) or `./a.out`(Linux)

## Test Samples

We provide many test samples, from simple ones like `HelloWorld.c` to complex ones like `B+Tree.c`. All codes are placed in `./C-Compiler/test/`.

