%{
#include "AST.hpp"
#include "Parser.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
extern "C" int yywrap() {return 1;}

char Escape2Char(char ch){
	switch(ch){
	case 'a': return '\a';
	case 'b': return '\b';
	case 'f': return '\f';
	case 'n': return '\n';
	case 'r': return '\r';
	case 't': return '\t';
	case 'v': return '\v';
	case '\\': return '\\';
	case '\'': return '\'';
	case '\"': return '\"';
	default:
		if ('0'<=ch && ch<='9')
			return (char)(ch-'0');
		else
			return ch;
	}
}
%}

%option never-interactive
%option outfile="Lexer.cpp"

%%
","														{return COMMA;}
"."														{return DOT;}
";"														{return SEMI;}
"("														{return LPAREN;}
")"														{return RPAREN;}
"["														{return LBRACKET;}
"]"														{return RBRACKET;}
"{"														{return LBRACE;}
"}"														{return RBRACE;}
"<<="													{return SHLEQ;}
"<<"													{return SHL;}
">>="													{return SHREQ;}
">>"													{return SHR;}
"=="													{return EQ;}
">="													{return GE;}
">"														{return GT;}
"<="													{return LE;}
"<"														{return LT;}
"!="													{return NEQ;}
"!"														{return NOT;}
"="														{return ASSIGN;}
"&&"													{return AND;}
"&="													{return BANDEQ;}
"&"														{return BAND;}
"||"													{return OR;}
"|="													{return BOREQ;}
"|"														{return BOR;}
"->"													{return ARW;}
"^="													{return BXOREQ;}
"^"														{return BXOR;}
"~"														{return BNOT;}
"++"													{return DADD;}
"+="													{return ADDEQ;}
"+"														{return ADD;}
"--"													{return DSUB;}
"-="													{return SUBEQ;}
"-"														{return SUB;}
"*="													{return MULEQ;}
"*"														{return MUL;}
"/="													{return DIVEQ;}
"/"														{return DIV;}
"%="													{return MODEQ;}
"%"														{return MOD;}
"?"														{return QUES;}
":"														{return COLON;}
"struct"												{return STRUCT;}
"typedef"												{return TYPEDEF;}
"const"													{return CONST;}
"enum"													{return ENUM;}
"ptr"													{return PTR;}
"array"													{return ARRAY;}
"if"													{return IF;}
"else"													{return ELSE;}
"for"													{return FOR;}
"while"													{return WHILE;}
"do"													{return DO;}
"switch"												{return SWITCH;}
"case"													{return CASE;}
"default"												{return DEFAULT;}
"continue"												{return CONTINUE;}
"return"												{return RETURN;}
"true"													{return TRUE; }
"false"													{return FALSE; }
"break"													{return BREAK; }
"sizeof"												{return SIZEOF; }
"bool"													{return BOOL; }
"short"													{return SHORT; }
"int"													{return INT; }
"long"													{return LONG; }
"char"													{return CHAR; }
"float"													{return FLOAT; }
"double"												{return DOUBLE; }
"void"													{return VOID; }
[ \t\n]													{ ; }
"/*"[^*]*[*]+([^*/][^*]*[*]+)*"/"						{ ; }
"//".*													{ ; }
"\'"\\."\'"												{ 
															yylval.cVal = Escape2Char(yytext[2]);
															return CHARACTER; 
														}
"\'"[^\\']"\'"											{ 
															yylval.cVal = yytext[1];
															return CHARACTER; 
														}
"\'"													{return SQUOTE; }
"\""(\\.|[^"\\])*"\""									{
															yylval.strVal = new std::string("");
															for (int i = 1; i <= yyleng-2; i++)
																if (yytext[i] == '\\'){
																	i++;
																	yylval.strVal->push_back(Escape2Char(yytext[i]));
																}else{
																	yylval.strVal->push_back(yytext[i]);
																}
															return STRING;
														}
"\""													{return DQUOTE;}
[a-zA-Z_][a-zA-Z0-9_]*									{ 
															yylval.sVal = new std::string(yytext, yyleng);
															return IDENTIFIER; 
														} 
[0-9]+\.[0-9]+											{ 
															double dtmp;
															sscanf(yytext, "%lf", &dtmp);
															yylval.dVal = dtmp;
															return REAL; 
														}
[0-9]+													{
															int itmp;
															sscanf(yytext, "%d", &itmp);
															yylval.iVal = itmp; 
															return INTEGER; 
														}
%%