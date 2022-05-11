
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 ".\\Parser.y"

#include "AST.hpp" 
#include <string>
#include <iostream>


void yyerror(const char *s) {
    std::printf("Error: %s\n", s);
    std::exit(1); 
}

int yylex(void);

AST::Program *Root;


/* Line 189 of yacc.c  */
#line 90 "Parser.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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
     CHARACTER = 336,
     STRING = 337
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 19 ".\\Parser.y"

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



/* Line 214 of yacc.c  */
#line 252 "Parser.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 264 "Parser.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1800

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  83
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  152
/* YYNRULES -- Number of states.  */
#define YYNSTATES  272

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   337

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    11,    13,    15,    22,
      29,    33,    37,    41,    43,    44,    48,    50,    52,    56,
      60,    65,    67,    70,    72,    77,    82,    85,    91,    93,
      95,    97,    99,   101,   103,   105,   107,   109,   112,   113,
     117,   119,   123,   125,   126,   130,   132,   136,   138,   139,
     143,   145,   147,   151,   155,   157,   158,   162,   164,   167,
     169,   173,   176,   177,   180,   182,   184,   186,   188,   190,
     192,   194,   196,   198,   200,   202,   204,   212,   218,   228,
     237,   243,   250,   258,   261,   262,   267,   271,   274,   277,
     280,   284,   289,   294,   299,   304,   309,   313,   317,   320,
     323,   328,   331,   334,   337,   340,   343,   346,   349,   352,
     356,   360,   364,   368,   372,   376,   380,   384,   388,   392,
     396,   400,   404,   408,   412,   416,   420,   424,   430,   434,
     438,   442,   446,   450,   454,   458,   462,   466,   470,   474,
     478,   480,   482,   486,   488,   489,   493,   495,   497,   499,
     501,   503,   505
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      84,     0,    -1,    85,    -1,    85,    86,    -1,    -1,    87,
      -1,    89,    -1,    93,    -1,    94,    79,    10,   104,    11,
       7,    -1,    94,    79,    10,   104,    11,    88,    -1,    14,
     108,    15,    -1,    94,    90,     7,    -1,    91,     3,    92,
      -1,    92,    -1,    -1,    91,     3,    92,    -1,    92,    -1,
      79,    -1,    79,    27,   123,    -1,    79,    27,   120,    -1,
      51,    94,    79,     7,    -1,    95,    -1,    52,    95,    -1,
      96,    -1,    50,    14,    97,    15,    -1,    53,    14,   101,
      15,    -1,    95,    54,    -1,    95,    55,    10,    78,    11,
      -1,    79,    -1,    70,    -1,    71,    -1,    72,    -1,    73,
      -1,    74,    -1,    75,    -1,    76,    -1,    77,    -1,    97,
      98,    -1,    -1,    94,    99,     7,    -1,     7,    -1,   100,
       3,    79,    -1,    79,    -1,    -1,   100,     3,    79,    -1,
      79,    -1,   102,     3,   103,    -1,   103,    -1,    -1,   102,
       3,   103,    -1,   103,    -1,    79,    -1,    79,    27,    78,
      -1,   105,     3,   106,    -1,   106,    -1,    -1,   105,     3,
     106,    -1,   106,    -1,    94,    79,    -1,    94,    -1,    14,
     108,    15,    -1,   108,   109,    -1,    -1,   120,     7,    -1,
     110,    -1,   111,    -1,   112,    -1,   113,    -1,   114,    -1,
     118,    -1,   117,    -1,   119,    -1,   107,    -1,    89,    -1,
      93,    -1,     7,    -1,    56,    10,   120,    11,   109,    57,
     109,    -1,    56,    10,   120,    11,   109,    -1,    58,    10,
     120,     7,   120,     7,   120,    11,   109,    -1,    58,    10,
      89,   120,     7,   120,    11,   109,    -1,    59,    10,   120,
      11,   109,    -1,    60,   109,    59,    10,   120,    11,    -1,
      61,    10,   120,    11,    14,   115,    15,    -1,   115,   116,
      -1,    -1,    62,   120,     9,   108,    -1,    63,     9,   108,
      -1,    65,     7,    -1,    64,     7,    -1,    66,     7,    -1,
      66,   120,     7,    -1,   120,    12,   120,    13,    -1,    67,
      10,    79,    11,    -1,    67,    10,   120,    11,    -1,    67,
      10,    94,    11,    -1,    79,    10,   121,    11,    -1,   120,
       4,    79,    -1,   120,    34,    79,    -1,    40,   120,    -1,
      43,   120,    -1,    10,    94,    11,   120,    -1,    38,   120,
      -1,   120,    38,    -1,    41,   120,    -1,   120,    41,    -1,
      45,   120,    -1,    30,   120,    -1,    26,   120,    -1,    37,
     120,    -1,   120,    47,   120,    -1,   120,    45,   120,    -1,
     120,    49,   120,    -1,   120,    40,   120,    -1,   120,    43,
     120,    -1,   120,    17,   120,    -1,   120,    19,   120,    -1,
     120,    22,   120,    -1,   120,    21,   120,    -1,   120,    24,
     120,    -1,   120,    23,   120,    -1,   120,    20,   120,    -1,
     120,    25,   120,    -1,   120,    30,   120,    -1,   120,    36,
     120,    -1,   120,    33,   120,    -1,   120,    28,   120,    -1,
     120,    31,   120,    -1,   120,     8,   120,     9,   120,    -1,
     120,    27,   120,    -1,   120,    46,   120,    -1,   120,    44,
     120,    -1,   120,    48,   120,    -1,   120,    39,   120,    -1,
     120,    42,   120,    -1,   120,    16,   120,    -1,   120,    18,
     120,    -1,   120,    29,   120,    -1,   120,    35,   120,    -1,
     120,    32,   120,    -1,    10,   120,    11,    -1,    79,    -1,
     123,    -1,   122,     3,   120,    -1,   120,    -1,    -1,   122,
       3,   120,    -1,   120,    -1,    68,    -1,    69,    -1,    81,
      -1,    78,    -1,    80,    -1,    82,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   136,   136,   139,   140,   143,   144,   145,   148,   149,
     152,   155,   158,   159,   160,   163,   164,   167,   168,   169,
     172,   175,   176,   179,   180,   181,   182,   183,   184,   187,
     188,   189,   190,   191,   192,   193,   194,   197,   198,   201,
     202,   205,   206,   207,   210,   211,   214,   215,   216,   219,
     220,   223,   224,   227,   228,   229,   232,   233,   236,   237,
     240,   243,   244,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   262,   263,   266,   267,
     270,   273,   276,   279,   280,   283,   284,   288,   291,   294,
     295,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   351,   352,   353,   356,   357,   360,   361,   362,
     363,   364,   365
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "COMMA", "DOT", "SQUOTE", "DQUOTE",
  "SEMI", "QUES", "COLON", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET",
  "LBRACE", "RBRACE", "SHLEQ", "SHL", "SHREQ", "SHR", "EQ", "GE", "GT",
  "LE", "LT", "NEQ", "NOT", "ASSIGN", "AND", "BANDEQ", "BAND", "OR",
  "BOREQ", "BOR", "ARW", "BXOREQ", "BXOR", "BNOT", "DADD", "ADDEQ", "ADD",
  "DSUB", "SUBEQ", "SUB", "MULEQ", "MUL", "DIVEQ", "DIV", "MODEQ", "MOD",
  "STRUCT", "TYPEDEF", "CONST", "ENUM", "PTR", "ARRAY", "IF", "ELSE",
  "FOR", "WHILE", "DO", "SWITCH", "CASE", "DEFAULT", "BREAK", "CONTINUE",
  "RETURN", "SIZEOF", "TRUE", "FALSE", "BOOL", "SHORT", "INT", "LONG",
  "CHAR", "FLOAT", "DOUBLE", "VOID", "INTEGER", "IDENTIFIER", "REAL",
  "CHARACTER", "STRING", "$accept", "Program", "Decls", "Decl", "FuncDecl",
  "FuncBody", "VarDecl", "VarList", "_VarList", "VarInit", "TypeDecl",
  "VarType", "_VarType", "BuiltInType", "FieldDecls", "FieldDecl",
  "MemList", "_MemList", "EnmList", "_EnmList", "Enm", "ArgList",
  "_ArgList", "Arg", "Block", "Stmts", "Stmt", "IfStmt", "ForStmt",
  "WhileStmt", "DoStmt", "SwitchStmt", "CaseList", "CaseStmt",
  "ContinueStmt", "BreakStmt", "ReturnStmt", "Expr", "ExprList",
  "_ExprList", "Constant", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    83,    84,    85,    85,    86,    86,    86,    87,    87,
      88,    89,    90,    90,    90,    91,    91,    92,    92,    92,
      93,    94,    94,    95,    95,    95,    95,    95,    95,    96,
      96,    96,    96,    96,    96,    96,    96,    97,    97,    98,
      98,    99,    99,    99,   100,   100,   101,   101,   101,   102,
     102,   103,   103,   104,   104,   104,   105,   105,   106,   106,
     107,   108,   108,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   110,   110,   111,   111,
     112,   113,   114,   115,   115,   116,   116,   117,   118,   119,
     119,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   121,   121,   121,   122,   122,   123,   123,   123,
     123,   123,   123
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     1,     6,     6,
       3,     3,     3,     1,     0,     3,     1,     1,     3,     3,
       4,     1,     2,     1,     4,     4,     2,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     0,     3,
       1,     3,     1,     0,     3,     1,     3,     1,     0,     3,
       1,     1,     3,     3,     1,     0,     3,     1,     2,     1,
       3,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     7,     5,     9,     8,
       5,     6,     7,     2,     0,     4,     3,     2,     2,     2,
       3,     4,     4,     4,     4,     4,     3,     3,     2,     2,
       4,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     3,     1,     0,     3,     1,     1,     1,     1,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     2,     1,     0,     0,     0,     0,    29,    30,
      31,    32,    33,    34,    35,    36,    28,     3,     5,     6,
       7,    14,    21,    23,    38,     0,    22,    48,    17,     0,
       0,    13,    26,     0,     0,     0,    51,     0,     0,    47,
      55,     0,    11,     0,     0,    40,    24,    43,    37,    20,
       0,    25,     0,    59,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   147,   148,   150,
     140,   151,   149,   152,    19,   141,    17,    12,     0,    42,
       0,     0,    52,    46,    58,     0,     0,   140,     0,     0,
     141,   107,   106,   108,   101,    98,   103,    99,   105,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,   104,     0,     0,
       0,     0,     0,     0,     0,     0,    27,    39,     0,     8,
      62,     9,    53,     0,   139,   140,     0,     0,   143,     0,
       0,    96,     0,     0,   134,   114,   135,   115,   120,   117,
     116,   119,   118,   121,   128,   125,   136,   122,   126,   138,
     124,    97,   137,   123,   132,   112,   133,   113,   130,   110,
     129,   109,   131,   111,    41,     0,   100,    92,    94,    93,
      95,     0,     0,    91,    75,    62,    10,     0,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    14,    72,    61,
      64,    65,    66,    67,    68,    70,    69,    71,     0,   142,
     127,     0,     0,     0,     0,     0,     0,    88,    87,    89,
       0,    63,    60,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,    77,     0,     0,    80,
       0,    84,     0,     0,     0,    81,     0,    76,     0,     0,
      82,     0,     0,    83,    79,     0,     0,    62,    78,    62,
      86,    85
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    17,    18,   141,   205,    29,    30,    31,
     206,   207,    22,    23,    34,    48,    80,    81,    37,    38,
      39,    54,    55,    56,   208,   185,   209,   210,   211,   212,
     213,   214,   256,   263,   215,   216,   217,   218,   149,   150,
      90
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -169
static const yytype_int16 yypact[] =
{
    -169,    12,  1683,  -169,    16,  1711,  1721,    17,  -169,  -169,
    -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,
    -169,   -64,   -48,  -169,  -169,   -62,   -48,   -47,     1,    27,
      32,    34,  -169,    30,   523,    36,    14,    29,    43,    44,
    1711,   502,  -169,   -31,   -29,  -169,  -169,   -27,  -169,  -169,
     -25,  -169,   -47,   -24,    48,    51,    59,   411,   502,   502,
     502,   502,   502,   502,   502,   502,    78,  -169,  -169,  -169,
      93,  -169,  -169,  -169,  1429,     2,    80,   105,    94,   106,
     103,   110,  -169,   112,  -169,    22,  1711,     3,   107,   693,
    -169,     4,     4,     4,     4,     4,     4,     4,     4,   484,
     502,    38,   502,   502,   502,   502,   502,   502,   502,   502,
     502,   502,   502,   502,   502,   502,   502,   502,   502,   502,
     502,    40,   502,   502,  -169,   502,   502,  -169,   502,   502,
     502,   502,   502,   502,   502,   502,  -169,  -169,    42,  -169,
    -169,  -169,   119,   502,  -169,   101,   113,   739,   600,   114,
     120,  -169,   785,   831,  1429,   313,  1429,   313,  1703,   428,
     428,   428,   428,  1703,  1429,  1551,  1429,  1665,  1513,  1429,
    1589,  -169,  1429,  1627,  1429,   102,  1429,   102,  1429,     4,
    1429,     4,  1429,     4,   123,   186,     4,  -169,  -169,  -169,
    -169,   502,   502,  -169,  -169,  -169,  -169,   122,   124,   127,
     338,   128,   132,   134,    90,  -169,  -169,   -31,  -169,  -169,
    -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,   877,   647,
    1475,   262,   502,   411,   502,    70,   502,  -169,  -169,  -169,
     923,  -169,  -169,   969,   502,  1015,  1061,   136,  1107,  -169,
     338,  1153,   502,   338,   502,   130,    91,   502,  1199,  -169,
    1245,  -169,   338,  1291,   502,  -169,   -12,  -169,   338,  1337,
    -169,   502,   141,  -169,  -169,   338,  1383,  -169,  -169,  -169,
     338,   338
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -169,  -169,  -169,  -169,  -169,  -169,    -2,  -169,  -169,   118,
     160,    -1,   157,  -169,  -169,  -169,  -169,  -169,  -169,  -169,
     121,  -169,  -169,    88,  -169,  -168,   -98,  -169,  -169,  -169,
    -169,  -169,  -169,  -169,  -169,  -169,  -169,   -39,  -169,  -169,
     125
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -147
static const yytype_int16 yytable[] =
{
      19,    21,    74,   260,    25,   -18,    32,    33,   101,   -18,
     -28,    40,     3,   100,   -28,    28,   103,    35,    89,    91,
      92,    93,    94,    95,    96,    97,    98,   221,    41,   139,
      24,    27,    36,    47,    42,    43,   140,   -16,   121,    53,
      44,    50,   124,    49,    51,   127,    52,   -50,    76,    78,
     261,   262,    79,    82,    86,    84,    88,   -28,   -28,    85,
     147,   148,   -57,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   -28,   172,   173,    53,   174,   175,    99,   176,
     177,   178,   179,   180,   181,   182,   183,   229,   146,   270,
      57,   271,   225,   100,   186,   136,   101,    41,   -15,   -45,
     137,   100,   187,   138,   103,   -49,    58,   151,   143,   171,
      59,   184,   -56,   191,   188,   190,   -44,    60,    61,   237,
      62,    63,   222,    64,   223,    65,   121,   224,   226,   227,
     124,   228,   246,   127,   251,   249,   244,   131,   252,   133,
     267,   135,   219,   220,   257,   -28,   -28,    66,    67,    68,
     264,    77,    20,    26,     0,   230,    75,   268,    69,    70,
      71,    72,    73,    83,   142,     0,     0,     0,     0,     0,
       0,     0,     0,   233,   235,   236,     0,   238,     0,     0,
       0,     0,     0,   194,     0,   241,    57,     0,     0,     0,
     195,   196,     0,   248,     0,   250,     0,     0,   253,     0,
       0,     0,    58,     0,     0,   259,    59,     0,     0,     0,
       0,   234,   266,    60,    61,     0,    62,    63,     0,    64,
       0,    65,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,   197,     0,   198,   199,   200,   201,     0,     0,
     202,   203,   204,    66,    67,    68,     8,     9,    10,    11,
      12,    13,    14,    15,    69,    87,    71,    72,    73,   194,
       0,     0,    57,     0,     0,     0,   195,   232,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,    60,
      61,     0,    62,    63,     0,    64,     0,    65,     0,     0,
       0,     0,     4,     5,     6,     7,     0,   101,   197,     0,
     198,   199,   200,   201,     0,   103,   202,   203,   204,    66,
      67,    68,     8,     9,    10,    11,    12,    13,    14,    15,
      69,    87,    71,    72,    73,   194,     0,   121,    57,     0,
       0,   124,   195,   126,   127,     0,   129,     0,   131,     0,
     133,     0,   135,     0,    58,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,    60,    61,     0,    62,    63,
       0,    64,     0,    65,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,   197,     0,   198,   199,   200,   201,
       0,     0,   202,   203,   204,    66,    67,    68,     8,     9,
      10,    11,    12,    13,    14,    15,    69,    87,    71,    72,
      73,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,     0,     0,    58,     0,     0,
     103,    59,     0,     0,     0,   105,     0,   107,    60,    61,
       0,    62,    63,     0,    64,     0,    65,     0,     0,     0,
       0,     4,   121,     6,     7,     0,   124,     0,   126,   127,
       0,   129,     0,   131,     0,   133,     0,   135,    66,    67,
      68,     8,     9,    10,    11,    12,    13,    14,    15,    69,
      87,    71,    72,    73,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    57,     0,    59,     0,     0,     0,     0,     0,
       0,    60,    61,     0,    62,    63,     0,    64,    58,    65,
      45,     0,    59,     0,     4,     0,     6,     7,    46,    60,
      61,     0,    62,    63,     0,    64,     0,    65,     0,     0,
       0,    66,    67,    68,     8,     9,    10,    11,    12,    13,
      14,    15,    69,   145,    71,    72,    73,     0,     0,    66,
      67,    68,     0,     4,     0,     6,     7,     0,     0,     0,
      69,    70,    71,    72,    73,     0,     0,     0,     0,     0,
       0,     0,     0,     8,     9,    10,    11,    12,    13,    14,
      15,     0,    16,  -146,   101,     0,     0,     0,   102,     0,
       0,     0,   103,     0,     0,     0,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,     0,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,     0,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
    -145,   101,     0,     0,     0,   102,     0,     0,     0,   103,
       0,     0,     0,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,     0,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,     0,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   101,     0,     0,
       0,   102,     0,     0,   144,   103,     0,     0,     0,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,     0,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
       0,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   101,     0,     0,     0,   102,     0,     0,
     189,   103,     0,     0,     0,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,     0,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,     0,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   101,
       0,     0,     0,   102,   192,     0,     0,   103,     0,     0,
       0,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,     0,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   101,     0,     0,     0,   102,
       0,     0,     0,   103,   193,     0,     0,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,     0,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,     0,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   101,     0,     0,   231,   102,     0,     0,     0,   103,
       0,     0,     0,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,     0,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,     0,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   101,     0,     0,
     239,   102,     0,     0,     0,   103,     0,     0,     0,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,     0,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
       0,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   101,     0,     0,     0,   102,     0,     0,
     240,   103,     0,     0,     0,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,     0,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,     0,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   101,
       0,     0,   242,   102,     0,     0,     0,   103,     0,     0,
       0,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,     0,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   101,     0,     0,     0,   102,
       0,     0,   243,   103,     0,     0,     0,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,     0,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,     0,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   101,     0,     0,     0,   102,     0,     0,   245,   103,
       0,     0,     0,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,     0,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,     0,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   101,     0,     0,
     247,   102,     0,     0,     0,   103,     0,     0,     0,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,     0,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
       0,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   101,     0,     0,   254,   102,     0,     0,
       0,   103,     0,     0,     0,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,     0,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,     0,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   101,
       0,     0,     0,   102,     0,     0,   255,   103,     0,     0,
       0,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,     0,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   101,     0,     0,     0,   102,
       0,     0,   258,   103,     0,     0,     0,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,     0,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,     0,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   101,     0,     0,     0,   102,     0,     0,   265,   103,
       0,     0,     0,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,     0,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,     0,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   101,     0,     0,
       0,   102,   269,     0,     0,   103,     0,     0,     0,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,     0,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
       0,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   101,     0,     0,     0,   102,     0,     0,
       0,   103,     0,     0,     0,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,     0,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,     0,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   101,
       0,     0,     0,   102,     0,     0,     0,   103,     0,     0,
       0,     0,   105,     0,   107,   108,   109,   110,   111,   112,
     113,     0,     0,   115,     0,   117,   118,     0,   120,   121,
       0,   123,     0,   124,     0,   126,   127,   101,   129,     0,
     131,     0,   133,     0,   135,   103,     0,     0,     0,     0,
     105,     0,   107,   108,   109,   110,   111,   112,   113,     0,
       0,   115,     0,   117,     0,     0,   120,   121,     0,   123,
       0,   124,     0,   126,   127,   101,   129,     0,   131,     0,
     133,     0,   135,   103,     0,     0,     0,     0,   105,     0,
     107,   108,   109,   110,   111,   112,   113,     0,     0,     0,
       0,   117,     0,     0,   120,   121,     0,   123,     0,   124,
       0,   126,   127,   101,   129,     0,   131,     0,   133,     0,
     135,   103,     0,     0,     0,     0,   105,     0,   107,   108,
     109,   110,   111,   112,   113,     0,     0,     0,     0,   117,
       0,     0,     0,   121,     0,   123,     0,   124,     0,   126,
     127,   101,   129,     0,   131,     0,   133,     0,   135,   103,
       0,     0,     0,     0,   105,     0,   107,   108,   109,   110,
     111,   112,   113,     0,     0,     0,     0,   117,     0,     0,
       0,   121,     0,     0,     0,   124,     0,   126,   127,   101,
     129,     0,   131,     0,   133,     0,   135,   103,     0,     0,
       0,     0,   105,     0,   107,   108,   109,   110,   111,   112,
     113,     0,     0,     0,     0,     0,     0,     0,     0,   121,
       0,     0,     0,   124,     0,   126,   127,   101,   129,     0,
     131,     0,   133,     0,   135,   103,     0,     0,     0,     0,
     105,     0,   107,     0,   109,   110,   111,   112,     0,     0,
       0,     0,     0,     4,     5,     6,     7,   121,     0,     0,
       0,   124,     0,   126,   127,     0,   129,     0,   131,     0,
     133,     0,   135,     8,     9,    10,    11,    12,    13,    14,
      15,     4,    16,     6,     7,     0,     0,     0,     0,     0,
       0,     4,     0,     0,     7,     0,     0,     0,     0,     0,
       0,     8,     9,    10,    11,    12,    13,    14,    15,     0,
      16,     8,     9,    10,    11,    12,    13,    14,    15,     0,
      16
};

static const yytype_int16 yycheck[] =
{
       2,     2,    41,    15,     5,     3,    54,    55,     4,     7,
       7,    10,     0,    10,    11,    79,    12,    79,    57,    58,
      59,    60,    61,    62,    63,    64,    65,   195,    27,     7,
      14,    14,    79,    34,     7,     3,    14,     3,    34,    40,
      10,    27,    38,     7,    15,    41,     3,     3,    79,    78,
      62,    63,    79,    78,     3,    79,    57,    54,    55,    11,
      99,   100,     3,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,    79,   122,   123,    86,   125,   126,    10,   128,
     129,   130,   131,   132,   133,   134,   135,     7,    99,   267,
      10,   269,   200,    10,   143,    11,     4,    27,     3,     3,
       7,    10,    11,     3,    12,     3,    26,    79,    11,    79,
      30,    79,     3,     3,    11,    11,     3,    37,    38,    59,
      40,    41,    10,    43,    10,    45,    34,    10,    10,     7,
      38,     7,   240,    41,    14,   243,    10,    45,    57,    47,
       9,    49,   191,   192,   252,    54,    55,    67,    68,    69,
     258,    43,     2,     6,    -1,   204,    41,   265,    78,    79,
      80,    81,    82,    52,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   222,   223,   224,    -1,   226,    -1,    -1,
      -1,    -1,    -1,     7,    -1,   234,    10,    -1,    -1,    -1,
      14,    15,    -1,   242,    -1,   244,    -1,    -1,   247,    -1,
      -1,    -1,    26,    -1,    -1,   254,    30,    -1,    -1,    -1,
      -1,   223,   261,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    45,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    -1,    56,    -1,    58,    59,    60,    61,    -1,    -1,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,     7,
      -1,    -1,    10,    -1,    -1,    -1,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    45,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,     4,    56,    -1,
      58,    59,    60,    61,    -1,    12,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,     7,    -1,    34,    10,    -1,
      -1,    38,    14,    40,    41,    -1,    43,    -1,    45,    -1,
      47,    -1,    49,    -1,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    45,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    -1,    56,    -1,    58,    59,    60,    61,
      -1,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     4,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      12,    30,    -1,    -1,    -1,    17,    -1,    19,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    45,    -1,    -1,    -1,
      -1,    50,    34,    52,    53,    -1,    38,    -1,    40,    41,
      -1,    43,    -1,    45,    -1,    47,    -1,    49,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,    10,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    41,    -1,    43,    26,    45,
       7,    -1,    30,    -1,    50,    -1,    52,    53,    15,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    45,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    -1,    67,
      68,    69,    -1,    50,    -1,    52,    53,    -1,    -1,    -1,
      78,    79,    80,    81,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,     3,     4,    -1,    -1,    -1,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
       3,     4,    -1,    -1,    -1,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     4,    -1,    -1,
      -1,     8,    -1,    -1,    11,    12,    -1,    -1,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,     4,    -1,    -1,    -1,     8,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     4,
      -1,    -1,    -1,     8,     9,    -1,    -1,    12,    -1,    -1,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,     4,    -1,    -1,    -1,     8,
      -1,    -1,    -1,    12,    13,    -1,    -1,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     4,    -1,    -1,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     4,    -1,    -1,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,     4,    -1,    -1,    -1,     8,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     4,
      -1,    -1,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,     4,    -1,    -1,    -1,     8,
      -1,    -1,    11,    12,    -1,    -1,    -1,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     4,    -1,    -1,    -1,     8,    -1,    -1,    11,    12,
      -1,    -1,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     4,    -1,    -1,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,     4,    -1,    -1,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     4,
      -1,    -1,    -1,     8,    -1,    -1,    11,    12,    -1,    -1,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,     4,    -1,    -1,    -1,     8,
      -1,    -1,    11,    12,    -1,    -1,    -1,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     4,    -1,    -1,    -1,     8,    -1,    -1,    11,    12,
      -1,    -1,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     4,    -1,    -1,
      -1,     8,     9,    -1,    -1,    12,    -1,    -1,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,     4,    -1,    -1,    -1,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     4,
      -1,    -1,    -1,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    17,    -1,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    28,    -1,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    -1,    40,    41,     4,    43,    -1,
      45,    -1,    47,    -1,    49,    12,    -1,    -1,    -1,    -1,
      17,    -1,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    28,    -1,    30,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    40,    41,     4,    43,    -1,    45,    -1,
      47,    -1,    49,    12,    -1,    -1,    -1,    -1,    17,    -1,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    40,    41,     4,    43,    -1,    45,    -1,    47,    -1,
      49,    12,    -1,    -1,    -1,    -1,    17,    -1,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    34,    -1,    36,    -1,    38,    -1,    40,
      41,     4,    43,    -1,    45,    -1,    47,    -1,    49,    12,
      -1,    -1,    -1,    -1,    17,    -1,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    38,    -1,    40,    41,     4,
      43,    -1,    45,    -1,    47,    -1,    49,    12,    -1,    -1,
      -1,    -1,    17,    -1,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    38,    -1,    40,    41,     4,    43,    -1,
      45,    -1,    47,    -1,    49,    12,    -1,    -1,    -1,    -1,
      17,    -1,    19,    -1,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    50,    51,    52,    53,    34,    -1,    -1,
      -1,    38,    -1,    40,    41,    -1,    43,    -1,    45,    -1,
      47,    -1,    49,    70,    71,    72,    73,    74,    75,    76,
      77,    50,    79,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      79,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      79
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    84,    85,     0,    50,    51,    52,    53,    70,    71,
      72,    73,    74,    75,    76,    77,    79,    86,    87,    89,
      93,    94,    95,    96,    14,    94,    95,    14,    79,    90,
      91,    92,    54,    55,    97,    79,    79,   101,   102,   103,
      10,    27,     7,     3,    10,     7,    15,    94,    98,     7,
      27,    15,     3,    94,   104,   105,   106,    10,    26,    30,
      37,    38,    40,    41,    43,    45,    67,    68,    69,    78,
      79,    80,    81,    82,   120,   123,    79,    92,    78,    79,
      99,   100,    78,   103,    79,    11,     3,    79,    94,   120,
     123,   120,   120,   120,   120,   120,   120,   120,   120,    10,
      10,     4,     8,    12,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    11,     7,     3,     7,
      14,    88,   106,    11,    11,    79,    94,   120,   120,   121,
     122,    79,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,    79,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,    79,   108,   120,    11,    11,    11,
      11,     3,     9,    13,     7,    14,    15,    56,    58,    59,
      60,    61,    64,    65,    66,    89,    93,    94,   107,   109,
     110,   111,   112,   113,   114,   117,   118,   119,   120,   120,
     120,   108,    10,    10,    10,   109,    10,     7,     7,     7,
     120,     7,    15,   120,    89,   120,   120,    59,   120,     7,
      11,   120,     7,    11,    10,    11,   109,     7,   120,   109,
     120,    14,    57,   120,     7,    11,   115,   109,    11,   120,
      15,    62,    63,   116,   109,    11,   120,     9,   109,     9,
     108,   108
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 136 ".\\Parser.y"
    {  std::cout << "Program -> Decls" << std::endl; (yyval.program) = new AST::Program((yyvsp[(1) - (1)].decls)); Root = (yyval.program);   ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 139 ".\\Parser.y"
    {  std::cout << "Decls -> Decls Decl" << std::endl; (yyval.decls) = (yyvsp[(1) - (2)].decls); (yyval.decls)->push_back((yyvsp[(2) - (2)].decl));   ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 140 ".\\Parser.y"
    {  std::cout << "Decls -> e" << std::endl; (yyval.decls) = new AST::Decls();   ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 143 ".\\Parser.y"
    {  std::cout << "Decl -> FuncDecl" << std::endl; (yyval.decl) = (yyvsp[(1) - (1)].funcDecl);   ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 144 ".\\Parser.y"
    {  std::cout << "Decl -> VarDecl" << std::endl; (yyval.decl) = (yyvsp[(1) - (1)].varDecl);   ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 145 ".\\Parser.y"
    {  std::cout << "Decl -> TypeDecl" << std::endl; (yyval.decl) = (yyvsp[(1) - (1)].typeDecl);   ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 148 ".\\Parser.y"
    {  std::cout << "FuncDecl -> VarType IDENTIFIER LPAREN ArgList RPAREN SEMI" << std::endl; (yyval.funcDecl) = new AST::FuncDecl((yyvsp[(1) - (6)].varType),*(yyvsp[(2) - (6)].sVal),(yyvsp[(4) - (6)].argList));   ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 149 ".\\Parser.y"
    {  std::cout << "FuncDecl -> VarType IDENTIFIER LPAREN ArgList RPAREN FuncBody" << std::endl; (yyval.funcDecl) = new AST::FuncDecl((yyvsp[(1) - (6)].varType),*(yyvsp[(2) - (6)].sVal),(yyvsp[(4) - (6)].argList),(yyvsp[(6) - (6)].funcBody));   ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 152 ".\\Parser.y"
    {  std::cout << "FuncBody -> LBRACE Stmts RBRACE" << std::endl; (yyval.funcBody) = new AST::FuncBody((yyvsp[(2) - (3)].stmts));   ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 155 ".\\Parser.y"
    {  std::cout << "VarDecl -> VarType VarList SEMI" << std::endl; (yyval.varDecl) = new AST::VarDecl((yyvsp[(1) - (3)].varType),(yyvsp[(2) - (3)].varList));   ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 158 ".\\Parser.y"
    {  std::cout << "VarList -> _VarList COMMA VarInit" << std::endl; (yyval.varList) = (yyvsp[(1) - (3)].varList); (yyval.varList)->push_back((yyvsp[(3) - (3)].varInit));   ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 159 ".\\Parser.y"
    {  std::cout << "VarList -> VarInit" << std::endl; (yyval.varList) = new AST::VarList(); (yyval.varList)->push_back((yyvsp[(1) - (1)].varInit));   ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 160 ".\\Parser.y"
    {  std::cout << "VarList -> e" << std::endl; (yyval.varList) = new AST::VarList();   ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 163 ".\\Parser.y"
    {  std::cout << "VarList -> _VarList COMMA VarInit" << std::endl; (yyval.varList) = (yyvsp[(1) - (3)].varList); (yyval.varList)->push_back((yyvsp[(3) - (3)].varInit));   ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 164 ".\\Parser.y"
    {  std::cout << "VarList -> VarInit" << std::endl; (yyval.varList) = new AST::VarList(); (yyval.varList)->push_back((yyvsp[(1) - (1)].varInit));   ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 167 ".\\Parser.y"
    {  std::cout << "VarInit -> IDENTIFIER" << std::endl; (yyval.varInit) = new AST::VarInit(*(yyvsp[(1) - (1)].sVal));   ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 168 ".\\Parser.y"
    {  std::cout << "VarInit -> IDENTIFIER ASSIGN Expr" << std::endl; (yyval.varInit) = new AST::VarInit(*(yyvsp[(1) - (3)].sVal),(yyvsp[(3) - (3)].constant));   ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 169 ".\\Parser.y"
    {  std::cout << "VarInit -> IDENTIFIER ASSIGN Expr" << std::endl; (yyval.varInit) = new AST::VarInit(*(yyvsp[(1) - (3)].sVal),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 172 ".\\Parser.y"
    {  std::cout << "TypeDecl -> TYPEDEF VarType IDENTIFIER SEMI" << std::endl; (yyval.typeDecl) = new AST::TypeDecl((yyvsp[(2) - (4)].varType),*(yyvsp[(3) - (4)].sVal));   ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 175 ".\\Parser.y"
    {  std::cout << "VarType -> _VarType" << std::endl; (yyval.varType) = (yyvsp[(1) - (1)].varType);   ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 176 ".\\Parser.y"
    {  (yyval.varType) = (yyvsp[(2) - (2)].varType); (yyval.varType)->SetConst();   ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 179 ".\\Parser.y"
    {  std::cout << "_VarType -> BuiltInType" << std::endl; (yyval.varType) = (yyvsp[(1) - (1)].builtInType);   ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 180 ".\\Parser.y"
    {  (yyval.varType) = new AST::StructType((yyvsp[(3) - (4)].fieldDecls));   ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 181 ".\\Parser.y"
    {  (yyval.varType) = new AST::EnumType((yyvsp[(3) - (4)].enmList));   ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 182 ".\\Parser.y"
    {  (yyval.varType) = new AST::PointerType((yyvsp[(1) - (2)].varType));   ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 183 ".\\Parser.y"
    {  (yyval.varType) = new AST::ArrayType((yyvsp[(1) - (5)].varType),(yyvsp[(4) - (5)].iVal));   ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 184 ".\\Parser.y"
    {  (yyval.varType) = new AST::DefinedType(*(yyvsp[(1) - (1)].sVal));   ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 187 ".\\Parser.y"
    {  (yyval.builtInType) = new AST::BuiltInType(AST::BuiltInType::TypeID::_Bool);   ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 188 ".\\Parser.y"
    {  (yyval.builtInType) = new AST::BuiltInType(AST::BuiltInType::TypeID::_Short);   ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 189 ".\\Parser.y"
    {  std::cout << "BuiltInType -> INT" << std::endl; (yyval.builtInType) = new AST::BuiltInType(AST::BuiltInType::TypeID::_Int);   ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 190 ".\\Parser.y"
    {  (yyval.builtInType) = new AST::BuiltInType(AST::BuiltInType::TypeID::_Long);   ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 191 ".\\Parser.y"
    {  (yyval.builtInType) = new AST::BuiltInType(AST::BuiltInType::TypeID::_Char);   ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 192 ".\\Parser.y"
    {  (yyval.builtInType) = new AST::BuiltInType(AST::BuiltInType::TypeID::_Float);   ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 193 ".\\Parser.y"
    {  (yyval.builtInType) = new AST::BuiltInType(AST::BuiltInType::TypeID::_Double);   ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 194 ".\\Parser.y"
    {  (yyval.builtInType) = new AST::BuiltInType(AST::BuiltInType::TypeID::_Void);   ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 197 ".\\Parser.y"
    {  (yyval.fieldDecls) = (yyvsp[(1) - (2)].fieldDecls); if ((yyvsp[(2) - (2)].fieldDecl) != NULL) (yyval.fieldDecls)->push_back((yyvsp[(2) - (2)].fieldDecl));   ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 198 ".\\Parser.y"
    {  (yyval.fieldDecls) = new AST::FieldDecls();   ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 201 ".\\Parser.y"
    {  (yyval.fieldDecl) = new AST::FieldDecl((yyvsp[(1) - (3)].varType),(yyvsp[(2) - (3)].memList));   ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 202 ".\\Parser.y"
    {  (yyval.fieldDecl) = NULL;   ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 205 ".\\Parser.y"
    {  (yyval.memList) = (yyvsp[(1) - (3)].memList); (yyval.memList)->push_back(*(yyvsp[(3) - (3)].sVal));   ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 206 ".\\Parser.y"
    {  (yyval.memList) = new AST::MemList(); (yyval.memList)->push_back(*(yyvsp[(1) - (1)].sVal));   ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 207 ".\\Parser.y"
    {  (yyval.memList) = new AST::MemList();   ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 210 ".\\Parser.y"
    {  (yyval.memList) = (yyvsp[(1) - (3)].memList); (yyval.memList)->push_back(*(yyvsp[(3) - (3)].sVal));   ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 211 ".\\Parser.y"
    {  (yyval.memList) = new AST::MemList(); (yyval.memList)->push_back(*(yyvsp[(1) - (1)].sVal));   ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 214 ".\\Parser.y"
    {  (yyval.enmList) = (yyvsp[(1) - (3)].enmList); (yyval.enmList)->push_back((yyvsp[(3) - (3)].enm));   ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 215 ".\\Parser.y"
    {  (yyval.enmList) = new AST::EnmList(); (yyval.enmList)->push_back((yyvsp[(1) - (1)].enm));   ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 216 ".\\Parser.y"
    {  (yyval.enmList) = new AST::EnmList();   ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 219 ".\\Parser.y"
    {  (yyval.enmList) = (yyvsp[(1) - (3)].enmList); (yyval.enmList)->push_back((yyvsp[(3) - (3)].enm));   ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 220 ".\\Parser.y"
    {  (yyval.enmList) = new AST::EnmList(); (yyval.enmList)->push_back((yyvsp[(1) - (1)].enm));   ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 223 ".\\Parser.y"
    {  (yyval.enm) = new AST::Enm(*(yyvsp[(1) - (1)].sVal));   ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 224 ".\\Parser.y"
    {  (yyval.enm) = new AST::Enm(*(yyvsp[(1) - (3)].sVal),true,(yyvsp[(3) - (3)].iVal));   ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 227 ".\\Parser.y"
    {  (yyval.argList) = (yyvsp[(1) - (3)].argList); (yyval.argList)->push_back((yyvsp[(3) - (3)].arg));   ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 228 ".\\Parser.y"
    {  (yyval.argList) = new AST::ArgList(); (yyval.argList)->push_back((yyvsp[(1) - (1)].arg));   ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 229 ".\\Parser.y"
    {  (yyval.argList) = new AST::ArgList();   ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 232 ".\\Parser.y"
    {  (yyval.argList) = (yyvsp[(1) - (3)].argList); (yyval.argList)->push_back((yyvsp[(3) - (3)].arg));   ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 233 ".\\Parser.y"
    {  (yyval.argList) = new AST::ArgList(); (yyval.argList)->push_back((yyvsp[(1) - (1)].arg));   ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 236 ".\\Parser.y"
    {  (yyval.arg) = new AST::Arg((yyvsp[(1) - (2)].varType),*(yyvsp[(2) - (2)].sVal));   ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 237 ".\\Parser.y"
    {  (yyval.arg) = new AST::Arg((yyvsp[(1) - (1)].varType));   ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 240 ".\\Parser.y"
    {  (yyval.block) = new AST::Block((yyvsp[(2) - (3)].stmts));   ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 243 ".\\Parser.y"
    {  (yyval.stmts) = (yyvsp[(1) - (2)].stmts); if ((yyvsp[(2) - (2)].stmt) != NULL) (yyval.stmts)->push_back((yyvsp[(2) - (2)].stmt));   ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 244 ".\\Parser.y"
    {  (yyval.stmts) = new AST::Stmts();   ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 247 ".\\Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (2)].expr);   ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 248 ".\\Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].ifStmt);   ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 249 ".\\Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].forStmt);   ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 250 ".\\Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].whileStmt);   ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 251 ".\\Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].doStmt);   ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 252 ".\\Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].switchStmt);   ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 253 ".\\Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].breakStmt);   ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 254 ".\\Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].continueStmt);   ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 255 ".\\Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].returnStmt);   ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 256 ".\\Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].block);   ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 257 ".\\Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].varDecl);   ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 258 ".\\Parser.y"
    {  (yyval.stmt) = (yyvsp[(1) - (1)].typeDecl);   ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 259 ".\\Parser.y"
    {  (yyval.stmt) = NULL;   ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 262 ".\\Parser.y"
    {  (yyval.ifStmt) = new AST::IfStmt((yyvsp[(3) - (7)].expr),(yyvsp[(5) - (7)].stmt),(yyvsp[(7) - (7)].stmt));   ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 263 ".\\Parser.y"
    {  (yyval.ifStmt) = new AST::IfStmt((yyvsp[(3) - (5)].expr),(yyvsp[(5) - (5)].stmt));   ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 266 ".\\Parser.y"
    {  (yyval.forStmt) = new AST::ForStmt((yyvsp[(3) - (9)].expr),(yyvsp[(5) - (9)].expr),(yyvsp[(7) - (9)].expr),(yyvsp[(9) - (9)].stmt));   ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 267 ".\\Parser.y"
    {  (yyval.forStmt) = new AST::ForStmt((yyvsp[(3) - (8)].varDecl),(yyvsp[(4) - (8)].expr),(yyvsp[(6) - (8)].expr),(yyvsp[(8) - (8)].stmt));   ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 270 ".\\Parser.y"
    {  (yyval.whileStmt) = new AST::WhileStmt((yyvsp[(3) - (5)].expr),(yyvsp[(5) - (5)].stmt));   ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 273 ".\\Parser.y"
    {  (yyval.doStmt) = new AST::DoStmt((yyvsp[(2) - (6)].stmt),(yyvsp[(5) - (6)].expr));   ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 276 ".\\Parser.y"
    {  (yyval.switchStmt) = new AST::SwitchStmt((yyvsp[(3) - (7)].expr),(yyvsp[(6) - (7)].caseList));   ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 279 ".\\Parser.y"
    {  (yyval.caseList) = (yyvsp[(1) - (2)].caseList); (yyval.caseList)->push_back((yyvsp[(2) - (2)].caseStmt));   ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 280 ".\\Parser.y"
    {  (yyval.caseList) = new AST::CaseList();   ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 283 ".\\Parser.y"
    {  (yyval.caseStmt) = new AST::CaseStmt((yyvsp[(2) - (4)].expr),(yyvsp[(4) - (4)].stmts));   ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 284 ".\\Parser.y"
    {  (yyval.caseStmt) = new AST::CaseStmt(NULL,(yyvsp[(3) - (3)].stmts));   ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 288 ".\\Parser.y"
    {  (yyval.continueStmt) = new AST::ContinueStmt();   ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 291 ".\\Parser.y"
    {  (yyval.breakStmt) = new AST::BreakStmt();   ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 294 ".\\Parser.y"
    {  (yyval.returnStmt) = new AST::ReturnStmt();   ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 295 ".\\Parser.y"
    {  (yyval.returnStmt) = new AST::ReturnStmt((yyvsp[(2) - (3)].expr));   ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 298 ".\\Parser.y"
    {  (yyval.expr) = new AST::Subscript((yyvsp[(1) - (4)].expr),(yyvsp[(3) - (4)].expr));   ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 299 ".\\Parser.y"
    {  (yyval.expr) = new AST::SizeOf(*(yyvsp[(3) - (4)].sVal));   ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 300 ".\\Parser.y"
    {  (yyval.expr) = new AST::SizeOf((yyvsp[(3) - (4)].expr));   ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 301 ".\\Parser.y"
    {  (yyval.expr) = new AST::SizeOf((yyvsp[(3) - (4)].varType));   ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 302 ".\\Parser.y"
    {  (yyval.expr) = new AST::FunctionCall(*(yyvsp[(1) - (4)].sVal),(yyvsp[(3) - (4)].exprList));   ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 303 ".\\Parser.y"
    {  (yyval.expr) = new AST::StructReference((yyvsp[(1) - (3)].expr),*(yyvsp[(3) - (3)].sVal));   ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 304 ".\\Parser.y"
    {  (yyval.expr) = new AST::StructDereference((yyvsp[(1) - (3)].expr),*(yyvsp[(3) - (3)].sVal));   ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 305 ".\\Parser.y"
    {  (yyval.expr) = new AST::UnaryPlus((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 306 ".\\Parser.y"
    {  (yyval.expr) = new AST::UnaryMinus((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 307 ".\\Parser.y"
    {  (yyval.expr) = new AST::TypeCast((yyvsp[(2) - (4)].varType),(yyvsp[(4) - (4)].expr));   ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 308 ".\\Parser.y"
    {  (yyval.expr) = new AST::PrefixInc((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 309 ".\\Parser.y"
    {  (yyval.expr) = new AST::PostfixInc((yyvsp[(1) - (2)].expr));   ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 310 ".\\Parser.y"
    {  (yyval.expr) = new AST::PrefixDec((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 311 ".\\Parser.y"
    {  (yyval.expr) = new AST::PostfixDec((yyvsp[(1) - (2)].expr));   ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 312 ".\\Parser.y"
    {  (yyval.expr) = new AST::Indirection((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 313 ".\\Parser.y"
    {  (yyval.expr) = new AST::AddressOf((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 314 ".\\Parser.y"
    {  (yyval.expr) = new AST::LogicNot((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 315 ".\\Parser.y"
    {  (yyval.expr) = new AST::BitwiseNot((yyvsp[(2) - (2)].expr));   ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 316 ".\\Parser.y"
    {  (yyval.expr) = new AST::Division((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 317 ".\\Parser.y"
    {  (yyval.expr) = new AST::Multiplication((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 318 ".\\Parser.y"
    {  (yyval.expr) = new AST::Modulo((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 319 ".\\Parser.y"
    {  (yyval.expr) = new AST::Addition((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 320 ".\\Parser.y"
    {  (yyval.expr) = new AST::Subtraction((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 321 ".\\Parser.y"
    {  (yyval.expr) = new AST::LeftShift((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 322 ".\\Parser.y"
    {  (yyval.expr) = new AST::RightShift((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 323 ".\\Parser.y"
    {  (yyval.expr) = new AST::LogicGT((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 324 ".\\Parser.y"
    {  (yyval.expr) = new AST::LogicGE((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 325 ".\\Parser.y"
    {  (yyval.expr) = new AST::LogicLT((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 326 ".\\Parser.y"
    {  (yyval.expr) = new AST::LogicLE((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 327 ".\\Parser.y"
    {  (yyval.expr) = new AST::LogicEQ((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 328 ".\\Parser.y"
    {  (yyval.expr) = new AST::LogicNEQ((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 329 ".\\Parser.y"
    {  (yyval.expr) = new AST::BitwiseAND((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 330 ".\\Parser.y"
    {  (yyval.expr) = new AST::BitwiseXOR((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 331 ".\\Parser.y"
    {  (yyval.expr) = new AST::BitwiseOR((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 332 ".\\Parser.y"
    {  (yyval.expr) = new AST::LogicAND((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 333 ".\\Parser.y"
    {  (yyval.expr) = new AST::LogicOR((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 334 ".\\Parser.y"
    {  (yyval.expr) = new AST::TernaryCondition((yyvsp[(1) - (5)].expr),(yyvsp[(3) - (5)].expr),(yyvsp[(5) - (5)].expr));   ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 335 ".\\Parser.y"
    {  (yyval.expr) = new AST::DirectAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 336 ".\\Parser.y"
    {  (yyval.expr) = new AST::DivAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 337 ".\\Parser.y"
    {  (yyval.expr) = new AST::MulAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 338 ".\\Parser.y"
    {  (yyval.expr) = new AST::ModAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 339 ".\\Parser.y"
    {  (yyval.expr) = new AST::AddAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 340 ".\\Parser.y"
    {  (yyval.expr) = new AST::SubAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 341 ".\\Parser.y"
    {  (yyval.expr) = new AST::SHLAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 342 ".\\Parser.y"
    {  (yyval.expr) = new AST::SHRAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 343 ".\\Parser.y"
    {  (yyval.expr) = new AST::BitwiseANDAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 344 ".\\Parser.y"
    {  (yyval.expr) = new AST::BitwiseXORAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 345 ".\\Parser.y"
    {  (yyval.expr) = new AST::BitwiseORAssign((yyvsp[(1) - (3)].expr),(yyvsp[(3) - (3)].expr));   ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 346 ".\\Parser.y"
    {  (yyval.expr) = (yyvsp[(2) - (3)].expr);   ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 347 ".\\Parser.y"
    {  (yyval.expr) = new AST::Variable(*(yyvsp[(1) - (1)].sVal));   ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 348 ".\\Parser.y"
    {  std::cout << "Expr -> Constant" << std::endl; (yyval.expr) = (yyvsp[(1) - (1)].constant);   ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 351 ".\\Parser.y"
    {  (yyval.exprList) = (yyvsp[(1) - (3)].exprList); (yyval.exprList)->push_back((yyvsp[(3) - (3)].expr));   ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 352 ".\\Parser.y"
    {  (yyval.exprList) = new AST::ExprList(); (yyval.exprList)->push_back((yyvsp[(1) - (1)].expr));   ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 353 ".\\Parser.y"
    {  (yyval.exprList) = new AST::ExprList();   ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 356 ".\\Parser.y"
    {  (yyval.exprList) = (yyvsp[(1) - (3)].exprList); (yyval.exprList)->push_back((yyvsp[(3) - (3)].expr));   ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 357 ".\\Parser.y"
    {  (yyval.exprList) = new AST::ExprList(); (yyval.exprList)->push_back((yyvsp[(1) - (1)].expr));   ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 360 ".\\Parser.y"
    {  (yyval.constant) =  new AST::Constant(true);   ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 361 ".\\Parser.y"
    {  (yyval.constant) =  new AST::Constant(false);   ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 362 ".\\Parser.y"
    {  std::cout << "Constant -> CHARACTER" << std::endl; (yyval.constant) =  new AST::Constant((yyvsp[(1) - (1)].cVal));   ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 363 ".\\Parser.y"
    {  (yyval.constant) =  new AST::Constant((yyvsp[(1) - (1)].iVal));   ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 364 ".\\Parser.y"
    {  (yyval.constant) =  new AST::Constant((yyvsp[(1) - (1)].dVal));   ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 365 ".\\Parser.y"
    {  (yyval.constant) =  new AST::GlobalString(*(yyvsp[(1) - (1)].strVal));   ;}
    break;



/* Line 1455 of yacc.c  */
#line 3089 "Parser.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 367 ".\\Parser.y"

