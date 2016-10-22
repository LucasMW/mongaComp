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
  Parameter* param;
  void* exp;
  Type* type;
  NameL* namelist;
  CommandL* cmd;
  Block* block;


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



%type<prog> program constant  
%type<cmd> command command1 command2  
%type<block> block
%type <param> parameters parameter defVarList defVarList2
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
  $$ = (progNode*)malloc(sizeof(progNode));
  $$->next = $1;
  globalTree = $$;
}
;

definitionList: {
        //$$ = NULL; //printf(">>>>Null\n");
}
            | definitionList definition  {
              if(!$1)
                printf("1 null ");
              if(!$2)
                printf("2 null ");
              $$ = $2;
              $$->next = $1;
              // if($$->tag == DVar)
              //   $$->u.v = $2->u.v;
              // else {
              //   $$->u.f = $2->u.f;
              // }
              // $$->next = $1;
            }

;
definition : defVar {
  $$ = (Def*)malloc(sizeof(Def));
  $$->u.v = $1;
  $$->tag = DVar;
  //printDefVar($$->u.v);
}
| defFunc {
  $$ = (Def*)malloc(sizeof(Def));
  $$->u.f = $1;
  $$-> tag = DFunc;
}
;

defFunc : type ID '(' parameters ')' block {//printf("typed %s ",$2);
          $$ = (DefFunc*)malloc(sizeof(DefFunc));
          $$->id = $2;
          $$->retType = $1;
          $$->params = $4;

        }
        | TK_WVOID ID '(' parameters ')' block {//printf("untyped %s ",$2);
        $$ = (DefFunc*)malloc(sizeof(DefFunc));
        $$->id = $2;
        $$->retType = NULL;
        $$->params = $4;
      }

;

parameters :   {
          $$ = NULL;
}
        | parameter  { 
          $$=$1;
        }
        | parameter ',' parameters { 
          $$ =  $1;
          $$->next = $3;
        }

parameter : type ID {
    $$ = (Parameter*)malloc(sizeof(Parameter));
    $$->t = $1;
    $$->id = $2;
}
;
command : command1
;

defVar : type nameList ';' {  //correct
  printf("defVar\n");
  $$ = (DefVar*)malloc(sizeof(DefVar));
  $$->t = $1;
  $$->nl = $2;
  
}
;

nameList: ID idList {
  
    if($2==NULL)
   {
     $$ = (NameL*)malloc(sizeof(NameL));
     $$->name = $1;
     $$->next = NULL;
     printf("just one\n");
   }
   else {
     $$ = $2;
     $$->next = (NameL*)malloc(sizeof(NameL));
     ($$->next)->next = NULL;
     ($$->next)->name = $1;
     //printf("some\n");
   }
   //printf(" namelist: ");
   //printNameList($$);
   printf("\n");
}

idList: {$$ = NULL;}
    |idList2 { $$ = $1; //printf("ONE ID");
  }
idList2: ID { $$ = (NameL*)malloc(sizeof(NameL));
              $$->name = $1;
             $$->next = NULL;
             //printf("<nl null>");
           }
    | ',' ID idList {
      $$ = (NameL*)malloc(sizeof(NameL));
      $$->name = $2;
      $$->next = $3;
    }

block : '{'  defVarList   commandList  '}'
{
  $$ = (Block*) malloc (sizeof(Block));
};


defVarList : {
  $$ = NULL;
}
| defVarList2 {
  $$ = $1;
}

defVarList2: defVar defVarList {

}

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
ID: TK_VAR { $$=yyval.str_val;
  //printf("id:%s\n",$$);
}
;
type : baseType { $$ = (Type*)malloc(sizeof(Type));
                  $$->tag = base; 
                  //printf("base %d\n",$1); 
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



