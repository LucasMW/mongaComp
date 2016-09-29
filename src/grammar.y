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

%token	<int_val>	INTEGER_LITERAL
%token TK_GE

%type	<int_val>	exp
%left	PLUS
%left	MULT

%%

input:		/* empty */
		| exp	{ printf("Approved\n");  }
		;

exp:		INTEGER_LITERAL	{ $$ = $1; }
		| exp PLUS exp	{ $$ = $1 + $3; }
		| exp MULT exp	{ $$ = $1 * $3; }
		;

%%

int yyerror(char* s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  //cerr << "ERROR: " << s << " at symbol \"" << yytext;
  //cerr << "\" on line " << yylineno << endl;
  printf("Deu erro na linha %d \n",yylineno);
  exit(1);
}



