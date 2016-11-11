#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

#if !defined(grammar_h)
	#include "grammar.h"
	#define grammar_h
#endif

typedef struct SymbolListNode
{
	char* symbol;
	struct SymbolListNode* next;
} SymbolListNode; 

typedef struct SymbolListStack {
	SymbolListNode **list;
	int size;
	int current;
} SymbolListStack;

static SymbolListStack symbolTable;

static char* variables[1000]; 
static int scopes[100]; 

static int scopesTop;
static int variablesTop;


void typeError() {
	printf("Typing error detected\n");
	exit(01);
}

void initSymbolTable() {
	scopesTop=0;
	variablesTop=0;
}
void enterScope() {
	scopesTop++;
	scopes[scopesTop] = variablesTop; 
}
void leaveScope() {
	scopesTop--;
}
int find(char * symbol) {
	int i;
	for(i=variablesTop;i>=0;i--) {
		if(strcmp(variables[i],symbol)==0) {
			return i;
		}
	}
	return -1;

}
int findCurrentScope(char * symbol) {
	int i;
	for(i=variablesTop;i>=scopes[scopesTop];i--) {
		if(strcmp(variables[i],symbol)==0) {
			return i;
		}
	}
	return -1;

}
void insert(char* symbol) {
	variables[variablesTop] = symbol;
	variablesTop++;
}
// typedef struct Var {
// 	VarTag tag;
// 	Type* type;
// 	union {
// 		union {
// 			const char* id;
// 			DecVar* dec;
// 		} name;
// 		struct{
// 			Exp *et, *ei;
// 		} indexed;
// 	} u;
	
// } Var;

int typeTree(progNode* p)
{
	return 0;
}
int typeDefList(Def* d)
{
	Def* df = d;
	while(df!=NULL) {
		switch(df->tag) {
			case DVar:
				typeVar(df->u.v);
			break;
			case DFunc:
				typeFunc(df->u.f);
			break;
		}
		df = df->next;
	}
}
int typeNameList(NameL* nl ) {
	if(!nl)
		return -1;
	NameL* p = nl;
	do  {
		//printDepthLevel(p->name);
		p = p->next;
	} while(p);
}
int typeDefVar(DefVar* dv ){
	
	if(!dv)
		return -1;
	typeNameList(dv->nl);
}
int typeDefFunc(DefFunc* df )
{
	if(!df)
		return -1;
	// printDepthLevel("DefFunc",x);
	// printDepthLevel(df->id );
	// printType(df->retType );
	typeParams(df->params );
	typeBlock(df->b );
}


int typeParams(Parameter* params )
{
	//printf("params\n");
	if(!params) {
		// printDepthLevel("None",x);
		return -1;
	}
	Parameter* p = params;
	while(p) {
		//printType(p->t,x);
		if(p->id)
			printf(p->id);
		p = p->next; 
	}
}
int typeBlock(Block* b ) {
	if(!b)
		return -1;
	//printDepthLevel("block{}",x);
	
	typeDefVarList(b->dvl );
	typeCommandList(b->cl );
}
int typeDefVarList(DefVarL* dvl ) {
	//printDepthLevel("DefVarL",x);
	if(!dvl)
		return -1;
	DefVarL* d = dvl;
	while(d){
		typeDefVar(d->dv );
		d = d->next;
	}
}
int typeCommandList(CommandL* cl ) {
	if(!cl)
		return -1;
	CommandL* c = cl;
	while(c) {
		switch(c->tag) {
			case CWhile:
				//printDepthLevel("While",x);
				typeExp(c->condExp );
				typeCommandList(c->cmdIf );
			break;
			case CIf:
				//printDepthLevel("If",x);
				typeExp(c->condExp );
				typeCommandList(c->cmdIf );
			break;
			case CIfElse:
				//printDepthLevel("if/else",x);
				typeExp(c->condExp );
				typeCommandList(c->cmdIf );
				typeCommandList(c->cmdElse );
			break;
			case CReturn:
				//printDepthLevel("return ",x);
				typeExp(c->retExp );
			break;
			case CAssign:
				//printDepthLevel("Assign",x);
				typeExp(c->expLeft );
				typeExp(c->expRight );
			break;
			case CBlock:
				//printDepthLevel("block ",x);
				typeBlock((Block*)c->block );
			break;
			case CCall:
				//printDepthLevel("call",x);
				typeExp(c->expRight );
			break;
		}
		c = c->next;
	}
}
int typeExp(Exp* e ) {
	if(!e)
		return -1;
	switch(e->tag) {
		case ExpAdd: 
			typeExp(e->bin.e1 );
			typeExp(e->bin.e2 );
		break;
		case ExpSub:
			typeExp(e->bin.e1 );
			typeExp(e->bin.e2 );
		break;
		case ExpMul:
			typeExp(e->bin.e1 );
			typeExp(e->bin.e2 );
		break;
		case ExpDiv:
			typeExp(e->bin.e1 );
			typeExp(e->bin.e2 );
		break;
		case ExpCall:
			typeExpList(e->call.expList);
		break;
		case ExpVar:
			typeVar(e->var);
		break;
		case ExpUnary:
			typeExp(e->unary.e);
		break;
		case ExpPrim:
			typeConstant(e->c);
		break;
		case ExpNew:
			//printDepthLevel("New",x);
			typeExp(e->eNew.e );
		break;
		case ExpCmp:
			// switch(e->cmp.op) {
			// 	case GT:
			// 		printDepthLevel(">",x);
			// 	break;
			// 	case GTE:
			// 		printDepthLevel(">=",x);
			// 	break;
			// 	case LS:
			// 		printDepthLevel("<",x);
			// 	break;
			// 	case LSE:
			// 		printDepthLevel("<=",x);
			// 	break;
			// 	case AND:
			// 		printDepthLevel("&&",x);
			// 	break;
			// 	case OR:
			// 		printDepthLevel("||",x);
			// 	break;
			// 	case EqEq:
			// 		printDepthLevel("==",x);
			// 	break;
			// }
			typeExp(e->cmp.e1);
			typeExp(e->cmp.e2);
		break;
		case ExpAccess:
			//printDepthLevel("[]");
			typeExp(e->access.varExp);
			typeExp(e->access.indExp);
		break;
	}
}
Type* typeOfConstant(Constant* c) {
	if(!c)
		return NULL;
	Type* t = (Type*)malloc(sizeof(Type));
	t->tag = base;
	switch(c->tag) {
		case KInt:
			t->b = WInt;
		break;
		case KFloat:
			t->b = WFloat;
		break;
		case KStr:
			t->tag = array;
			t->of = (Type*)malloc(sizeof(Type));
			t->of->tag = base;
			t->of->b = WChar;
		break;
	}
		return t;
}
Type* getTypeOfExp(Exp* e) {

	if(e->tag == ExpPrim)
		return  typeOfConstant(e->c);
	if(e->tag == ExpUnary) {

	}
	return NULL;
}



int typeExpList(ExpList* el ) {
	if(!el)
		return -1;
	ExpList *p = el;
	//printDepthLevel("expList",x);
	while(p) {
		typeExp(p->e);
		p = p->next;
	}
	return 0;

}
int typeConstant(Constant* c) {
	return 0;
}
int typeFunc(DefFunc* f) {
	return 0;
}
int typeVar(Var* v) {
	if(!v)
		return -1;
	return 0;
	// printDepthLevel("Var",x);
	// printDepthLevel(v->id,x+1);
}


