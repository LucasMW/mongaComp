#include <stdio.h>
#include "lex.h"

typedef enum token 
{
	TK_GT = 256,
	TK_GE
} Token;


int main (void)
{
	yylex();
	printf("hi\n");
}