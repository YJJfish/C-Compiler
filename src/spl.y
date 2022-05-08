%{
#include "ast.h" 
#include <string>
#include <iostream>

using namespace std;


void yyerror(const char *s) { 
    std::printf("Error: %s\n", s);
    std::exit(1); 
}

int yylex(void);

Program *root;
%}
%union {
    int iVal;
    std::string *sVal;
    double dVal;
    char cVal;
    Program *program;
    Decl *decl;
    Decls *decls; 
    FuncDecl *funcDecl;
    FuncBody *funcBody;
    VarDecl *varDecl;
    TypeDecl *typeDecl;
    VarType *varType;
    BuiltInType* builtInType;
    FieldDecls* fieldDecls;
	FieldDecl* fieldDecl;
	MemList* memList;
    Stmt *stmt;
    IfStmt *ifStmt;
    ForStmt *forStmt;
    WhileStmt *whileStmt;
    DoStmt *doStmt;
    SwitchStmt *switchStmt;
    BreakStmt* breakStmt;
	ContinueStmt* continueStmt;
    ReturnStmt *returnStmt;
    Stmts *stmts;
    Block *block;
    Arg *arg;
    ArgList *argList;
    VarInit *varInit;
    VarList *varList;
    Expr *expr;
    Constant* constant;
	ExprList* exprLis;
    Enm *enm;
    EnmList *enmList;
}

%token  COMMA DOT SQUOTE DQUOTE SEMI QUES COLON
		LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE
		SHLEQ SHL SHREQ SHR
		EQ GE GT LE LT NEQ NOT ASSIGN
		AND BANDEQ BAND OR BOREQ BOR
		ARW BXOREQ BXOR BNOT
		DADD ADDEQ ADD DSUB SUBEQ SUB
		MULEQ MUL DIVEQ DIV MODEQ MOD
		STRUCT TYPEDEF CONST ENUM PTR ARRAY
		IF ELSE FOR WHILE DO SWITCH CASE DEFAULT 
		BREAK CONTINUE RETURN SIZEOF TRUE FALSE
		BOOL SHORT INT LONG CHAR FLOAT DOUBLE VOID		
		
%token<iVal> INTEGER
%token<sVal> IDENTIFIER 
%token<dVal> REAL
%token<cVal> CHARACTER
%type<progam>							Program	
%type<decl>								Decl	
%type<decls>							Decls	
%type<funcDecl>							FuncDecl
%type<funcBody>							FuncBody	
%type<varDecl>							VarDecl	
%type<typeDecl>							TypeDecl	
%type<varType>							VarType _VarType
%type<builtInType>						BuiltInType
%type<fieldDecls>						FieldDecls
%type<fieldDecl>						FieldDecl
%type<memList>							MemList _MemList	
%type<stmt>								Stmt
%type<ifStmt>							IfStmt
%type<forStmt>							ForStmt
%type<whileStmt>						WhileStmt
%type<doStmt>							DoStmt
%type<switchStmt>						SwitchStmt
%type<breakStmt>						BreakStmt
%type<continueStmt>						ContinueStmt
%type<returnStmt>						ReturnStmt	
%type<stmts>							Stmts	
%type<block>							Block
%type<arg>								Arg	
%type<argList>							ArgList _ArgList
%type<varInit>							VarInit	
%type<varList>							VarList _VarList
%type<expr>								Expr	
%type<constant>							Constant
%type<exprList>							ExprList _ExprList
%type<enm>								Enm
%type<enmList>							EnmList	_EnmList
%left	COMMA //15
%right	ASSIGN ADDEQ SUBEQ MULEQ DIVEQ MODEQ SHLEQ SHREQ BANDEQ BOREQ BXOREQ //14
%right	QUES COLON //13
%left	OR//12
%left	AND//11
%left	BOR//10
%left	BXOR//9
%left	BAND//8
%left	EQ NEQ//7
%left	GE GT LE LT//6
%left	SHL SHR//5
%left	ADD SUB//4
%left	MUL DIV MOD//3
%right	DADD DSUB NOT BNOT SIZEOF//2
%left	DOT ARW//1

%start Program
%%
//
Program:	Decls													{  $$ = new Program($1);   }
			;
			
Decls:		Decls Decl      										{  $$ = $1; if($2 != NULL) $$->push_back($2);   }
			|														{  $$ = new Decls();   }
			;

Decl:		FuncDecl												{  $$ = $1;   }
			| VarDecl												{  $$ = $1;   }
			| TypeDecl												{  $$ = $1;   }
			|														{  Decl-> ; $$ = NULL;   }
			;

FuncDecl:	VarType IDENTIFIER LPAREN ArgList RPAREN SEMI 			{  $$ = new FuncDecl($1,$2,$4);   }
			| VarType IDENTIFIER LPAREN ArgList RPAREN FuncBody		{  $$ = new FuncDecl($1,$2,$4,$6);   }
			;

FuncBody:	LBRACE Stmts RBRACE										{  $$ = new FuncBody($2);   }
			;

VarDecl:	VarType VarList	SEMI   									{  $$ = new VarDecl($1,$2);   }
			;

VarList:	_VarList COMMA VarInit									{  $$ = $1; $$->push_back($3);   }
			| VarInit												{  $$ = new VarList(); $$->push_back($1);   }											
			|														{  $$ = new VarList();   }
			;

_VarList:	_VarList COMMA VarInit									{  $$ = $1; $$->push_back($3);   }
			| VarInit												{  $$ = new VarList(); $$->push_back($1);   }	
			;

VarInit:	IDENTIFIER												{  $$ = new VarInit($1);   }
			| IDENTIFIER ASSIGN Expr								{  $$ = new VarInit($1,$3);   }
			;

TypeDecl:	TYPEDEF VarType IDENTIFIER	SEMI						{  $$ = new TypeDecl($2,$3);   }
			;

VarType:	_VarType												{  $$ = $1;   }
			| CONST _VarType										{  $$ = $1; $$->SetConst();   }
			;

_VarType:	BuiltInType												{  $$ = $1;   }
			| STRUCT LBRACE FieldDecls RBRACE						{  $$ = new StructType($3);   }
			| ENUM LBRACE EnmList RBRACE							{  $$ = new EnumType($3);   }
			| _VarType PTR											{  $$ = new PointerType($1);   }
			| _VarType ARRAY LPAREN INTEGER RPAREN					{  $$ = new ArrayType($1,$4);   }
			| IDENTIFIER											{  $$ = new DefinedType($1);   }
			;
			
BuiltInType: BOOL													{  $$ = new BuiltInType(_Bool);   }
			| SHORT													{  $$ = new BuiltInType(_Short);   }
			| INT													{  $$ = new BuiltInType(_Int);   }
			| LONG													{  $$ = new BuiltInType(_Long);   }
			| CHAR													{  $$ = new BuiltInType(_Char);   }
			| FLOAT													{  $$ = new BuiltInType(_Float);   }
			| DOUBLE												{  $$ = new BuiltInType(_Double);   }
			| VOID													{  $$ = new BuiltInType(_Void);   }
			;

FieldDecls:	FieldDecls FieldDecl            						{  $$ = $1; if($2 != NULL) $$->push_back($2);   }
			|														{  $$ = new FieldDecls();   }
			;

FieldDecl:	VarType MemList	SEMI    								{  $$ = new FieldDecl($1,$2);   }
			| SEMI													{  $$ = NULL;   }
			;

MemList:	_MemList COMMA IDENTIFIER								{  $$ = $1; $$->push_back($3);   }
			| IDENTIFIER											{  $$ = new MemList(); $$->push_back($1);   }	
			|														{  $$ = new MemList();   }
			;

_MemList:	_MemList COMMA IDENTIFIER								{  $$ = $1; $$->push_back($3);   }
			| IDENTIFIER											{  $$ = new MemList(); $$->push_back($1);   }
			;

EnmList:	_EnmList COMMA Enm										{  $$ = $1; $$->push_back($3);   }	
			| Enm													{  $$ = new EnmList(); $$->push_back($1);   }
			|														{  $$ = new EnmList();   }		
			;

_EnmList:	_EnmList COMMA Enm										{  $$ = $1; $$->push_back($3);   }	
			| Enm													{  $$ = new EnmList(); $$->push_back($1);   }
			;

Enm:		IDENTIFIER												{  $$ = new Enm($1);   }
			| IDENTIFIER ASSIGN INTEGER								{  $$ = new Enm($1,true,$3);   };
			;

ArgList:	_ArgList COMMA Arg										{  $$ = $1; $$->push_back($3);   }	 
			| Arg													{  $$ = new ArgList(); $$->push_back($1);   }
			|														{  $$ = new ArgList();   }
			;

_ArgList:	_ArgList COMMA Arg										{  $$ = $1; $$->push_back($3);   }	 
			| Arg													{  $$ = new ArgList(); $$->push_back($1);   }
			;

Arg:		VarType IDENTIFIER										{  $$ = new Arg($1,$2);   }
			| VarType												{  $$ = new Arg($1);   }
			;

Block:		LBRACE Stmts RBRACE										{  $$ = new Block($2);   }
			;

Stmts:		Stmts Stmt      										{  $$ = $1; if($2 != NULL) $$->push_back($2);   }	
			|														{  $$ = new Stmts();   }
			;

Stmt:		Expr SEMI												{  $$ = $1;   }
			| IfStmt												{  $$ = $1;   }
			| ForStmt												{  $$ = $1;   }
			| WhileStmt												{  $$ = $1;   }
			| DoStmt												{  $$ = $1;   }
			| SwitchStmt											{  $$ = $1;   } 
			| BreakStmt												{  $$ = $1;   }
			| ContinueStmt											{  $$ = $1;   }
			| ReturnStmt											{  $$ = $1;   }
			| Block													{  $$ = $1;   }
			| VarDecl												{  $$ = $1;   }
			| TypeDecl												{  $$ = $1;   }
			| SEMI													{  $$ = NULL;   }
			;

IfStmt:		IF LPAREN Expr RPAREN Stmt ELSE Stmt					{  $$ = new IfStmt($3,$5,$7);   }
			| IF LPAREN Expr RPAREN Stmt         					{  $$ = new IfStmt($3,$5);   }
			;

ForStmt:	FOR LPAREN Expr SEMI Expr SEMI Expr RPAREN Stmt			{  $$ = new ForStmt($3,$5,$7,$9);   }
            | FOR LPAREN VarDecl Expr SEMI Expr RPAREN Stmt			{  $$ = new ForStmt($3,$4,$6,$8);   }
			;

WhileStmt:	WHILE LPAREN Expr RPAREN Stmt							{  $$ = new WhileStmt($3,$5);   }
			;

DoStmt:		DO Stmt WHILE LPAREN Expr RPAREN						{  $$ = new DoStmt($2,$5);   }
			;

SwitchStmt:	SWITCH LPAREN Expr RPAREN LBRACE CaseList RBRACE		{  $$ = new SwitchStmt($3,$6);   }
			;

CaseList:	CaseList CaseStmt										{  $$ = $1; $$->push_back($2);   }	
			|														{  $$ = new CaseList();   }
			;

CaseStmt:	CASE Expr COLON Stmts									{  $$ = new CaseStmt($2,$4);   }
			| DEFAULT COLON Stmts									{  $$ = new CaseStmt(NULL,$4);   }
			;


ContinueStmt:CONTINUE SEMI											{  $$ = new ContinueStmt();   }
			;

BreakStmt:	BREAK SEMI												{  $$ = new BreakStmt();   }
			;

ReturnStmt: RETURN SEMI 											{  $$ = new ReturnStmt();   }
			| RETURN Expr SEMI										{  $$ = new ReturnStmt($2);   }
			;

Expr:		Expr LBRACKET Expr RBRACKET								{  $$ = new Subscrpit($1,$3);   }
			| SIZEOF LPAREN Expr RPAREN								{  $$ = new SizeOf($3);   }
			| SIZEOF LPAREN VarType RPAREN							{  $$ = new SizeOf($3);   }
			| IDENTIFIER LPAREN ExprList RPAREN						{  $$ = new FunctionCall($1,$3);   }
			| Expr DOT IDENTIFIER									{  $$ = new StructReference($1,$3);   }
			| Expr ARW IDENTIFIER									{  $$ = new StructDereference($1,$3);   }
			| ADD Expr	%prec NOT									{  $$ = new UnaryPlus($2);   }
			| SUB Expr	%prec NOT									{  $$ = new UnaryMinus($2);   }
			| LPAREN VarType RPAREN Expr							{  $$ = new TypeCase($2,$4);   }
			| DADD Expr	%prec NOT									{  $$ = new PrefixInc($2);   }
			| Expr DADD %prec ARW									{  $$ = new PostfixInc($1);   }
			| DSUB Expr %prec NOT									{  $$ = new PrefixDec($2);   }
			| Expr DSUB	%prec ARW									{  $$ = new PostfixDec($1);   }
			| MUL Expr												{  $$ = new Indirection($2);   }
			| BAND Expr												{  $$ = new AddressOf($2);   }
			| NOT Expr												{  $$ = new LogicNot($2);   }
			| BNOT Expr												{  $$ = new BitwiseNot($2);   }
			| Expr DIV Expr											{  $$ = new Division($1,$3);   }
			| Expr MUL Expr											{  $$ = new Multiplication($1,$3);   } 
			| Expr MOD Expr 										{  $$ = new Modulo($1,$3);   }
			| Expr ADD Expr											{  $$ = new Addition($1,$3);   } 
			| Expr SUB Expr											{  $$ = new Subtraction($1,$3);   } 
			| Expr SHL Expr											{  $$ = new LeftShift($1,$3);   } 
			| Expr SHR Expr											{  $$ = new RightShift($1,$3);   } 
			| Expr GT Expr											{  $$ = new LogicGT($1,$3);   } 
			| Expr GE Expr											{  $$ = new LogicGE($1,$3);   } 
			| Expr LT Expr											{  $$ = new LogicLT($1,$3);   } 
			| Expr LE Expr											{  $$ = new LogicLE($1,$3);   } 
			| Expr EQ Expr											{  $$ = new LogicEQ($1,$3);   } 
			| Expr NEQ Expr											{  $$ = new LogicNEQ($1,$3);   } 
			| Expr BAND Expr										{  $$ = new BitwiseAND($1,$3);   }
			| Expr BXOR Expr										{  $$ = new BitwiseXOR($1,$3);   }
			| Expr BOR Expr											{  $$ = new BitwiseOR($1,$3);   } 
			| Expr AND Expr											{  $$ = new LogicAND($1,$3);   } 
			| Expr OR Expr											{  $$ = new LogicOR($1,$3);   } 
			| Expr QUES Expr COLON Expr								{  $$ = new TernaryCondition($1,$3,$5);   }
			| Expr ASSIGN Expr 										{  $$ = new DirectAssign($1,$3);   } 
			| Expr DIVEQ Expr 										{  $$ = new DivAssign($1,$3);   } 
			| Expr MULEQ Expr										{  $$ = new MulAssign($1,$3);   }  
			| Expr MODEQ Expr										{  $$ = new ModAssign($1,$3);   } 
			| Expr ADDEQ Expr										{  $$ = new AddAssign($1,$3);   } 
			| Expr SUBEQ Expr										{  $$ = new SubAssign($1,$3);   } 
			| Expr SHLEQ Expr										{  $$ = new SHLAssign($1,$3);   } 
			| Expr SHREQ Expr										{  $$ = new SHRAssign($1,$3);   } 
			| Expr BANDEQ Expr										{  $$ = new BitwiseANDAssign($1,$3);   } 
			| Expr BXOREQ Expr										{  $$ = new BitwiseXORAssign($1,$3);   } 
			| Expr BOREQ Expr										{  $$ = new BitwiseORAssign($1,$3);   }
			| LPAREN Expr RPAREN									{  $$ = $1;   }
			| IDENTIFIER											{  $$ = new Variable($1);   } 
			| Constant												{  $$ = $1;   }												
			;

ExprList:	_ExprList COMMA Expr									{  $$ = $1; $$->push_back($3);   }
			| Expr													{  $$ = new ExprList(); $$->push_back($1);   }
			|														{  $$ = new ExprList();   }
			;

_ExprList:	_ExprList COMMA Expr 									{  $$ = $1; $$->push_back($3);   }
			| Expr													{  $$ = new ExprList(); $$->push_back($1);   }
			;

Constant:	TRUE													{  $$ =  new Constant(true);   }
			| FALSE													{  $$ =  new Constant(false);   }
			| CHARACTER												{  $$ =  new Constant($1);   }
			| INTEGER 												{  $$ =  new Constant($1);   }
			| REAL													{  $$ =  new Constant($1);   }								
			;
%%