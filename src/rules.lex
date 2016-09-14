
%option noyywrap
%{
   #if !defined(lex_h)
	#include "lex.h"
	#define lex_h
   #endif

	static void countlinesintoken()
	{
		for(int i = 0; i < yyleng; i++)  
		{
			if(yytext[i] == '\n')
				yy_lines++;
		}
		printf("Comment\n");	
	}
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
">="	{return TK_GE;}
">"		{return TK_GT;}
"<=" 	{return TK_LE;}
"<"		{return TK_LS;}
";"		{return TK_STATEMENT;}
"=="	{return TK_EQEQ;}
"="		{return TK_EQ;}
"{"		{return '{';}
"}"		{return '}';}
"["		{return '[';}
"]"		{return ']';}
"("		{return '(';}
")"		{return ')';}
"!"		{return '!';}
"&&"	{return TK_AND;}
"||"	{return TK_OR;}
"+"		{return '+';}
"-"		{return '-';}
"*"		{return '*';}
"/"		{return '/';}
","		{return ',';}

"0x"([0-9]|[A-F])+	{seminfo.i = strtol(yytext, NULL, 0);
				return TK_HEXA;}
[0-9]+	{ seminfo.i = atoi( yytext );
		return TK_INT;}
[0-9]+"."[0-9]+ {seminfo.d = strtod(yytext,NULL);
				return TK_FLOAT;}

([a-z]|[A-Z])([a-z]|[A-Z]|[0-9])* { seminfo.s = yytext;
									return TK_VAR;}
\"(([\\][\"])|[^\"])*\" { seminfo.s = yytext;
		 return TK_STR;}
\"[^"\""]*	{ lexError("Unfinished String",3); 
			}	
\'.\' 	{seminfo.i = *(yytext+1);
		return TK_INT;}
\'"\\n"\' { seminfo.i = '\n';
		return TK_INT;}
\'"\\t"\' { seminfo.i = '\t';
		return TK_INT;}
\'"\\"\"\' { seminfo.i = '\"';
		return TK_INT;}

\'.[^\']	{
		 lexError("Unfinished Character",2);}
"/*"([^*]|"*"+[^*/])*"*"+"*/"	{ 
		 							countlinesintoken(); //comments can be mult-line
		 						}
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/] { 
		 							countlinesintoken();
		 							}
[/][*]                          { //from http://stackoverflow.com/questions/25395251/detecting-and-skipping-line-comments-with-flex
								lexError("Unfinished Comment",1); }

[\n] 	{  	//printf("Line Break\n");
			yy_lines++; }
[ ]|[\t] {//ignore spaces and tabs in flex output
	 }



