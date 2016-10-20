



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


} DefFunc;

typedef enum BType { WInt, WFloat, WChar} BType;
typedef enum Types { base, array} Types;
typedef struct Type
{
	Types tag;
	BType b;
	struct Type* of; //array of
	
} Type;
typedef struct NameL
{
	const char* name;
	struct NameL* next;
} NameL;
typedef struct DefVar
{
	Type t;
	NameL* next;
} DefVar;

typedef struct Def {
	defType tag;
	union {
		DefFunc* f;
		DefVar* v;
	} u;
	struct Def* next;
} Def;

typedef struct progNode {
	int i;
	double v;
	Def* next;
} progNode;
extern progNode* globalTree;
void printTree();
Constant* makeConstant(constantType t);
void notConst();
void minusConst();
void makeID();