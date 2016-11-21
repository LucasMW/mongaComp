#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#if !defined(tree_h)
	#include "tree.h"
	#define tree_h
#endif

void codeDefVar(DefVar* dv);
void codeDefFunc(DefFunc* df);
void codeDefVarList(DefVarL* dvl);
void codeDefList(Def* d);
void codeNameList(NameL* nl,Type* t,int scope);
void codeType(Type* t);
void codeParams(Parameter* params);
void codeForAllocParams(Parameter* params);
void codeCommandList(CommandL* cl);
void codeBlock(Block* b);
int codeExp(Exp* e);
void codeVar(Var* v);
void codeConstant(Constant* c);
void codeExpList(ExpList* el);
char* stringForType(Type* t);

static FILE* output = NULL;
static int currentFunctionTIndex = 0;

void setCodeOutput(FILE* out) {
	output = out;
} 

static void defaultOutput() {
	output = stdout;
}

static Parameter* currentParameters = NULL;

char* stringForType(Type* t) {
	char* tStr = NULL;
	if(t == NULL)
		return "void";
	switch(t->tag) {
		case base:
			switch(t->b) {
				case WInt:
					return "i32";
				break;
				case WFloat:
					return "float";
				break;
				case WChar:
					return "i8";
				break;
			}
		break;
		case array:
			
			tStr = stringForType(t->of);
			int tlen = strlen(tStr);
			char* new = malloc(tlen+2);
			strcpy(new,tStr);
			//free(tStr); // no need anymore
			new[tlen] = '*';
			new[tlen+1] = '\0';
			return new;

		break;
	}
}

char* stringForVarAddress(const char* name,int scope) {
	char string[50] = "no string yet";
	if(scope == 0) {
		sprintf(string,"g%s",name);
	}
	else {
		sprintf(string,"l%d%s",scope,name);
	}
	
	char* str = (char*)malloc(strlen(string)+1);
	strcpy(str,string);
	str[strlen(string)] = '\0';
	return str;
}

void codeTree() {
	if(output==NULL) {
		defaultOutput();
	}
	printf("generating code for tree\n");
	codeDefList(globalTree->next);
} 
void codeDefVar(DefVar* dv) {
	
	codeNameList(dv->nl,dv->t,dv->scope);	

}
void codeDefFunc(DefFunc* df) {

	char* typeStr = stringForType(df->retType);
	if(df->b) {
		currentFunctionTIndex = 0;
		currentParameters = df->params;
		fprintf(output, "define %s @%s(", typeStr,df->id);
		codeParams(df->params);
		fprintf(output, ")\n{\n");
		codeForAllocParams(df->params);
		codeBlock(df->b);
		if(df->retType == NULL) {
			fprintf(output, "ret void\n");
		} //probably missing a ret in the end of void func
		fprintf(output, "}\n");
		currentFunctionTIndex = 0;
		currentParameters = NULL;
	}
	else {
		fprintf(output, "declare %s @%s(", typeStr,df->id);
		codeParams(df->params);
		fprintf(output, ")\n");
	}
}
void codeDefVarList(DefVarL* dvl) {

	DefVarL* p = dvl;
	while(p) {
		codeDefVar(p->dv);
		p=p->next;
	}
}
void codeDefList(Def* d) {
	if(!d)
		return;
	//printf("coding DefList\n");
	switch (d->tag) {
		case DVar:
			codeDefVar(d->u.v);
		break;
		case DFunc:
			codeDefFunc(d->u.f);
		break;
	}
	codeDefList(d->next);

}
void codeNameList(NameL* nl,Type* t,int scope) {
	char* tStr = stringForType(t);
	NameL* p = nl;
	
	while(p) {
		char* string = stringForVarAddress(p->name,scope);
		fprintf(output, "%%%s = alloca %s\n", 
			string,  
			tStr );
		free(string);
		p=p->next;
	}
}
void codeType(Type* t);
void codeParams(Parameter* params) {
	if(!params)
		return;
	char * tStr = stringForType(params->t);
	fprintf(output,"%s",tStr);
	if(params->next) {
		fprintf(output, "," );
		codeParams(params->next);
	}
}
void codeForAllocParams(Parameter* params) {
	if(!params)
		return;
	char * tStr = stringForType(params->t);
	int index = currentFunctionTIndex;
	Parameter* p = params;
	while(p) {
		fprintf(output,"%%t%d = alloca %s\n", currentFunctionTIndex++, tStr);
		p = p->next;
	}
	p = params;
	int i=0;
	while(p) {
		fprintf(output,"store %s %%%d, %s* %%t%d\n", tStr, i++, tStr, index++);
		p = p->next;
	}

}
void codeForAssign() {

}
char* adressOfLeftAssign(Exp* e) {
	if(e->tag == ExpVar) {
		int scope = e->var->declaration->scope;
		char* varAddr = stringForVarAddress(e->var->id,scope);
		return varAddr;
	}
	else {
		fprintf(output, "; not implemented \n" );
	}
	return NULL;
}
void codeCommandList(CommandL* cl) {
	if(!cl)
		return;
	CommandL* c = cl;
	printf("CommandL\n");
	int i1,i2;
	while(c) {
		printf("cl\n");
		switch(c->tag) {
			case CWhile:
				// typeExp(c->condExp );
				// enterScope();
				// typeCommandList(c->cmdIf );
				// leaveScope();
			break;
			case CIf:
				// typeExp(c->condExp );
				// enterScope();
				// typeCommandList(c->cmdIf );
				// leaveScope();
			break;
			case CIfElse:
				// typeExp(c->condExp );
				// enterScope();
				// typeCommandList(c->cmdIf );
				// leaveScope();
				// enterScope();
				// typeCommandList(c->cmdElse );
				// leaveScope();
			break;
			case CReturn:	
				printf("cret\n");
				if(c->retExp == NULL) {
					fprintf(output, "ret void\n");
				}
				else {
				char * tStr = stringForType(c->retExp->type);
				i1 = codeExp(c->retExp);
				fprintf(output, "ret %s %%t%d\n",tStr,i1);
				}
			break;
			case CAssign:
				 printf("CAssign\n");
				 char* tStr = stringForType(c->expLeft->type);
				 char* addr = adressOfLeftAssign(c->expLeft);
				 //i1 = codeExp(c->expLeft);
				 i2 = codeExp(c->expRight);

				 fprintf(output, "store %s %%t%d, %s* %%%s \n",
				 	tStr,i2,tStr,addr);
			break;
			case CBlock:
				printf("cblock\n");
				codeBlock((Block*)c->block );
				// leaveScope();
			break;
			case CCall:
				printf("ccall\n");
				codeExp(c->expRight);
			break;
			case CPrint:
				// typeExp(c->printExp);
				// if (!checkPrintability(c->printExp)) {
				// 	typeError("Expression is not printable");
				// }
			break;
		}
		c = c->next;
	}
}
void codeBlock(Block* b) {
	codeDefVarList(b->dvl);
	codeCommandList(b->cl);
}
int codeBinExp(Exp* e ,char * cmd) {
	int te1,te2; 
	te1 = codeExp(e->bin.e1 );
	te2 = codeExp(e->bin.e2 );
	// te2 = currentFunctionTIndex-1;
	// te1 = te2-1;
	char * tStr = stringForType(e->type);
	currentFunctionTIndex++;
	int index = currentFunctionTIndex;
	fprintf(output, "%%t%d = %s %s %%t%d, %%t%d\n",
		currentFunctionTIndex++,cmd, tStr, te1,te2);
	return index;
}
int codeCallExp(Exp* e) {
	int toCall = -1;
	int size=0;
	ExpList *p = e->call.expList;
	//calculate size
	while(p) {
		size++;
		p = p->next;
	}
	//generate code for arguments
	p = e->call.expList;
	int * args = (int*)malloc(sizeof(int)*size);
	int i=0;
	while(p) {
		int index = codeExp(p->e);
		args[i] = index;
		i++;
		p=p->next;
	}

	if(e->type == NULL) {
		fprintf(output, "call void @%s(",
			e->call.id);
	}
	else {
		char* fTypeStr = stringForType(e->type);
		 toCall = ++currentFunctionTIndex; 
		fprintf(output, "%%t%d = call %s @%s(",
			toCall,
			fTypeStr,
			e->call.id);
	}
	p = e->call.expList;
	i=0;
	while(p) {
		char* tStr = stringForType(p->e->type);
		fprintf(output, "%s %%t%d",tStr,args[i]);
		if(p->next)
			fprintf(output, ", ");
		p = p->next;
		i++;
	}
	fprintf(output, ")\n" );
	return toCall;		
}
char* stringForConstant(Constant* c) {
	//char str[40] = "no string given";
	char* str = (char*)malloc(40);
	double nd;
	int exponent = 0;
	if(!c)
		return NULL;
	switch(c->tag) {
		case KInt:
			sprintf(str, "%d", c->u.i);
		break;
		case KFloat:
			nd = frexp(c->u.d, &exponent);
			sprintf(str, "%f", c->u.d);
		break;
		case KStr:
			sprintf(str, "\"%s\"", c->u.str);
		break;
	}
	return &str[0];
}
int codeExpPrim(Exp* e) {
	currentFunctionTIndex++;
	char* tStr = stringForType(e->type);
	if(e->c->tag == KStr) {
		fprintf(output, "no code yet for strings\n" );
		return -1;
	}
	
	char* cStr = stringForConstant(e->c);
	// fprintf(output, "%%t%d = alloca %s* \n",currentFunctionTIndex);
	// fprintf(output, "store i8* %0, i8** %2, align 8\n", );
	if(e->c->tag == KFloat) {
		fprintf(output, "%%t%d= fadd %s 0.0 , %s\n",
			currentFunctionTIndex,
			tStr,
			cStr );
	} else {
		fprintf(output, "%%t%d= add nsw %s 0 , %s\n",
			currentFunctionTIndex,
			tStr,
			cStr );
	}
	int index = currentFunctionTIndex++;
	return index;
}
int getAddressOfVar(Var* id) {
	return -1;
}
int codeExpVar(Exp* e) {
	currentFunctionTIndex++;
	char* tStr = stringForType(e->type);
	if(e->var->declaration == NULL)
	{
		printf(";params\n");
		Parameter* p = currentParameters;
		int t=0;
		while(p) {
			if(strcmp(e->var->id,p->id)==0)
				break;
			p=p->next;
			t++;
		}

		fprintf(output,"%%t%d = load %s, %s* %%t%d\n", 
				currentFunctionTIndex,
				 tStr,
				 tStr,
				 t);
	}
	else {
	int scope = e->var->declaration->scope;
	char* varAddr = stringForVarAddress(e->var->id,scope);
	fprintf(output,"%%t%d = load %s, %s* %%%s\n", 
				currentFunctionTIndex,
				 tStr,
				 tStr,
				 varAddr);
	}
	return currentFunctionTIndex;
}
int codeExpUnary(Exp* e) {
	char* tStr = stringForType(e->type);
	currentFunctionTIndex++;
	int i1,i2;
	i1 = codeExp(e->unary.e);
	switch(e->unary.op) {
		case MINUS:
			if(e->type->b == WFloat) {
				fprintf(output, "%%t%d = fsub %s 0.0, %%t%d\n",
				 currentFunctionTIndex,
				 tStr,
				 i1);
				
			}
			else {
				fprintf(output, "%%t%d = sub nsw %s 0, %%t%d\n",
				 currentFunctionTIndex,
				 tStr,
				 i1);
			}
		break;
		case NOT:
			if(e->type->b == WFloat) {
				fprintf(output, "%%t%d = fcmp oeq float %%t%d, 0.0\n",
				 currentFunctionTIndex,
				 i1);
			i2 = currentFunctionTIndex++;
  			fprintf(output, "%%t%d = uitofp i1 %%t%d to float\n",
				currentFunctionTIndex,
				i2);
			}
			else {
				fprintf(output, "%%t%d = icmp eq %s %%t%d, 0\n",
				 currentFunctionTIndex,
				 tStr,
				 i1);
				i2 = currentFunctionTIndex++;
				fprintf(output, "%%t%d = zext i1 %%t%d to %s\n",
				currentFunctionTIndex,
				i2,
				tStr );
			}
		break; 
	}
	return currentFunctionTIndex;
}
int codeExpCast(Exp* e) {
	int i1 = codeExp(e->cast.e);
	char* orTStr = stringForType(e->cast.e->type);
	char* toTStr = stringForType(e->type);
	currentFunctionTIndex++;
	if(e->type->b == WFloat) {
		fprintf(output, "%%t%d = sitofp %s %%t%d to float\n",
		currentFunctionTIndex,
		orTStr,
		i1 );
	}
	else if(e->cast.e->type->b == WFloat) {
		fprintf(output, "%%t%d = fptosi float %%t%d to %s\n",
		currentFunctionTIndex,
		i1,
		toTStr);
		
	}
	else {
		fprintf(output, "cast not implemented \n" );
	}
	return currentFunctionTIndex;
}
int codeExpAccess(Exp* e) {
	int i2;
	//i1 = codeExp(e->access.varExp);
	char* arrayName = adressOfLeftAssign(e->access.varExp);
	i2 = codeExp(e->access.indExp);
	currentFunctionTIndex++;
	fprintf(output, "%%t%d = sext i32 %%t%d to i64\n",
			currentFunctionTIndex,
			i2 );
	int p64 = currentFunctionTIndex++;
	fprintf(output, "%%t%d = getelementptr inbounds [100 x i32], [100 x i32]* %s, i64 0, i64 %%t%d\n",
	currentFunctionTIndex,
	arrayName,
	p64 );
	return currentFunctionTIndex;
}

int codeExp(Exp* e) {
	int result =-1;
	if(!e)
		return -1;
	switch(e->tag) {
		case ExpAdd:
			if(e->type->b == WFloat) 
				result = codeBinExp(e,"fadd nsw");
			else
				result = codeBinExp(e,"add nsw");
		break;
		case ExpSub:
			if(e->type->b == WFloat) 
				result = codeBinExp(e,"fsub nsw");
			else
				result =codeBinExp(e,"sub nsw");
		break;
		case ExpMul:
			if(e->type->b == WFloat) 
				result = codeBinExp(e,"fmul nsw");
			else
				result = codeBinExp(e,"mul nsw");
		break;
		case ExpDiv:
			if(e->type->b == WFloat) 
				result = codeBinExp(e,"fdiv");
			else
				result = codeBinExp(e,"sdiv");
		break;
		case ExpCall:
			result = codeCallExp(e);
		break;
		case ExpVar:
			result = codeExpVar(e);
		break;
		case ExpUnary:
			result = codeExpUnary(e);
		break;
		case ExpPrim:
			result = codeExpPrim(e);
			// e->type = typeOfConstant(e->c);
		break;
		case ExpNew:
			// if(!checkTypeIndex(e->eNew.e)) {
			// 	typeError("Index of array is not an int");
			// }
			// e->type = typeOfNew(e);
		break;
		case ExpCmp:
			// typeExp(e->cmp.e1);
			// typeExp(e->cmp.e2);
			// switch(e->cmp.op) {
			// 	default:
			// 		if(!checkTypeLogic(e->cmp.e1,e->cmp.e2)) {
			// 			typeError("Types not suitble for logic");
			// 		}
			// 	break;
			// 	case EqEq:
			// 		if(!typeEquals(e->cmp.e1->type,e->cmp.e2->type)) {
			// 			printType(e->cmp.e1->type,0);
			// 			printType(e->cmp.e2->type,0);
			// 			typeError("Not comparable types in ==");
			// 		}
			// 	break;
			// }
			// e->type = CmpType(e);

		break;
		case ExpAccess:
			result = codeExpAccess(e);
			// typeExp(e->access.varExp);
			// typeExp(e->access.indExp);
			// if(!checkTypeIndex(e->access.indExp)) {
			// 	typeError("Index of array is not an int");
			// }
			// e->type = e->access.varExp->type->of;
		break;
		case ExpCast:
			result = codeExpCast(e);
			// if(!checkTypeCast(e)) {
			// 	// printType(e->type,0);
			// 	// printType(e->cast.type,0);
			// 	// printType(e->cast.e->type,0);
			// 	typeError("Cast not avaible for these types");
			// }
			// e->type = e->cast.type;
		break;
	}

	return result;
}
void codeVar(Var* v) {
	char* tStr = stringForType(v->type);
	fprintf(output, "%%L%s = alloca %s\n",
	v->id,  tStr );
}
void codeConstant(Constant* c);
void codeExpList(ExpList* el) {
	char * tStr;
	if(!el)
		return;
	ExpList *p = el;
	while(p) {
		int index = codeExp(p->e);
		tStr = stringForType(p->e->type);
		fprintf(output, "%s %%t%d",tStr,index);
		if(p->next)
			fprintf(output, ", ");
		p = p->next;
	}
	return;

}