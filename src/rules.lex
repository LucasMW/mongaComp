
%option noyywrap
%{
   #if !defined(lex_h)
	#include "lex.h"
	#define lex_h
   #endif
%}
%%
"char"	{return 301;}
"else"	{return 302; }
"float"	{return 303;}
"if"	{return 304;}
"int"	{return 305;}
"new"	{return 306;}
"return" {return 307;}
"void"	{return 308;}
"while"	{return 309;}
"\n"	{ return '\n';}
"\t"	{ return '\t' ;}
">="	{return 258;}
">"		{return TK_GT;}
"<=" 	{return 257;}
"<"		{return '<';}
"="		{return '=';}
";"		{return ';';}
"{"		{return '{';}
"}"		{return '}';}
[0-9]+	{ seminfo.i = atoi( yytext );
		return TK_INT;}
[0-9]+.[0-9]+ {seminfo.d = strtod(yytext,NULL);
				return TK_FLOAT;}
\".*\" {return 501;}



