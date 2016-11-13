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


#include <string.h>

Seminfo_t seminfo;
int yy_lines=1; //save one for EOF

void lexError(const char* message, int ret)
{
	printf("Lexical error detected in source:\n");
	fprintf(stderr,"ERROR: %s\t\t#line %d\n", message, yy_lines);
	exit( ret ? ret : 1);
}
int main (int argc, char** argv)
{
	char noTree =0;
	char noChecks=0;

	if(argc == 2)
	{
		if(strcmp("-syntax",argv[1])==0) 
		{
			noTree = 1;
			noChecks = 1;
		}
		if(strcmp("-lex",argv[1])==0)
		{
			testLex();
			return 0;
		}
		if(strcmp("-noChecks",argv[1])==0)
		{
			noChecks =1;
		}
	}

	yyparse();
	printf("Syntax OK\n");

	
	if(!noChecks) {
		checkAndFixesTypesInTree();
	}
	if(!noTree) 
	{
		printf("Typing OK\n");
		printTree();
	}
}

