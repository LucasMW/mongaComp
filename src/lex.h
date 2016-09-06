extern int yylex(void);

typedef enum token 
{
	TK_GT = 256,
	TK_GE,
	TK_LT,
	TK_LE,
	TK_EQ,
	TK_STATEMENT,
	TK_INT,
	TK_FLOAT

} Token;

typedef union seminfo_t
{
	int i;
	double d;
	const char *s;

} Seminfo_t;
extern Seminfo_t seminfo;

// char *yytext;
// /*holds the text of the current token. It may be modified but not lengthened (you cannot append characters to the end).
// If the special directive %array appears in the first section of the scanner description, then yytext is instead declared char yytext[YYLMAX], where YYLMAX is a macro definition that you can redefine in the first section if you don't like the default value (generally 8KB). Using %array results in somewhat slower scanners, but the value of yytext becomes immune to calls to unput(), which potentially destroy its value when yytext is a character pointer. The opposite of %array is %pointer, which is the default.

// You cannot use %array when generating C++ scanner classes (the ‘-+’ flag).
// */
// int yyleng;
// /*holds the length of the current token.*/


// FILE *yyin;
// is the file which by default flex reads from. It may be redefined but doing so only makes sense before scanning begins or after an EOF has been encountered. Changing it in the midst of scanning will have unexpected results since flex buffers its input; use yyrestart() instead. Once scanning terminates because an end-of-file has been seen, you can assign yyin at the new input file and then call the scanner again to continue scanning.


// void yyrestart( FILE *new_file );
// /*may be called to point yyin at the new input file. The switch-over to the new file is immediate (any previously buffered-up input is lost). Note that calling yyrestart() with yyin as an argument thus throws away the current input buffer and continues scanning the same input file.
// */

// FILE *yyout;
// /*is the file to which ECHO actions are done. It can be reassigned by the user.
// */
