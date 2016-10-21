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
#if !defined(tree_h)
  #include "tree.h"
  #define tree_h
   #endif
%}

%union{
  int		int_val;
  char*	str_val;
  double double_val;
  progNode* prog;
  Def* def;
  DefVar* dVar;
  DefFunc* dFunc;
  void* exp;
  Type* type;
  NameL* namelist;


}

%start program

%token <int_val> TK_GE
%token <int_val> TK_LE
%token <int_val> TK_INT
%token <double_val> TK_FLOAT
%token <int_val> TK_WCHAR
%token <int_val> TK_WELSE
%token <int_val> TK_WFLOAT
%token <int_val> TK_WIF
%token <int_val> TK_WINT
%token <int_val> TK_WNEW
%token <int_val> TK_WRETURN
%token <int_val> TK_WVOID
%token <int_val> TK_WWHILE
%token <int_val> TK_AND
%token <int_val> TK_OR
%token <int_val> TK_EQEQ
%token <str_val> TK_VAR
%token <str_val> TK_STR



%type<prog> program constant parameters parameter command command1 command2 defVarList defVarList2 
%type <def> definitionList  definition 
%type <dVar> defVar 
%type <dFunc> defFunc
%type <exp> expUnary expVar expAnd expCmp expOr expMul expAdd expCall expNew
%type <type> type;
%type <namelist> idList idList2 nameList
%type <int_val> baseType 
%type <str_val> ID

%%
program : definitionList  {
  globalTree = (progNode*)$1;
}
;

definitionList: {$$ = NULL;}
            | definition definitionList {
              //$2->next = $1;
              $$ = $2;
            }
;
definition : defVar {
  //$$->u.v = $1;
  //$$->tag = DVar;
}
| defFunc {
  //$$->u.f = $1;
  //$$ -> tag = DFunc;
}
;

defFunc : type ID '(' parameters ')' block {printf("typed %s ",$2);}
        | TK_WVOID ID '(' parameters ')' block {printf("untyped %s ",$2);}

;

parameters :   {
          //$$ = NULL;
}
        | parameter  { 
          //$$=$1;
        }
        | parameter ',' parameters { //$$ 
        }

parameter : type ID 
;
command : command1
;

defVar : type nameList ';' { 
  printf("defVar\n");
  $$ = (DefVar*)malloc(sizeof(DefVar));
  $$->t = $1;
  $$->nl = $2;
}
;

nameList: ID idList {
  printf("namelist\n");
   $$ = (NameL*)malloc(sizeof(NameL));
   $$->name = $1;
}

idList: {$$ = NULL;}
    |idList2 { $$ = $1;}
idList2: ID {$$->name = $1;
             $$->next = NULL;}
    | ',' ID idList {
      // $$->name = $2;
      // $$->next = $3;
    }

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
exp: expNew
;


expCall: TK_VAR '(' expList ')' 
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
expAdd: expAdd '+' expMul { //$$ = $1 + $2;
}
      | expAdd '-' expMul { //$$ = $1 - $2;
      }
      | expMul
;
expMul: expMul '*' expUnary
      | expMul '/' expUnary
      | expUnary

expUnary: '!' expVar { $$ = $2; }
      | '-' expVar {$$ = $2;}
      | expVar 
;

expVar: expVar '[' exp ']'
      | ID
      | primary
      | expCall

primary: constant
      | '(' exp ')'
;
constant: TK_INT  {     $$=makeConstant(KInt);
           }
      | TK_FLOAT  {//$$=(double)$1;
      }
      | TK_STR    {//$$=(char*)$1;
      }
;
ID: TK_VAR { //$$=$1;
}
;
type : baseType { $$ = (Type*)malloc(sizeof(Type));
                  $$->tag = base; 
                 $$->b = $1; }
    | type '[' ']' {
      $$ = (Type*)malloc(sizeof(Type)); 
      $$->tag =array;
      $$->of = $1;
    }
;
baseType : TK_WINT { $$ = WInt;}
| TK_WCHAR  { $$ = WChar;}
| TK_WFLOAT {$$ = WFloat;}
;
%%

int yyerror(char* s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  printf("Syntax Error at token \"%s\" at line %d \n",yytext,yy_lines);
  //printf("yyval.str_val = %s\n",yylval.str_val);
  exit(1);
}



