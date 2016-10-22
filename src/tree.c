// data Var = VarID ID
// 		| VarIndexed Exp Exp
// 		| ...
#if !defined(tree_h)
	#include "tree.h"
	#define tree_h
#endif
#include <stdio.h> 
#include <stdlib.h>

progNode* globalTree;

void printTree() {
	printf("Tree");
	if(globalTree->next == NULL) {
		printf("is Null\n");
	}
	else {
		printf("\nroot\n|\n");
		printDefList(globalTree->next);
	}
}
void printDefList(Def* d)
{
	Def* df = d;
	printf("DEF LIST:\n");
	while(df!=NULL) {
		switch(df->tag) {
			case DVar:
				printf("defVar \t");
				printDefVar(df->u.v);
			break;
			case DFunc:
				printf("defFunc \t");
				printDefFunc(df->u.f);
			break;
		}
		df = df->next;
		printf("\n");
	}
}

void printNameList(NameL* nl) {
	if(!nl)
		return;
	NameL* p = nl;
	do  {
		printf("name %s ",p->name);
		p = p->next;
	} while(p);
}
void printDefVar(DefVar* dv){
	if(!dv)
		return;
	printType(dv->t);
	printNameList(dv->nl);
}
void printDefFunc(DefFunc* df)
{
	if(!df)
		return;
	printf("f <id : %s; rt: ", df->id);
	printType(df->retType);
	printf("; params: ");
	printParams(df->params);
	printf(">\n");
	printBlock(df->b);
}
void printType(Type* t) {
	if(!t) {
		printf("void ");
		return;
	}
	switch(t->tag) {
		case base:
			//printf("b: %d ",t->b);
			switch(t->b) {
				case WInt:
				printf("int ");
				break;
				case WFloat:
				printf("float ");
				break;
				case WChar:
				printf("char ");
				break;
			}
			
		break;
		case array:
			printf("array of ");
			printType(t->of);
		break;
	}
}

void printParams(Parameter* params)
{
	//printf("params\n");
	if(!params) {
		printf("None ");
		return;
	}
	Parameter* p = params;
	while(p) {
		printType(p->t);
		if(p->id)
			printf("%s ", p->id);
		p = p->next; 
	}
}
void printBlock(Block* b) {
	if(!b)
		return;

	printf("{ ");
	printDefVarList(b->dvl);
	printCommandList(b->cl);
	printf("} ");
}
void printDefVarList(DefVarL* dvl) {
	if(!dvl)
		return;
	printf("dvl");
	DefVarL* d = dvl;
	while(d){
		printDefVar(dvl->dv);
		d = d->next;
	}
}
void printCommandList(CommandL* cl) {
	if(!cl)
		return;
	printf("cl");
	CommandL* c = cl;
	while(c) {
		switch(c->tag) {
			case CWhile:
				printf("While");
			break;
			case CIf:
				printf("If");
			break;
			case CIfElse:
				printf("if/else");
			break;
			case CReturn:
				printf("return");
				printExp(c->retExp);
			break;
			case CAssign:
				printf("Assign");
				printExp(c->expLeft);
				printExp(c->expRight);
			break;
			case CBlock:
				printf("block");
			break;
			case CCall:
				printf("call");
			break;
		}
		c = c->next;
	}
}
void printExp(Exp* e) {
	if(!e)
		return;
	switch(e->tag) {
		case ExpAdd: 
			printf("expAdd ");
		break;
		case ExpSub:
			printf("expSub ");
		break;
		case ExpMul:
			printf("expMul ");
		break;
		case ExpDiv:
			printf("expDiv ");
		break;
		case ExpCall:
			printf("expCall ");
		break;
		case ExpRet:
			printf("expRet ");
		break;
		case ExpOr:
			printf("expOr ");
		break;
		case ExpAnd:
			printf("expAnd ");
		break;
		case ExpEq:
			printf("expEq ");
		break;
		case ExpVar:
			printf("expVar ");
		break;
		case ExpNot:
			printf("expNot ");
		break;
		case ExpPrim:
			printf("expPrim ");
		break;
	}

}
void notConst()
{

}
void minusConst()
{

}
void makeID()
{
	
}

Constant* makeConstant(constantType t)
{
	Constant* k = (Constant*)malloc(sizeof(Constant));
	k->tag = t;
	return k;
}

// typedef enum VarE {VarId,VarIndexed} VarE;

// typedef struct Var {
// 	VarE tag;
// 	union {
// 		const char* id;
// 		struct{
// 			struct Exp *e1, *e2;
// 		} indexed;
// 	} e;
// } Var;

// // -----------------------------------------
// // expVar: expVar '[' exp ']'
// //       | ID
// //       | primary
// //       | expCall

// typedef enum ExpVar {VarId,VarIndexed,VarPrimary} ExpVar;
// typedef struct ExpVar
// {
// 	ExpVar tag;
// 	union {
// 		const char* id;
// 		struct {
// 			struct Exp *e1, e2;
// 		} indexed;
// 	} e;
// 	Primary p;
// 	Call c;
// } ExpVar ;


// typedef enum Exp E {
// 	ExpAdd,
// 	ExpSub,
// 	ExpMul,
// 	ExpCall,
// 	ExpNew,
// 	ExpCmp,
// 	.
// 	.
// } ExpE;

// struct Exp {
// 	ExpE tag;
// 	union {
// 		struct {
// 			Exp *e1, *e2;
// 		} bin;
// 		int ki;
// 		Var *var;
// 		.
// 		.
// 		.
// 	}
// };

