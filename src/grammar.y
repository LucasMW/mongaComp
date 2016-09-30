/* The grammar for monga*/
/* grammar.y */

%error-verbose /* instruct bison to generate verbose error messages*/
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
/*
program : definition {printf("program\n");}
        | definition program
;
definition: defVar
          | defFunc

defVar : type nameList ';'
;
nameList : TK_VAR 
          | TK_VAR nameList {//not finished
}
;
type : baseType
    | type '[' ']'
;
baseType : TK_WINT | TK_WCHAR | TK_WFLOAT
;
defFunc : returnType TK_VAR '(' parameters ')' block
;
returnType : type 
          | TK_WVOID
;
parameters :  parameter
;
parameter : type TK_VAR
;

block : '{'  defVar   command  '}' {printf("block\n");}
;
command : TK_WIF '(' exp ')' command 
        | TK_WIF  TK_WELSE command 
        | TK_WWHILE '(' exp ')' command
        | var '=' exp ';'
        | TK_WRETURN ';'
        | TK_WRETURN exp ';'
        | call ';'
        | block
;
var : TK_VAR 
    | exp '[' ']'
;

exp : NUMERAL | TK_STR {printf("exp\n");}
  | var
  | '(' exp ')'
  | call
  | TK_WNEW type '[' exp ']'
  | '-' exp
  | exp '+' exp
  | exp '-' exp
  | exp '*' exp
  | exp '/' exp
  | exp TK_EQEQ exp
  | exp TK_LE exp
  | exp TK_GE exp
  | exp '<' exp
  | exp '>' exp
  | '!' exp
  | exp TK_AND exp
  | exp TK_OR exp
;


call : TK_VAR '(' expList ')' {printf("Function\n");}
;
expList: exp
        | expLis
;


NUMERAL: TK_INT
        | TK_FLOAT
;
*/

program : exp
        | command     
;
command : commandIF
        | block
;

defVar : type nameList ';'
;

nameList: ID ',' idList

idList: 
    |idList2
idList2: ID
    | ID ',' idList

block : '{'  defVarList   commandList  '}'


defVarList :
| defVarList2

defVarList2: defVar defVarList

commandList: 
| commandList2

commandList2: command commandList

commandIF: TK_WIF '(' exp ')' command 

commandElse: TK_WIF '(' exp ')' command TK_WELSE command

command

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
baseType : TK_WINT | TK_WCHAR | TK_WFLOAT
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
constant: TK_INT
      | TK_FLOAT
      | TK_STR
;
ID: TK_VAR
;

%%

int yyerror(char* s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  printf("Syntax Error at token \"%s\" at line %d \n",yytext,yylineno);
  exit(1);
}



