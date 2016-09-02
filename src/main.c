#include <stdio.h>
#include "lex.h"

typedef enum token 
{
	TK_GT = 256,
	TK_GE
} Token;


int main (void)
{
	int lines=0;
	int tokens=-1;
	int control;
	while(control)
	{
		control = yylex();
		tokens++;
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
			case 400:
				printf("Number\n");
			break;
		}
	}
	printf("FINISHED READING FILE\n");
	printf("Proccessed %d tokens in %d lines\n",tokens,lines);
}