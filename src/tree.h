


typedef enum constantType { KFloat, KInt, KStr } constantType;
typedef struct Constant
{
	constantType tag;
	union {
		const char* str;
		int i;
		double d;
	} u;

} Constant;




typedef enum defType { DFunc, DVar} defType;

typedef struct DefFunc
{
	const char* id;


} DefFunc;

typedef enum BType { WInt, WFloat, WChar} BType;
typedef enum Types { base, array} Types;

typedef enum CTypes { CWhile, CIf, CIfElse, CReturn, CAssign, CBlock } CTypes;



typedef struct Var
{
	const char* id;
} Var;

typedef enum ExpE {
	ExpAdd,
	ExpSub,
	ExpMul,
	ExpDiv,
	ExpRet,
	ExpOr,
	ExpAnd,
	ExpEq,
	ExpNot,
	ExpVar,
	ExpCall,
	ExpPrim
} ExpE;

struct Exp{
	ExpE tag;
	union {
		struct {
			struct Exp *e1, *e2;
		} bin;
		struct Exp *unary;
		int ki;
		Var *var;
	};
};
typedef struct Type
{
	Types tag;
	BType b;
	struct Type* of; //array of
	
} Type;

typedef struct Parameter
{
	Type* t;
	char* id;
	struct Parameter* next;
} Parameter;

typedef struct NameL
{
	const char* name;
	struct NameL* next;
} NameL;

typedef struct DefVar
{
	Type* t;
	NameL* nl;
} DefVar;


typedef struct CommandL
{
	CTypes tag;
	struct CommandL* next;
	void* condExp;
	struct CommandL* cmdIf;
	struct CommandL* CmdElse;
	void* expLeft;
	void* expRight;
} CommandL;



// typedef struct CommandL
// {
// 	CTypes tag;
// 	struct CommandL* next;

// } CommandL;
typedef struct Block
{	
	DefVar* dvarlist;
	CommandL* cl;
	
} Block;
typedef struct Def {
	defType tag;
	union {
		DefFunc* f;
		DefVar* v;
	} u;
	struct Def* next;
} Def;

typedef struct progNode {
	Def* next;
} progNode;
void printDefVar(DefVar* dv);
void printDefFunc(DefFunc* df);
extern progNode* globalTree;
void printTree();
void printDefList(Def* d);
void printNameList(NameL* nl);
void printType(Type* t);
Constant* makeConstant(constantType t);
void notConst();
void minusConst();
void makeID();