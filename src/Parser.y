%{
#include "AST.hpp" 
#include <string>
#include <iostream>


void yyerror(const char *s) {
    std::printf("Error: %s\n", s);
    std::exit(1); 
}

int yylex(void);

AST::Program *Root;
%}

%output "Parser.cpp"

%union {
    int iVal;
    std::string *sVal;
    double dVal;
    char cVal;
	std::string *strVal;
    AST::Program *program;
    AST::Decl *decl;
    AST::Decls *decls; 
    AST::FuncDecl *funcDecl;
    AST::FuncBody *funcBody;
    AST::VarDecl *varDecl;
    AST::TypeDecl *typeDecl;
    AST::VarType *varType;
    AST::BuiltInType* builtInType;
    AST::FieldDecls* fieldDecls;
	AST::FieldDecl* fieldDecl;
	AST::MemList* memList;
    AST::Stmt *stmt;
    AST::IfStmt *ifStmt;
    AST::ForStmt *forStmt;
    AST::WhileStmt *whileStmt;
    AST::DoStmt *doStmt;
    AST::SwitchStmt *switchStmt;
    AST::CaseList *caseList;
    AST::CaseStmt *caseStmt;
    AST::BreakStmt* breakStmt;
	AST::ContinueStmt* continueStmt;
    AST::ReturnStmt *returnStmt;
    AST::Stmts *stmts;
    AST::Block *block;
    AST::Arg *arg;
    AST::ArgList *argList;
    AST::VarInit *varInit;
    AST::VarList *varList;
    AST::Expr *expr;
    AST::Constant* constant;
	AST::ExprList* exprList;
    AST::Enm *enm;
    AST::EnmList *enmList;
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
%token<strVal> STRING
%type<program>							Program	
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
%type<caseList>							CaseList
%type<caseStmt>							CaseStmt
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

%nonassoc IF
%nonassoc ELSE

%left	COMMA //15
%left	FUNC_CALL_ARG_LIST
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
Program:	Decls													{  std::cout << "Program -> Decls" << std::endl; $$ = new AST::Program($1); Root = $$;   }
			;
			
Decls:		Decls Decl												{  std::cout << "Decls -> Decls Decl" << std::endl; $$ = $1; $$->push_back($2);   }
			|														{  std::cout << "Decls -> e" << std::endl; $$ = new AST::Decls();   }
			;

Decl:		FuncDecl												{  std::cout << "Decl -> FuncDecl" << std::endl; $$ = $1;   }
			| VarDecl												{  std::cout << "Decl -> VarDecl" << std::endl; $$ = $1;   }
			| TypeDecl												{  std::cout << "Decl -> TypeDecl" << std::endl; $$ = $1;   }
			;

FuncDecl:	VarType IDENTIFIER LPAREN ArgList RPAREN SEMI 			{  std::cout << "FuncDecl -> VarType IDENTIFIER LPAREN ArgList RPAREN SEMI" << std::endl; $$ = new AST::FuncDecl($1,*$2,$4);   }
			| VarType IDENTIFIER LPAREN ArgList RPAREN FuncBody		{  std::cout << "FuncDecl -> VarType IDENTIFIER LPAREN ArgList RPAREN FuncBody" << std::endl; $$ = new AST::FuncDecl($1,*$2,$4,$6);   }
			;

FuncBody:	LBRACE Stmts RBRACE										{  std::cout << "FuncBody -> LBRACE Stmts RBRACE" << std::endl; $$ = new AST::FuncBody($2);   }
			;

VarDecl:	VarType VarList	SEMI   									{  std::cout << "VarDecl -> VarType VarList SEMI" << std::endl; $$ = new AST::VarDecl($1,$2);   }
			;

VarList:	_VarList COMMA VarInit									{  std::cout << "VarList -> _VarList COMMA VarInit" << std::endl; $$ = $1; $$->push_back($3);   }
			| VarInit												{  std::cout << "VarList -> VarInit" << std::endl; $$ = new AST::VarList(); $$->push_back($1);   }											
			|														{  std::cout << "VarList -> e" << std::endl; $$ = new AST::VarList();   }
			;

_VarList:	_VarList COMMA VarInit									{  std::cout << "VarList -> _VarList COMMA VarInit" << std::endl; $$ = $1; $$->push_back($3);   }
			| VarInit												{  std::cout << "VarList -> VarInit" << std::endl; $$ = new AST::VarList(); $$->push_back($1);   }	
			;

VarInit:	IDENTIFIER												{  std::cout << "VarInit -> IDENTIFIER" << std::endl; $$ = new AST::VarInit(*$1);   }
			| IDENTIFIER ASSIGN Constant								{  std::cout << "VarInit -> IDENTIFIER ASSIGN Expr" << std::endl; $$ = new AST::VarInit(*$1,$3);   }
			| IDENTIFIER ASSIGN Expr								{  std::cout << "VarInit -> IDENTIFIER ASSIGN Expr" << std::endl; $$ = new AST::VarInit(*$1,$3);   }
			;

TypeDecl:	TYPEDEF VarType IDENTIFIER	SEMI						{  std::cout << "TypeDecl -> TYPEDEF VarType IDENTIFIER SEMI" << std::endl; $$ = new AST::TypeDecl($2,*$3);   }
			;

VarType:	_VarType												{  std::cout << "VarType -> _VarType" << std::endl; $$ = $1;   }
			| CONST _VarType										{  $$ = $2; $$->SetConst();   }
			;

_VarType:	BuiltInType												{  std::cout << "_VarType -> BuiltInType" << std::endl; $$ = $1;   }
			| STRUCT LBRACE FieldDecls RBRACE						{  $$ = new AST::StructType($3);   }
			| ENUM LBRACE EnmList RBRACE							{  $$ = new AST::EnumType($3);   }
			| _VarType PTR											{  $$ = new AST::PointerType($1);   }
			| _VarType ARRAY LPAREN INTEGER RPAREN					{  $$ = new AST::ArrayType($1,$4);   }
			| IDENTIFIER											{  $$ = new AST::DefinedType(*$1);   }
			;
			
BuiltInType: BOOL													{  $$ = new AST::BuiltInType(AST::BuiltInType::TypeID::_Bool);   }
			| SHORT													{  $$ = new AST::BuiltInType(AST::BuiltInType::TypeID::_Short);   }
			| INT													{  std::cout << "BuiltInType -> INT" << std::endl; $$ = new AST::BuiltInType(AST::BuiltInType::TypeID::_Int);   }
			| LONG													{  $$ = new AST::BuiltInType(AST::BuiltInType::TypeID::_Long);   }
			| CHAR													{  $$ = new AST::BuiltInType(AST::BuiltInType::TypeID::_Char);   }
			| FLOAT													{  $$ = new AST::BuiltInType(AST::BuiltInType::TypeID::_Float);   }
			| DOUBLE												{  $$ = new AST::BuiltInType(AST::BuiltInType::TypeID::_Double);   }
			| VOID													{  $$ = new AST::BuiltInType(AST::BuiltInType::TypeID::_Void);   }
			;

FieldDecls:	FieldDecls FieldDecl            						{  $$ = $1; if ($2 != NULL) $$->push_back($2);   }
			|														{  $$ = new AST::FieldDecls();   }
			;

FieldDecl:	VarType MemList	SEMI    								{  $$ = new AST::FieldDecl($1,$2);   }
			| SEMI													{  $$ = NULL;   }
			;

MemList:	_MemList COMMA IDENTIFIER								{  $$ = $1; $$->push_back(*$3);   }
			| IDENTIFIER											{  $$ = new AST::MemList(); $$->push_back(*$1);   }	
			|														{  $$ = new AST::MemList();   }
			;

_MemList:	_MemList COMMA IDENTIFIER								{  $$ = $1; $$->push_back(*$3);   }
			| IDENTIFIER											{  $$ = new AST::MemList(); $$->push_back(*$1);   }
			;

EnmList:	_EnmList COMMA Enm										{  $$ = $1; $$->push_back($3);   }	
			| Enm													{  $$ = new AST::EnmList(); $$->push_back($1);   }
			|														{  $$ = new AST::EnmList();   }		
			;

_EnmList:	_EnmList COMMA Enm										{  $$ = $1; $$->push_back($3);   }	
			| Enm													{  $$ = new AST::EnmList(); $$->push_back($1);   }
			;

Enm:		IDENTIFIER												{  $$ = new AST::Enm(*$1);   }
			| IDENTIFIER ASSIGN INTEGER								{  $$ = new AST::Enm(*$1,true,$3);   };
			;

ArgList:	_ArgList COMMA Arg										{  $$ = $1; $$->push_back($3);   }	 
			| Arg													{  $$ = new AST::ArgList(); $$->push_back($1);   }
			|														{  $$ = new AST::ArgList();   }
			;

_ArgList:	_ArgList COMMA Arg										{  $$ = $1; $$->push_back($3);   }	 
			| Arg													{  $$ = new AST::ArgList(); $$->push_back($1);   }
			;

Arg:		VarType IDENTIFIER										{  $$ = new AST::Arg($1,*$2);   }
			| VarType												{  $$ = new AST::Arg($1);   }
			;

Block:		LBRACE Stmts RBRACE										{  $$ = new AST::Block($2);   }
			;

Stmts:		Stmts Stmt      										{  $$ = $1; if ($2 != NULL) $$->push_back($2);   }	
			|														{  $$ = new AST::Stmts();   }
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

IfStmt:		IF LPAREN Expr RPAREN Stmt ELSE Stmt					{  $$ = new AST::IfStmt($3,$5,$7);   }
			| IF LPAREN Expr RPAREN Stmt         					{  $$ = new AST::IfStmt($3,$5);   }
			;

ForStmt:	FOR LPAREN Expr SEMI Expr SEMI Expr RPAREN Stmt			{  $$ = new AST::ForStmt($3,$5,$7,$9);   }
            | FOR LPAREN VarDecl Expr SEMI Expr RPAREN Stmt			{  $$ = new AST::ForStmt($3,$4,$6,$8);   }
			;

WhileStmt:	WHILE LPAREN Expr RPAREN Stmt							{  $$ = new AST::WhileStmt($3,$5);   }
			;

DoStmt:		DO Stmt WHILE LPAREN Expr RPAREN						{  $$ = new AST::DoStmt($2,$5);   }
			;

SwitchStmt:	SWITCH LPAREN Expr RPAREN LBRACE CaseList RBRACE		{  $$ = new AST::SwitchStmt($3,$6);   }
			;

CaseList:	CaseList CaseStmt										{  $$ = $1; $$->push_back($2);   }	
			|														{  $$ = new AST::CaseList();   }
			;

CaseStmt:	CASE Expr COLON Stmts									{  $$ = new AST::CaseStmt($2,$4);   }
			| DEFAULT COLON Stmts									{  $$ = new AST::CaseStmt(NULL,$3);   }
			;


ContinueStmt:CONTINUE SEMI											{  $$ = new AST::ContinueStmt();   }
			;

BreakStmt:	BREAK SEMI												{  $$ = new AST::BreakStmt();   }
			;

ReturnStmt: RETURN SEMI 											{  $$ = new AST::ReturnStmt();   }
			| RETURN Expr SEMI										{  $$ = new AST::ReturnStmt($2);   }
			;

Expr:		Expr LBRACKET Expr RBRACKET %prec ARW					{  $$ = new AST::Subscript($1,$3);   }
			| SIZEOF LPAREN IDENTIFIER RPAREN						{  $$ = new AST::SizeOf(*$3);   }
			| SIZEOF LPAREN Expr RPAREN								{  $$ = new AST::SizeOf($3);   }
			| SIZEOF LPAREN VarType RPAREN							{  $$ = new AST::SizeOf($3);   }
			| IDENTIFIER LPAREN ExprList RPAREN						{  std::cout << "Expr -> IDENTIFIER LPAREN ExprList RPAREN" << std::endl; $$ = new AST::FunctionCall(*$1,$3);   }
			| Expr DOT IDENTIFIER									{  $$ = new AST::StructReference($1,*$3);   }
			| Expr ARW IDENTIFIER									{  $$ = new AST::StructDereference($1,*$3);   }
			| ADD Expr	%prec NOT									{  $$ = new AST::UnaryPlus($2);   }
			| SUB Expr	%prec NOT									{  $$ = new AST::UnaryMinus($2);   }
			| LPAREN VarType RPAREN Expr %prec NOT					{  $$ = new AST::TypeCast($2,$4);   }
			| DADD Expr	%prec NOT									{  $$ = new AST::PrefixInc($2);   }
			| Expr DADD %prec ARW									{  $$ = new AST::PostfixInc($1);   }
			| DSUB Expr %prec NOT									{  $$ = new AST::PrefixDec($2);   }
			| Expr DSUB	%prec ARW									{  $$ = new AST::PostfixDec($1);   }
			| MUL Expr	%prec NOT									{  $$ = new AST::Indirection($2);   }
			| BAND Expr	%prec NOT									{  $$ = new AST::AddressOf($2);   }
			| NOT Expr												{  $$ = new AST::LogicNot($2);   }
			| BNOT Expr												{  $$ = new AST::BitwiseNot($2);   }
			| Expr DIV Expr											{  $$ = new AST::Division($1,$3);   }
			| Expr MUL Expr											{  $$ = new AST::Multiplication($1,$3);   } 
			| Expr MOD Expr 										{  $$ = new AST::Modulo($1,$3);   }
			| Expr ADD Expr											{  $$ = new AST::Addition($1,$3);   } 
			| Expr SUB Expr											{  $$ = new AST::Subtraction($1,$3);   } 
			| Expr SHL Expr											{  $$ = new AST::LeftShift($1,$3);   } 
			| Expr SHR Expr											{  $$ = new AST::RightShift($1,$3);   } 
			| Expr GT Expr											{  $$ = new AST::LogicGT($1,$3);   } 
			| Expr GE Expr											{  $$ = new AST::LogicGE($1,$3);   } 
			| Expr LT Expr											{  $$ = new AST::LogicLT($1,$3);   } 
			| Expr LE Expr											{  $$ = new AST::LogicLE($1,$3);   } 
			| Expr EQ Expr											{  $$ = new AST::LogicEQ($1,$3);   } 
			| Expr NEQ Expr											{  $$ = new AST::LogicNEQ($1,$3);   } 
			| Expr BAND Expr										{  $$ = new AST::BitwiseAND($1,$3);   }
			| Expr BXOR Expr										{  $$ = new AST::BitwiseXOR($1,$3);   }
			| Expr BOR Expr											{  $$ = new AST::BitwiseOR($1,$3);   } 
			| Expr AND Expr											{  $$ = new AST::LogicAND($1,$3);   } 
			| Expr OR Expr											{  $$ = new AST::LogicOR($1,$3);   } 
			| Expr QUES Expr COLON Expr								{  $$ = new AST::TernaryCondition($1,$3,$5);   }
			| Expr ASSIGN Expr 										{  $$ = new AST::DirectAssign($1,$3);   } 
			| Expr DIVEQ Expr 										{  $$ = new AST::DivAssign($1,$3);   } 
			| Expr MULEQ Expr										{  $$ = new AST::MulAssign($1,$3);   }  
			| Expr MODEQ Expr										{  $$ = new AST::ModAssign($1,$3);   } 
			| Expr ADDEQ Expr										{  $$ = new AST::AddAssign($1,$3);   } 
			| Expr SUBEQ Expr										{  $$ = new AST::SubAssign($1,$3);   } 
			| Expr SHLEQ Expr										{  $$ = new AST::SHLAssign($1,$3);   } 
			| Expr SHREQ Expr										{  $$ = new AST::SHRAssign($1,$3);   } 
			| Expr BANDEQ Expr										{  $$ = new AST::BitwiseANDAssign($1,$3);   } 
			| Expr BXOREQ Expr										{  $$ = new AST::BitwiseXORAssign($1,$3);   } 
			| Expr BOREQ Expr										{  $$ = new AST::BitwiseORAssign($1,$3);   }
			| LPAREN Expr RPAREN									{  $$ = $2;   }
			| IDENTIFIER											{  $$ = new AST::Variable(*$1);   } 
			| Constant												{  std::cout << "Expr -> Constant" << std::endl; $$ = $1;   }												
			| Expr COMMA Expr										{  std::cout << "Expr -> Expr COMMA Expr" << std::endl; $$ = new AST::CommaExpr($1, $3);   }
			;

ExprList:	_ExprList COMMA Expr									{  std::cout << "ExprList -> _ExprList COMMA Expr" << std::endl; $$ = $1; $$->push_back($3);   }
			| Expr %prec FUNC_CALL_ARG_LIST							{  std::cout << "ExprList -> Expr" << std::endl; $$ = new AST::ExprList(); $$->push_back($1);   }
			|														{  std::cout << "ExprList -> e" << std::endl; $$ = new AST::ExprList();   }
			;

_ExprList:	_ExprList COMMA Expr 									{  std::cout << "_ExprList -> _ExprList COMMA Expr" << std::endl; $$ = $1; $$->push_back($3);   }
			| Expr %prec FUNC_CALL_ARG_LIST							{  std::cout << "_ExprList -> Expr" << std::endl; $$ = new AST::ExprList(); $$->push_back($1);   }
			;

Constant:	TRUE													{  $$ =  new AST::Constant(true);   }
			| FALSE													{  $$ =  new AST::Constant(false);   }
			| CHARACTER												{  std::cout << "Constant -> CHARACTER" << std::endl; $$ =  new AST::Constant($1);   }
			| INTEGER 												{  $$ =  new AST::Constant($1);   }
			| REAL													{  $$ =  new AST::Constant($1);   }
			| STRING												{  $$ =  new AST::GlobalString(*$1);   }
			;
%%