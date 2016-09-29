extern int yylex(void);

typedef enum token 
{
	TK_LE,
	TK_INT,
	TK_FLOAT,
	TK_WCHAR = 301, //Reserved words start at 300
	TK_WELSE,
	TK_WFLOAT,
	TK_WIF,
	TK_WINT,
	TK_WNEW,
	TK_WRETURN,
	TK_WVOID,
	TK_WWHILE,
	TK_AND = 401,
	TK_OR,
	TK_EQEQ,
	TK_STR = 501,
	TK_VAR

} Token;


typedef union seminfo_t
{
	int i;
	double d;
	const char *s;

} Seminfo_t;
extern Seminfo_t seminfo;
extern int yy_lines;
void lexError(const char* message, int ret);

