
%option noyywrap
%{
   #if !defined(lex_h)
	#include "lex.h"
	#define lex_h
   #endif
   #if !defined(grammar_h)
	#include "grammar.h"
	#define grammar_h
   #endif

	static void countlinesintoken() //Legacy
	{
		for(int i = 0; i < yyleng; i++)  
		{
			if(yytext[i] == '\n')
				yy_lines++;
		}
	}
	// transforms scapes of yytext to ascii characters
	static char * translatescape()
	{
		char flag = 0;
		char* newStr = (char*)malloc(yyleng);
		int j = 0;
		for(int i = 1; i < yyleng-1; i++) //limits to discard quotes  
		{
			if(flag == 1) //means last character was '\\'
			{
				switch(yytext[i])
				{
					case 'n':
					newStr[j++]='\n';
					break;
					case 't':
					newStr[j++]='\t';
					break;
					case '\"':
					newStr[j++]='\"';
					break;
					case '\\': //convenience
					newStr[j++]='\\';
					break;
				}
				flag = 0; 
			}
			else
			{
				if(yytext[i] == '\\' )
					flag = 1;
				else
					newStr[j++] = yytext[i];
			}
		}
		newStr[j]= '\0';
		return newStr;
	}
%}

%x IN_COMMENT 
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
">"		{return '>';}
"<=" 	{return TK_LE;}
"<"		{return '<';}
";"		{return ';';}
"=="	{return TK_EQEQ;}
"="		{return '=';}
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

"0x"([0-9]|[A-F]|[a-f])+	{seminfo.i = strtol(yytext, NULL, 0);
				return TK_INT;}
[0-9]+	{ seminfo.i = atoi( yytext );
		return TK_INT;}
[0-9]+"."[0-9]+([Ee][-+]?[0-9]+)? {seminfo.d = strtod(yytext,NULL);
				return TK_FLOAT;}

([a-z]|[A-Z])([a-z]|[A-Z]|[0-9])* { seminfo.s = yytext;
									return TK_VAR;}

<INITIAL>{ //state machine based on the example of flex manual: http://flex.sourceforge.net/manual/How-can-I-match-C_002dstyle-comments_003f.html
     "/*"      BEGIN(IN_COMMENT); 
     }
     <IN_COMMENT>{
     "*/"      BEGIN(INITIAL); // finished comment; Return to the original state
     [^*\n]+   // eat comment in chunks
     "*"       // eat the lone star
     \n        yy_lines++; //comments can be mult-line
     <<EOF>>	   lexError("Unfinished Comment",1); //reached EOF 
     }


\"(([\\][\"])|([^\"\n])+)*\" { seminfo.s = translatescape(); //gives scape processed string
		 return TK_STR;}

\'.\' 	{seminfo.i = *(yytext+1); //no multi-char character constants
		return TK_INT;}
\'"\\n"\' { seminfo.i = '\n';
		return TK_INT;}
\'"\\t"\' { seminfo.i = '\t';
		return TK_INT;}
\'"\\"\"\' { seminfo.i = '\"';
		return TK_INT;}

\'.[^\']	{
		 lexError("Unfinished Character",2);}

\"[^"\""]*[\n]?	{ lexError("Unfinished String",3); 
			}	


[\n] 	{  	//count lines;
			yy_lines++; }
[ ]|[\t] {//ignore spaces and tabs
	 }
. { return yytext[0];}



