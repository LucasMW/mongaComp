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
void codeNameList(NameL* nl,Type* t);
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

void codeTree() {
	if(output==NULL) {
		defaultOutput();
	}
	printf("generating code for tree\n");
	codeDefList(globalTree->next);
} 
void codeDefVar(DefVar* dv) {

}
void codeDefFunc(DefFunc* df) {

	char* typeStr = stringForType(df->retType);
	if(df->b) {
		currentFunctionTIndex = 0;
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
	}
	else {
		fprintf(output, "declare %s @%s(", typeStr,df->id);
		codeParams(df->params);
		fprintf(output, ")\n");
	}
}
void codeDefVarList(DefVarL* dvl) {

}
void codeDefList(Def* d) {
	if(!d)
		return;
	//printf("coding DefList\n");
	switch (d->tag) {
		case DVar:
		break;
		case DFunc:
			codeDefFunc(d->u.f);
		break;
	}
	codeDefList(d->next);

}
void codeNameList(NameL* nl,Type* t);
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
				 i1 = codeExp(c->expLeft);
				 i2 = codeExp(c->expRight);
				// if(!typeEquals(c->expLeft->type,c->expRight->type)) {
				// 	typeError("Assigment left type differs from right type");
				// }
				
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
	// codeDefVarList(b->dvl);
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
void codeCallExp(Exp* e) {
	if(e->type == NULL) {
		fprintf(output, "call void @%s(",
			e->call.id);
	}
	else {
		char* fTypeStr = stringForType(e->type); 
		fprintf(output, "%%t%d = call %s @%s(",
			currentFunctionTIndex++,
			fTypeStr,
			e->call.id);
	}
	codeExpList(e->call.expList);
	fprintf(output, ")\n" );		
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
			codeCallExp(e);
		break;
		case ExpVar:
			// typeVar(e->var);
			// e->type = e->var->typtypeExp(e->unary.e);
			// if(!checkTypeUnary(e->unary.e))
			// {
			// 	typeError("type of Unary not right");
			// }
			// e->type = unaryType(e);e;
		break;
		case ExpUnary:
			// 
		break;
		case ExpPrim:
			result = codeExpPrim(e);
			// e->type = typeOfConstant(e->c);
		break;
		case ExpNew:
			// typeExp(e->eNew.e);
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
			// typeExp(e->access.varExp);
			// typeExp(e->access.indExp);
			// if(!checkTypeIndex(e->access.indExp)) {
			// 	typeError("Index of array is not an int");
			// }
			// e->type = e->access.varExp->type->of;
		break;
		case ExpCast:
			// typeExp(e->cast.e);
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
void codeVar(Var* v);
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