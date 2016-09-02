%option noyywrap

int lex_lines = 0;
%%
"\n"	{ return '\n';}
"\t"	{ return '\t' ;}
">"		{return '>';}
">="	{return 10;}
[0-9]+	{return 400;}
