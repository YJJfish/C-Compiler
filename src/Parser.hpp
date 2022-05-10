
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     COMMA = 258,
     DOT = 259,
     SQUOTE = 260,
     DQUOTE = 261,
     SEMI = 262,
     QUES = 263,
     COLON = 264,
     LPAREN = 265,
     RPAREN = 266,
     LBRACKET = 267,
     RBRACKET = 268,
     LBRACE = 269,
     RBRACE = 270,
     SHLEQ = 271,
     SHL = 272,
     SHREQ = 273,
     SHR = 274,
     EQ = 275,
     GE = 276,
     GT = 277,
     LE = 278,
     LT = 279,
     NEQ = 280,
     NOT = 281,
     ASSIGN = 282,
     AND = 283,
     BANDEQ = 284,
     BAND = 285,
     OR = 286,
     BOREQ = 287,
     BOR = 288,
     ARW = 289,
     BXOREQ = 290,
     BXOR = 291,
     BNOT = 292,
     DADD = 293,
     ADDEQ = 294,
     ADD = 295,
     DSUB = 296,
     SUBEQ = 297,
     SUB = 298,
     MULEQ = 299,
     MUL = 300,
     DIVEQ = 301,
     DIV = 302,
     MODEQ = 303,
     MOD = 304,
     STRUCT = 305,
     TYPEDEF = 306,
     CONST = 307,
     ENUM = 308,
     PTR = 309,
     ARRAY = 310,
     IF = 311,
     ELSE = 312,
     FOR = 313,
     WHILE = 314,
     DO = 315,
     SWITCH = 316,
     CASE = 317,
     DEFAULT = 318,
     BREAK = 319,
     CONTINUE = 320,
     RETURN = 321,
     SIZEOF = 322,
     TRUE = 323,
     FALSE = 324,
     BOOL = 325,
     SHORT = 326,
     INT = 327,
     LONG = 328,
     CHAR = 329,
     FLOAT = 330,
     DOUBLE = 331,
     VOID = 332,
     INTEGER = 333,
     IDENTIFIER = 334,
     REAL = 335,
     CHARACTER = 336
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 19 ".\\Parser.y"

    int iVal;
    std::string *sVal;
    double dVal;
    char cVal;
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



/* Line 1676 of yacc.c  */
#line 176 "Parser.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


