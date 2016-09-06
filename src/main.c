#include <stdio.h>
#if !defined(lex_h)
	#include "lex.h"
	#define lex_h
#endif
#include <string.h>


#define NORMALMODE 0;
#define TESTMODE 1;
Seminfo_t seminfo;
int main (int argc, char** argv)
{
	int lines=1; //save one for EOF
	int tokens=-1;
	int control=1; //Grants it enters the while
	char mode = 0;
	FILE* testout = NULL;
	if(argc == 2) {
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
				printf("tab \n");
			break;
			case '>':
				printf("Greater ");
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
			case 301:
				printf("reserved word\n");
			break;
			case 302:
				printf("reserved word\n");
			break;
			case 303:
				printf("reserved word\n");
			break;
			case 304:
				printf("reserved word\n");
			break;
			case 305:
				printf("reserved word\n");
			break;
			case 306:
				printf("reserved word\n");
			break;
			case 307:
				printf("reserved word\n");
			break;
			case 308:
				printf("reserved word\n");
			break;
			case 309:
				printf("reserved word\n");
			break;
			case 501:
				printf("String\n");
			break;

		}
	}
	printf("FINISHED READING FILE\n");
	printf("Proccessed %d tokens in %d lines\n",tokens,lines);
}