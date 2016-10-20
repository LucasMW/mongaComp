
typedef struct pNode {
	int i;
	double v;
} pNode;


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

typedef struct DefVar
{


} DefVar;

typedef struct Def {
	defType tag;
	union {
		DefFunc* f;
		DefVar* v;
	} u;
	struct Def* next;
} Def;
extern pNode* globalTree;
void printTree();
Constant* makeConstant(constantType t);
void notConst();
void minusConst();
void makeID();