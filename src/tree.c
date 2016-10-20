// data Var = VarID ID
// 		| VarIndexed Exp Exp
// 		| ...
#if !defined(tree_h)
	#include "tree.h"
	#define tree_h
#endif
#include <stdio.h> 
#include <stdlib.h>

pNode* globalTree;

void printTree() {
	printf("Tree\n");
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

