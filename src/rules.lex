%option noyywrap

%%
'\t'	{return '\t' ;}
'>'		{return '>';}
'>='	{return 10;}
[0-9]+	{return 9;}
