%option noyywrap

%%

"\t"	{printf("FOUND\\t\n"); return '\t' ;}
">"		{
		printf("FOUND>\n");return '>';}
">="	{return 10;}
[0-9]+	{printf("Number\n");return 9;}
