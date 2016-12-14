#include <stdio.h>
#include <stdlib.h>
#if !defined(lex_h)
	#include "lex.h"
	#define lex_h
#endif
#if !defined(bison_h)
	#include "bison.h"
	#define bison_h
#endif
#if !defined(tree_h)
	#include "tree.h"
	#define tree_h
#endif
#if !defined(grammar_h)
	#include "grammar.h"
	#define grammar_h
#endif
#if !defined(lextest_h)
	#include "lextest.h"
	#define lextest_h
#endif
#if !defined(symbolTable_h)
	#include "symbolTable.h"
	#define symbolTable_h
#endif
#if !defined(codeGen_h)
	#include "codeGen.h"
	#define codeGen_h
#endif

#include <string.h>
#include <assert.h>
Seminfo_t seminfo;
int yy_lines=1; //save one for EOF

void lexError(const char* message, int ret)
{
	printf("Lexical error detected in source:\n");
	fprintf(stderr,"ERROR: %s\t\t#line %d\n", message, yy_lines);
	exit( ret ? ret : 1);
}

static char* mongaOptions[] = {
	"-check",
	"-syntax",
	"-lex",
	"-noTree",
	"-noChecks",
	"-noBin",
	"-noCode"
};
static char mongaOptionsCount = 7;

static char* handleClangOptions(int argc,char** argv) {
	char* str = (char*)malloc(50*argc); //more than enough
	char flag = 0;
	for(int i=1;i<argc;i++) {
		for(int j=0;j<mongaOptionsCount;j++) {
			if(strcmp(argv[i],mongaOptions[j])==0) {
				flag = 0;
				break;
			}
			flag = 1;
		}
		if(flag == 1) {
			assert(strlen(argv[i])<50);
			strcat(str," ");
			strcat(str,argv[i]);
		}
		flag = 0;
	}
	return str;

}
int main (int argc, char** argv)
{
	char noTree =0;
	char noChecks=0;
	char noCode = 0;
	char noBin = 0;

	if(argc >= 2)
	{
		if(strcmp("-check",argv[1])==0)
		{
			noTree =1;
			noCode =1;
			noBin = 1;
		}
		if(strcmp("-syntax",argv[1])==0) 
		{
			noTree = 1;
			noChecks = 1;
			noCode = 1;
			noBin = 1;
		}
		if(strcmp("-lex",argv[1])==0)
		{
			testLex();
			return 0;
		}
		if(strcmp("-noTree",argv[1])==0)
		{
			noTree = 1;
		}
		if(strcmp("-noChecks",argv[1])==0)
		{
			noChecks = 1;
			noCode =1;
			noBin = 1;
		}
		if(strcmp("-noCode",argv[1])==0)
		{
			noCode =1;
			noBin =1;
		}
		if(strcmp("-noBin",argv[1])==0)
		{
			noBin =1;
		}
	}

	yyparse();
	printf("Syntax OK\n");

	
	if(!noChecks) {
		checkAndFixesTypesInTree();
		printf("Typing OK\n");
	}
	if(!noTree) 
	{
		printTree();
	}
	char * llvm_name = "a.ll";
	//char * bin_name = "a.out";
	if(!noCode) 
	{	FILE* llvm_ir_location = fopen(llvm_name,"wt");
		setCodeOutput(llvm_ir_location);
		codeTree();
		fclose(llvm_ir_location);
	}
	if(!noBin)
	{
		char* str = handleClangOptions(argc,argv);
		char* buff = (char*)malloc(
			strlen(str) + 
			strlen("clang") + 
			strlen(llvm_name)+1);
		sprintf(buff,"clang %s %s",
			str,
			llvm_name);
		int s = system(buff);
		return s;
	}
}

