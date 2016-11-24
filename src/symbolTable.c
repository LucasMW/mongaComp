#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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
	void* declaration;

} Symbol;

//static SymbolListStack symbolTable;

static Symbol variables[1000]; 
static int scopes[100]; 

static int scopesTop;
static int variablesTop;

static DefFunc* currentFunction = NULL;


void printSymbol(Symbol s);
Type* getTypeOfExp(Exp* e);
Type* typeOfConstant(Constant* c);
int typeEquals(Type* t1, Type* t2);
int checkPrintability(Exp* e);
void typeError(const char* message) {
	printf("Typing error: %s\n",message);
	exit(01);
}
void generateStardardDeclares(progNode* prog) {
	findFuncInTree("")
}
static Def* expandDefVar(Def* d) {
	DefVar* dv = d->u.v;
	NameL* nl = dv->nl;
	Def* dl = d;
	while(nl) {
		//printf("%s\n", nl->name);
		Def* nDef = (Def*)malloc(sizeof(Def));
		nDef->tag = DVar;
		DefVar* ndv = (DefVar*)malloc(sizeof(DefVar));
		char* name = (char*)malloc(strlen(nl->name)+1);
		name[strlen(nl->name)] = '\0';
		strcpy(name,nl->name);
		ndv->id = name;
		assert(ndv->id!=NULL);
		ndv->t = dv->t; //same type
		nDef->u.v = ndv;
		dl->next = nDef;
		dl = dl->next;
		nl = nl->next;
	}
	dl->next = d->next;
	return d;
}
static DefVarL* expandNameListIntoDefVarL(NameL * nl,Type* t) {
	if(nl==NULL) {
		return NULL;
	}
	DefVarL * dvl;

		dvl = (DefVarL*)malloc(sizeof(DefVarL));
		DefVar* dv = (DefVar*)malloc(sizeof(DefVar));
		dv->scope = VLocal;
		char* name = (char*)malloc(strlen(nl->name)+1);
		name[strlen(nl->name)] = '\0';
		strcpy(name,nl->name);
		dv->id = name;
		dv->t = t;
		dv->nl = NULL;
		dvl->dv = dv;
		dvl->next = expandNameListIntoDefVarL(nl->next,t);
	return dvl;
	

}
static DefVarL* expandDefVarL(DefVarL* dvl) {
	// DefVar* dv = dvl->dv;
	// NameL* nl = dv->nl;
	// DefVarL* rdvl = dvl;
	// dv->id = "NENHUM";
	// while(nl) {
	// 	printf("%s\n", nl->name);
	// 	printf("%s nl\n",nl->name );
	// 	DefVarL* ndvl = (DefVarL*)malloc(sizeof(DefVarL));

	// 	DefVar* ndv = (DefVar*)malloc(sizeof(DefVar));
	// 	char* name = (char*)malloc(strlen(nl->name)+1);
	// 	name[strlen(nl->name)] = '\0';
	// 	strcpy(name,nl->name);
	// 	ndv->id = name;
	// 	assert(ndv->id!=NULL);
	// 	ndv->t = dv->t; //same type
	// 	ndvl->dv = ndv;
	// 	dl->next = ndvl;
	// 	dl = dl->next;
	// 	nl = nl->next;
	// }
	// dl->next = d->next;

	// return d;
	DefVarL* internalList = expandNameListIntoDefVarL(dvl->dv->nl,dvl->dv->t);
	DefVarL* p = internalList;
	while(p->next !=NULL) {
		p = p->next;
	}
	p->next = dvl->next;
	dvl->next = internalList;
	return dvl;
}

Parameter* findParamsOfFunc(const char* funcId) {
	Def* dfl = globalTree->next;
	while(dfl) {
		if(dfl->tag == DFunc) {
			DefFunc* df = dfl->u.f;
			if(strcmp(funcId,df->id)==0) {
				return df->params;
			}
		}
		dfl = dfl->next;
	}
	return NULL;

}
DefFunc* findFuncInTree(const char* funcId) {
	Def* dfl = globalTree->next;
	while(dfl) {
		if(dfl->tag == DFunc) {
			DefFunc* df = dfl->u.f;
			if(strcmp(funcId,df->id)==0) {
				return df;
			}
		}
		dfl = dfl->next;
	}
	return NULL;

}
// Parameter* findTypeOfFunc(const char* funcId) {
// 	Def* dfl = globalTree->next;
// 	while(dfl) {
// 		if(dfl->tag == DFunc) {
// 			DefFunc* df = dfl->u.f;
// 			if(strcmp(funcId,df->id)==0) {
// 				return df->retType;
// 			}
// 		}
// 		dfl = dfl->next;
// 	}
// 	return NULL;

// }
void printSymbol(Symbol s) {
	printType(s.type,0);
	printf("symbol %s \n",s.id);
}
void debugScopes() {
	for(int j = scopesTop-1;j>=0;j--) {
		printf(":::scope %d:::",j);
		for(int i = variablesTop-1;i>=0;i--) {
			printSymbol(variables[i]);	
		}
		printf("\n:::end %d:::\n",j);
	}
}

void initSymbolTable() {
	scopesTop=0;
	variablesTop=0;
	scopes[0] = 0;
	currentFunction = NULL;
}

void enterScope() {
	//printf("enterScope %d\n",scopesTop+1);
	scopes[scopesTop] = variablesTop; 
	scopesTop++;
	//scopes[scopesTop] = variablesTop; 
	//debugScopes();
}
void leaveScope() {
	//printf("leaveScope %d\n",scopesTop);
	scopesTop--;
	variablesTop = scopes[scopesTop];
	//debugScopes();
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
void insert(const char* symbolID,Type* type,void* d) {
	//printf("insert %s \n",symbolID);
	if(findCurrentScope(symbolID)>=0) {
		printf("--%s--\n", symbolID);
		typeError("Symbol was already declared in this scope");
	}
	variables[variablesTop].id = symbolID;
	variables[variablesTop].type = type;
	variables[variablesTop].declaration = d;
	variablesTop++;
	//printf("variablesTop %d\n",variablesTop );
}





void typeTree(progNode* p)
{
	typeDefList(p->next);
}
void typeDefList(Def* d)
{
	Def* df = d;
	Def* ndf;
	while(df!=NULL) {
		switch(df->tag) {
			case DVar:
				// ndf = expandDefVar(d);
				// df = ndf;
				typeDefVar(df->u.v);
			break;
			case DFunc:
				typeDefFunc(df->u.f);
			break;
		}
		df = df->next;
	}
}
void typeNameList(NameL* nl, Type* t,DefVar* dv) {
	if(!nl)
		return;
	NameL* p = nl;
	do  {
		insert(p->name,t,dv);
		p = p->next;
	} while(p);
}
void typeDefVar(DefVar* dv){
	
	if(!dv)
		return;
	dv->scope = scopesTop;
	typeNameList(dv->nl,dv->t,dv);
}
void typeDefFunc(DefFunc* df)
{
	if(!df)
		return;
	// printType(df->retType );
	currentFunction = df;
	insert(df->id,df->retType,df);
	enterScope();
	typeParams(df->params );
	typeBlock(df->b );
	leaveScope();
	currentFunction = NULL;
	//printf("end df %s\n",df->id);
}


void typeParams(Parameter* params )
{
	if(!params) {
		return;
	}
	Parameter* p = params;
	while(p) {
		insert(p->id,p->t,NULL);
		p = p->next; 
	}
}
void typeBlock(Block* b ) {
	DefVarL* ndvl;
	if(!b)
		return;
	// ndvl = expandDefVarL(b->dvl);
	// b->dvl = ndvl;
	typeDefVarList(b->dvl );
	typeCommandList(b->cl );
}
void typeDefVarList(DefVarL* dvl ) {
	if(!dvl)
		return;
	DefVarL* d = dvl;
	while(d){
		typeDefVar(d->dv );
		d = d->next;
	}
}
int checkTypeReturn(Exp* retExp, DefFunc* df) {
	Type* t = NULL;
	if(retExp)
		t = retExp->type; 
	Type* ft = df->retType;
	return typeEquals(t,ft);
}
int checkCallability(Exp* callExp) {
	//printf("wsw\n");
	if(!callExp) {
		return 0; //cannot call null
	}
	Type* t = callExp->type;
	DefFunc* df = findFuncInTree(callExp->call.id);
	if(!df)
		return 0; //must be a function
	Type* ft = df->retType;
	return typeEquals(t,ft);
}
void typeCommandList(CommandL* cl ) {
	if(!cl)
		return;
	CommandL* c = cl;
	while(c) {
		switch(c->tag) {
			case CWhile:
				
				typeExp(c->condExp );
				enterScope();
				typeCommandList(c->cmdIf );
				leaveScope();
			break;
			case CIf:
				typeExp(c->condExp );
				enterScope();
				typeCommandList(c->cmdIf );
				leaveScope();
			break;
			case CIfElse:
				typeExp(c->condExp );
				enterScope();
				typeCommandList(c->cmdIf );
				leaveScope();
				enterScope();
				typeCommandList(c->cmdElse );
				leaveScope();
			break;
			case CReturn:
				typeExp(c->retExp );
				if(!currentFunction) {
					typeError("Return not allowed outbounds");
				}
				if(!checkTypeReturn(c->retExp,currentFunction)) {
					typeError("Return and func types mismatch");
				}

			break;
			case CAssign:
				
				typeExp(c->expLeft);
				typeExp(c->expRight);
				if(!typeEquals(c->expLeft->type,c->expRight->type)) {
					typeError("Assigment left type differs from right type");
				}
				
			break;
			case CBlock:
				enterScope();
				typeBlock((Block*)c->block );
				leaveScope();
			break;
			case CCall:
				typeExp(c->expRight );
				if(!checkCallability(c->expRight)) {
					typeError("Expression is not callable");
				}
			break;
			case CPrint:
				typeExp(c->printExp);
				if (!checkPrintability(c->printExp)) {
					typeError("Expression is not printable");
				}
			break;
		}
		c = c->next;
	}
}
int typeEquals(Type* t1, Type* t2) {
	// printType(t1,3);
	// printf("typeEqual\n");
	// printType(t2,3);
	// printf("\n");
	if(t1 == NULL || t2 == NULL) {
		return t2 == NULL && t1 == NULL;
	}
	if(t1->tag == base) {
		if(t1->b == t2->b) {
			return 1;
		}
		return ((t1->b == WChar) && (t2->b == WInt)) || 
		((t1->b == WInt) && (t2->b == WChar));
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
	t->tag = base;
	t->b = BTypeOfArith(e->bin.e1,e->bin.e2);
	return t;
}
Type* unaryType(Exp* e) {
	Type* t = (Type*)malloc(sizeof(Type));
	t->tag = base;
	t->b = WInt;
	return t;
}
Type* CmpType(Exp* e) {
	Type* t = (Type*)malloc(sizeof(Type));
	t->b = WInt;
	return t;
}
Type* typeOfCall(Exp* e) {
	if(!e)
		return NULL;
	int index = find(e->call.id);

	if(index < 0) {
		printf("--func %s--\n",e->call.id);
		typeError("No such func in scope");
	}
	return variables[index].type;
}

Type* typeOfNew(Exp* e) {
	Type* t = (Type*)malloc(sizeof(Type));
	t->tag = array;
	t->of = e->eNew.t;
	return t;
}
Type* typeOfAccess(Exp* e) {
	int index = find(e->call.id);

	if(index < 0) {
		printf("--func %s--\n",e->call.id);
		typeError("No such func in scope");
	}
	return variables[index].type;
}

int checkTypeIndex(Exp* e) {
	return e->type->tag == base && e->type->b == WInt;
}

int checkTypeArtih(Exp* left,Exp *right) {
	// printExp(left,0);
	// printExp(right,0);
	if(left->type->tag == base && right->type->tag == base) {
		if(typeEquals(left->type,right->type)) {
			return 1;
		}
		if(left->type->b == WChar || right->type->b == WChar) {
			if(left->type->b == WInt || right->type->b == WInt) {
				return 1;
			}
		}
	}
	return 0;
}
//receives exp inside unary
int checkTypeUnary(Exp* e) {
	Type* t = e->type;
	if(!t)
		return 0;
	return t->tag == base && t->b == WInt;
}
int checkTypeLogic(Exp* left,Exp* right) {
	if(left->type->tag == base && right->type->tag == base)
		return 1;
	return 0;
}
int checkTypeCast(Exp* e) {
	//printExp(e,5);
	if(e->type->tag == base) {
		return e->cast.type->tag == base;
	}
	return 0;
}
int checkTypeExpList(ExpList* el,Parameter* params) {

	if(el == NULL || params == NULL)
		return !el && !params;
	ExpList *p = el;
	//printExpList(el,0);
	//printParams(params,0);
	//puts("");
	while(p) {
		//printf("a\n");
		if(!typeEquals(params->t,p->e->type)) {
			printf("Not equal typing");
			return 0;
		}
		p = p->next;
		params = params->next;
		if(!params && p) {
			return 0;
		}
	}
	return params == NULL;
}
int checkTypeCallParamsArgs(Exp* e) {
	//printf("checkTypeCallParamsArgs\n");
	if(!e)
		return 0;
	int index = find(e->call.id);
	if(index<0)
		return 0;
	Parameter* params = findParamsOfFunc(e->call.id);
	return checkTypeExpList(e->call.expList,params);
}

int checkPrintability(Exp* e) {
	if(!e)
		return 0;
	if(e->type == NULL)
		return 1;
	if(e->type->tag == base) {
		return 1;
	}
	if(e->type->tag == array) { //can print strings
		if(e->type->of->tag == base 
			&& e->type->of->b ==WChar) {
			return 1;
		}
	}
	return 0;
}

void typeExp(Exp* e ) {
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
			if(!checkTypeArtih(e->bin.e1,e->bin.e2)) {
				printExp(e->bin.e1,0);
				printExp(e->bin.e2,0);
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
			e->type = typeOfCall(e);
			if(!checkCallability(e)) {
					printf("--%s--\n", e->call.id);
					typeError("Expression is not callable");
				}
			if(!checkTypeCallParamsArgs(e)) {
				printf("--%s--\n", e->call.id);
				typeError("Params typing differs from arguments in call");
			}
		break;
		case ExpVar:
			typeVar(e->var);
			e->type = e->var->type;
		break;
		case ExpUnary:
			typeExp(e->unary.e);
			if(!checkTypeUnary(e->unary.e))
			{
				typeError("type of Unary not right");
			}
			e->type = unaryType(e);
		break;
		case ExpPrim:
			e->type = typeOfConstant(e->c);
		break;
		case ExpNew:
			typeExp(e->eNew.e);
			if(!checkTypeIndex(e->eNew.e)) {
				typeError("Index of array is not an int");
			}
			e->type = typeOfNew(e);
		break;
		case ExpCmp:
			typeExp(e->cmp.e1);
			typeExp(e->cmp.e2);
			switch(e->cmp.op) {
				default:
					if(!checkTypeLogic(e->cmp.e1,e->cmp.e2)) {
						typeError("Types not suitble for logic");
					}
				break;
				case EqEq:
					if(!typeEquals(e->cmp.e1->type,e->cmp.e2->type)) {
						printType(e->cmp.e1->type,0);
						printType(e->cmp.e2->type,0);
						typeError("Not comparable types in ==");
					}
				break;
			}

			
			e->type = CmpType(e);

		break;
		case ExpAccess:
			
			typeExp(e->access.varExp);
			typeExp(e->access.indExp);
			if(!checkTypeIndex(e->access.indExp)) {
				typeError("Index of array is not an int");
			}
			e->type = e->access.varExp->type->of;
		break;
		case ExpCast:
			typeExp(e->cast.e);
			if(!checkTypeCast(e)) {
				// printType(e->type,0);
				// printType(e->cast.type,0);
				// printType(e->cast.e->type,0);
				typeError("Cast not avaible for these types");
			}
			e->type = e->cast.type;
		break;
	}
	//printExp(e,10);
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
	return NULL;
}


void typeExpList(ExpList* el ) {
	if(!el)
		return;
	ExpList *p = el;
	while(p) {
		typeExp(p->e);
		p = p->next;
	}
	return;

}

void typeVar(Var* v) {
	if(!v)
		return;
	int index = find(v->id);
	if(index < 0) {
		printf("--var %s--\n",v->id);
		typeError("No such var in scope");
	}
	v->type = variables[index].type;
	DefVar* dv = (DefVar*)variables[index].declaration;
	v->declaration = dv;
	if(findFuncInTree(v->id)) {
		typeError("Subject is a function, not a var");
	}
}





