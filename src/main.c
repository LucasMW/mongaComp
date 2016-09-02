#include <stdio.h>
#include "lex.h"

typedef enum token 
{
	TK_GT = 256,
	TK_GE
} Token;


int main (void)
{
	int lines=1; //save one for EOF
	int tokens=-1;
	int control=1; //Grants it enters the while
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
			case 401:
				printf("Int\n");
			break;
			case 402:
				printf("Float\n");
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