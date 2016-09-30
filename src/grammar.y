/* The grammar for monga*/
/* grammar.y */

%{
#include "lex.h"
int yyerror(char *s);
int yylex(void);
#include <stdio.h>
#include <stdlib.h>
%}

%union{
  int		int_val;
  char*	op_val;
}

%start	input 

%token TK_GE
%token TK_LE
%token TK_INT
%token TK_FLOAT
%token TK_WCHAR
%token TK_WELSE
%token TK_WFLOAT
%token TK_WIF
%token TK_WINT
%token TK_WNEW
%token TK_WRETURN
%token TK_WVOID
%token TK_WWHILE
%token TK_AND
%token TK_OR
%token TK_EQEQ


%%

input:		/* empty */
		| exp	{   }
		;
exp: expAdd
  | expMul
  | primary
expAdd: exp '+' exp 
;
primary:
  constant
| '(' exp ')'
;
constant: TK_INT
        | TK_FLOAT
expMul: exp '*' exp


%%

int yyerror(char* s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  printf("Syntax Error at token \"%s\" at line %d \n",yytext,yylineno);
  exit(1);
}



