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

typedef struct Symbol {
	const char* id;
	Type* type;
} Symbol;

static SymbolListStack symbolTable;

static Symbol variables[1000]; 
static int scopes[100]; 

static int scopesTop;
static int variablesTop;



void printSymbol(Symbol s);
Type* getTypeOfExp(Exp* e);
Type* typeOfConstant(Constant* c);
int typeEquals(Type* t1, Type* t2);
void typeError(const char* message) {
	printf("Typing error: %s\n",message);
	exit(01);
}

void initSymbolTable() {
	scopesTop=0;
	variablesTop=0;
}
void enterScope() {
	printf("enterScope\n");
	scopesTop++;
	scopes[scopesTop] = variablesTop; 
}
void leaveScope() {
	printf("leaveScope\n");
	scopesTop--;
}
int find(const char * symbol) {
	int i;
	for(i=variablesTop-1;i>=0;i--) {
		if(strcmp(variables[i].id,symbol)==0) {
			return i;
		}
	}
	return -1;

}
int findCurrentScope(const char * symbol) {
	int i;
	for(i=variablesTop-1;i>=scopes[scopesTop];i--) {
		if(strcmp(variables[i].id,symbol)==0) {
			return i;
		}
	}
	return -1;

}
void insert(const char* symbolID,Type* type) {
	printf("insert %s \n",symbolID);
	if(findCurrentScope(symbolID)>=0) {
		printf("--%s--\n", symbolID);
		typeError("Symbol was already declared in this scope");
	}
	variables[variablesTop].id = symbolID;
	variables[variablesTop].type = type;
	variablesTop++;
	printf("variablesTop %d\n",variablesTop );
}

void printSymbol(Symbol s) {
	printType(s.type,0);
	printf("symbol %s \n",s.id);
}
void debugScopes() {
	for(int i = variablesTop;i>=0;i--) {
		printSymbol(variables[i]);	
	}
}



void typeTree(progNode* p)
{
	typeDefList(p->next);
}
void typeDefList(Def* d)
{
	Def* df = d;
	while(df!=NULL) {
		switch(df->tag) {
			case DVar:
				typeDefVar(df->u.v);
			break;
			case DFunc:
				typeDefFunc(df->u.f);
			break;
		}
		df = df->next;
	}
}
void typeNameList(NameL* nl, Type* t) {
	if(!nl)
		return;
	NameL* p = nl;
	do  {
		insert(p->name,t);
		p = p->next;
	} while(p);
}
void typeDefVar(DefVar* dv ){
	
	if(!dv)
		return;
	typeNameList(dv->nl,dv->t);
	printf("nl\n");
}
void typeDefFunc(DefFunc* df )
{

	if(!df)
		return;
	// printDepthLevel("DefFunc",x);
	// printDepthLevel(df->id );
	// printType(df->retType );
	typeParams(df->params );
	typeBlock(df->b );
	printf("df\n");
}


void typeParams(Parameter* params )
{
	printf("params\n");
	if(!params) {
		// printDepthLevel("None",x);
		return;
	}
	Parameter* p = params;
	while(p) {
		//printType(p->t,x);
		if(p->id)
			printf("%s\n",p->id);
		p = p->next; 
	}
}
void typeBlock(Block* b ) {
	if(!b)
		return;
	//printDepthLevel("block{}",x);
	enterScope();
	typeDefVarList(b->dvl );
	typeCommandList(b->cl );
	leaveScope();
}
void typeDefVarList(DefVarL* dvl ) {
	//printDepthLevel("DefVarL",x);
	if(!dvl)
		return;
	DefVarL* d = dvl;
	while(d){
		typeDefVar(d->dv );
		d = d->next;
	}
}
void typeCommandList(CommandL* cl ) {
	if(!cl)
		return;
	printf("cl\n");
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
				typeExp(c->expLeft);
				typeExp(c->expRight);
				if(!typeEquals(c->expLeft->type,c->expRight->type)) {
					typeError("Assigment left type differs from right type");
				}
				
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
int typeEquals(Type* t1, Type* t2) {
	printType(t1,3);
	printf("vs\n");
	printType(t2,3);
	printf("\n");
	if(t1->tag == base && t1->b == t2->b) {
		return 1;
	}
	if(t1->tag == array && t2->tag == array) {
		return typeEquals(t1->of,t2->of);
	}
	return 0;
}
/* 
Em qualquer expressão, 
uma variável char tem seu valor 
automaticamente promovido para int.
*/
void promoteType(Exp* e) {	
	if(e->type->tag == base && e->type->b == WChar) {
		e->type->b = WInt;
	}
}
BType BTypeOfArith(Exp* e1,Exp *e2) {
	if(e1->type->b == WFloat || e2->type->b == WFloat)
		return WFloat;
	return WInt;
}
Type* arithType(Exp* e) {
	Type* t = (Type*)malloc(sizeof(Type));
	t->b = BTypeOfArith(e->bin.e1,e->bin.e2);
	return t;
}
int checkTypeArtih(Exp* left,Exp *right) {
	printf("\ncheckTypeArtih");
	printExp(left,0);
	printExp(right,0);
	if(left->type->tag == base && right->type->tag == base) {
		printf(":::");
		if(typeEquals(left->type,right->type)) {
			printf("s:s");
			return 1;
		}
		if(left->type->b == WChar || right->type->b == WChar) {

		}
	}
	return 0;
}
int checkTypeUnary(Exp* e) {
	Type* t = getTypeOfExp(e);
	return t->tag == base && t->b == WInt;
}
void typeExp(Exp* e ) {
	printf("exp\n");
	if(!e)
		return;
	switch(e->tag) {
		case ExpAdd: 
			typeExp(e->bin.e1 );
			typeExp(e->bin.e2 );
			if(!checkTypeArtih(e->bin.e1,e->bin.e2)) {
				printType(e->bin.e1->type,0);
				printType(e->bin.e2->type,0);
				typeError("Types in Add differs");
			}
			e->type = arithType(e);
		break;
		case ExpSub:
			typeExp(e->bin.e1 );
			typeExp(e->bin.e2 );
			if(!checkTypeArtih(e->bin.e1,e->bin.e2)){
				printType(e->bin.e1->type,0);
				printType(e->bin.e2->type,0);
				typeError("Types in Sub differs");
			}
			e->type = arithType(e);
		break;
		case ExpMul:
			typeExp(e->bin.e1 );
			typeExp(e->bin.e2 );
			printExp(e->bin.e1,0);
			printExp(e->bin.e2,0);
			if(!checkTypeArtih(e->bin.e1,e->bin.e2)) {
				
				printf("\n");
				typeError("Types in Mul differs");
			}
			e->type = arithType(e);
		break;
		case ExpDiv:
			typeExp(e->bin.e1 );
			typeExp(e->bin.e2 );
			if(!checkTypeArtih(e->bin.e1,e->bin.e2)) {
				printExp(e->bin.e1,0);
				printExp(e->bin.e2,0);
				typeError("Types in Div differs");
			}
			e->type = arithType(e);
		break;
		case ExpCall:
			typeExpList(e->call.expList);
		break;
		case ExpVar:
			typeVar(e->var);
			e->type = e->var->type;
		break;
		case ExpUnary:
			typeExp(e->unary.e);
		break;
		case ExpPrim:
			printType(e->type,10);
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
	printf("sd\n");
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
		return typeOfConstant(e->c);
	if(e->tag == ExpUnary) {

	}
	return NULL;
}



void typeExpList(ExpList* el ) {
	if(!el)
		return;
	ExpList *p = el;
	//printDepthLevel("expList",x);
	while(p) {
		typeExp(p->e);
		p = p->next;
	}
	return;

}

Type* typeOfVar(Var* v) {
	if(!v)
		return NULL;
	debugScopes();
	int index = findCurrentScope(v->id);

	if(index < 0) {
		printf("--var %s--\n",v->id);
		typeError("No such var in scope");
	}
	return variables[index].type;
	// printDepthLevel("Var",x);
	// printDepthLevel(v->id,x+1);
}
void typeVar(Var* v) {
	printf("%s\n",v->id );
	v->type = typeOfVar(v);
}





