


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

typedef enum BType { WInt, WFloat, WChar} BType;
typedef enum Types { base, array} Types;

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




typedef enum CTypes { CWhile, CIf, CIfElse, CReturn, CAssign, CBlock, CCall } CTypes;



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
	ExpNot,
	ExpVar,
	ExpCall,
	ExpPrim,
	ExpCmp,
	ExpNew,
	ExpAccess
} ExpE;



typedef struct Exp{
	ExpE tag;
	union {
		struct {
			struct Exp *e1, *e2;
		} bin;
		struct Exp *unary;
		Constant* c;
		Var *var;
		struct {
			struct Exp *e;
			Type* t;
		} eNew;
		struct {
			const char* id;
			void* expList;
		} call;
		struct {
			struct Exp *e1, *e2;
			enum {EqEq, GTE, GT, LSE, LS, AND, OR } op;
		} cmp;
		struct {
			struct Exp *varExp, *indExp;
		} access;
	};
} Exp;

typedef struct ExpList
{
	Exp* e;
	struct ExpList* next; 
} ExpList ;

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

typedef struct DefVarL
{
	DefVar* dv;
	struct DefVarL* next;
} DefVarL;

typedef struct CommandL
{
	CTypes tag;
	struct CommandL* next;
	Exp* condExp;
	struct CommandL* cmdIf;
	struct CommandL* cmdElse;
	Exp* expLeft;
	Exp* expRight;
	Exp* retExp;
	void* block;
} CommandL;



// typedef struct CommandL
// {
// 	CTypes tag;
// 	struct CommandL* next;

// } CommandL;
typedef struct Block
{	
	DefVarL* dvl;
	CommandL* cl;
	
} Block;
typedef struct DefFunc
{
	const char* id;
	Type* retType;
	Parameter* params;
	Block* b;


} DefFunc;
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
void printDefVarList(DefVarL* dvl);
void printDefList(Def* d);
void printNameList(NameL* nl);
void printType(Type* t);
void printParams(Parameter* params);
void printCommandList(CommandL* cl);
void printBlock(Block* b);
void printExp(Exp* e);
void printVar(Var* v);
Constant* makeConstant(constantType t);
void notConst();
void minusConst();
void makeID();