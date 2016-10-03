/* The grammar for monga*/
/* grammar.y */

%error-verbose /* instruct bison to generate verbose error messages*/

%{
#include "lex.h"
int yyerror(char *s);
int yylex(void);
#include <stdio.h>
#include <stdlib.h>
#define YYDEBUG 1
%}

%union{
  int		i;
  char*	s;
  double d;
}

%start program

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
%token TK_VAR
%token TK_STR


%%
program : definitionList
;

definitionList: 
            | definition definitionList
;
definition : defVar 
| defFunc
;

defFunc : type ID '(' parameters ')' block
        | TK_WVOID ID '(' parameters ')' block

;

parameters :  
        | parameter 
        | parameter ',' parameters

parameter : type ID 
;
command : command1
;

defVar : type nameList ';'
;

nameList: ID idList 

idList: 
    |idList2
idList2: ID
    | ',' ID idList

block : '{'  defVarList   commandList  '}'


defVarList :
| defVarList2

defVarList2: defVar defVarList

commandList: 
| commandList2

commandList2: command commandList

commandIF: TK_WIF '(' exp ')' command 

commandElse: TK_WIF '(' exp ')' command2 TK_WELSE command2

commandWhile: TK_WWHILE '(' exp ')' command

command1: TK_WRETURN ';'
        | TK_WRETURN exp ';'
        | expCall ';'
        | block
        | commandIF
        | commandElse
        | expVar '=' exp ';'
        | commandWhile

command2 : TK_WRETURN ';'
        | TK_WRETURN exp ';'
        | expCall ';'
        | block
        | commandElse

//exps
exp: expCall
;


expCall: TK_VAR '(' expList ')' 
      | expNew
;
expList: 
      | expList2
;
expList2: exp
      | exp ',' expList
;


expNew: TK_WNEW type '[' exp ']'
      | expCmp
;
type : baseType
    | type '[' ']'
;
baseType : TK_WINT 
| TK_WCHAR 
| TK_WFLOAT
;

expCmp: expCmp TK_EQEQ expAnd
      | expCmp TK_GE expAnd
      | expCmp TK_LE expAnd
      | expCmp '>' expAnd
      | expCmp '<' expAnd
      | expAnd
;
expAnd: expAnd TK_AND expOr
      | expOr
;
expOr: expOr TK_OR expAdd
      | expAdd
;
//arith
expAdd: expAdd '+' expMul
      | expAdd '-' expMul
      | expMul
;
expMul: expMul '*' expUnary
      | expMul '/' expUnary
      | expUnary

expUnary: '!' expVar
      | '-' expVar
      | expVar
;

expVar: expVar '[' exp ']'
      | ID
      | primary

primary: constant
      | '(' exp ')'
;
constant: TK_INT {//printf("%d %d\n",yyval.i,yy_lines);
}
      | TK_FLOAT
      | TK_STR
;
ID: TK_VAR {//printf("ID: %s\n",yylval.s);
}
;

%%

int yyerror(char* s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  printf("Syntax Error at token \"%s\" at line %d \n",yytext,yy_lines);
  //printf("yyval.s = %s\n",yylval.s);

  exit(1);
}



