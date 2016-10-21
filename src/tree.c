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
	printf("Tree\n");
	if(globalTree->next == NULL) {
		printf("Null\n");
	}
	else {
		printf("root\n|\n");
		printDefList(globalTree->next);
	}
}
void printDefList(Def* d)
{
	Def* df = d->next;

	while(df!=NULL) {
		switch(d->tag) {
			case DVar:
				printf("defVar\n");
			break;
			case DFunc:
				printf("defFunc\n");
			break;
		}
		df = df->next;
	}
}
void printType(Type* t){

}
void printNameList(NameL* nl) {

}
void printDefVar(DefVar* dv){
	printType(dv->t);
	printNameList(dv->nl);
}
void printType(Type* t) {
	switch(t->tag) {
		case base:
		break;
		case array:
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

