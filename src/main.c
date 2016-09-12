#include <stdio.h>
#if !defined(lex_h)
	#include "lex.h"
	#define lex_h
#endif
#include <string.h>


#define NORMALMODE 0;
#define TESTMODE 1;
Seminfo_t seminfo;
int tklen;
int main (int argc, char** argv)
{
	int lines=1; //save one for EOF
	int tokens=-1;
	int control=1; //Grants it enters the while
	char mode = 0;
	FILE* testout = NULL;
	if(argc == 2) 
	{
		if(strcmp(argv[1],"-test")==0)
		{
			mode = TESTMODE;
			testout = fopen("test_output.txt","wt");
		}
	}
	while(control)
	{
		control = yylex();
		tokens++;
		//printf("%s ",yytext);
		switch(control)
		{
			case '\n':
				printf("Line Break\n");
				lines++;
			break;
			case '\t':
				printf("tab\n");
			break;
			case '>':
				printf("Greater");
			break;
			case '<':
				printf("Lesser ");
			break;
			case '=':
				printf("Equal ");
			break;
			case ';':
				printf("Statement\n");
			break;
			case TK_INT:
				printf("Int %d\n",seminfo.i);
			break;
			case TK_FLOAT:
				printf("Float %f\n",seminfo.d);
			break;
			case TK_HEXA:
				printf("Hexa %d\n",seminfo.i);
			break;
			case TK_WCHAR:
				printf("reserved word char\n");
			break;
			case TK_WELSE:
				printf("reserved word else\n");
			break;
			case TK_WFLOAT:
				printf("reserved word float\n");
			break;
			case TK_WIF:
				printf("reserved word if\n");
			break;
			case TK_WINT:
				printf("reserved word int\n");
			break;
			case TK_WNEW:
				printf("reserved word new\n");
			break;
			case TK_WRETURN:
				printf("reserved word return\n");
			break;
			case TK_WVOID:
				printf("reserved word void\n");
			break;
			case TK_WWHILE:
				printf("reserved word while\n");
			break;
			case TK_STR:
				printf("String: %s\n",seminfo.s);
			break;
			case TK_VAR:
				printf("var\n");
			break;
			case TK_COMMENT:
				for(int i = 0; i < strlen(seminfo.s); i++)
				{
					if(seminfo.s[i]== '\n')
						lines++;
				}
				printf("Comment\n");
			break;
			case TK_ERROR:
				printf("Lexical error detected in source.\n");
				printf("ERROR:%s\n", seminfo.s );
			break;

		}
	}
	printf("FINISHED READING FILE\n");
	printf("Proccessed %d tokens in %d lines\n",tokens,lines);
}