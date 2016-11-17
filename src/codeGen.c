#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void codeCommandList(CommandL* cl);
void codeBlock(Block* b);
void codeExp(Exp* e);
void codeVar(Var* v);
void codeConstant(Constant* c);
void codeExpList(ExpList* el);
char* stringForType(Type* t);

static FILE* output = NULL;

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
		fprintf(output, "define %s @%s(", typeStr,df->id);
		codeParams(df->params);
		fprintf(output, ")\n{");
		codeBlock(df->b);
		fprintf(output, "}");
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
	printf("coding DefList\n");
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
void codeCommandList(CommandL* cl);
void codeBlock(Block* b) {

}
void codeExp(Exp* e);
void codeVar(Var* v);
void codeConstant(Constant* c);
void codeExpList(ExpList* el);