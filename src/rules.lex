
%option noyywrap
%{
   #if !defined(lex_h)
	#include "lex.h"
	#define lex_h
   #endif
%}
%%
"char"	{return TK_WCHAR;}
"else"	{return TK_WELSE; }
"float"	{return TK_WFLOAT;}
"if"	{return TK_WIF;}
"int"	{return TK_WINT;}
"new"	{return TK_WNEW;}
"return" {return TK_WRETURN;}
"void"	{return TK_WVOID;}
"while"	{return TK_WWHILE;}
"\n"	{return '\n';}
"\t"	{ return '\t' ;}
">="	{return 258;}
">"		{return TK_GT;}
"<=" 	{return 257;}
"<"		{return '<';}
"="		{return '=';}
";"		{return ';';}
"{"		{return '{';}
"}"		{return '}';}

"0x"([0-9]|[A-F])+	{seminfo.i = strtol(yytext, NULL, 0);
				return TK_HEXA;}
[0-9]+	{ seminfo.i = atoi( yytext );
		return TK_INT;}
[0-9]+.[0-9]+ {seminfo.d = strtod(yytext,NULL);
				return TK_FLOAT;}

([a-z]|[A-Z])+ { return TK_VAR;}
\".*\" {return TK_STR;}	
"/*".*"*/" {return TK_COMMENT;}



