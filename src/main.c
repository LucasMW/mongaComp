#include <stdio.h>
#include "lex.h"

typedef enum token 
{
	TK_GT = 256,
	TK_GE
} Token;


int main (void)
{
	int control;
	while(control)
	{
		control = yylex();
	}
	
	printf("FINISHED READING FILE\n");
}