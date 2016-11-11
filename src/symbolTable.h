#if !defined(tree_h)
	#include "tree.h"
	#define tree_h
#endif

int typeTree(progNode* p); 
int typeDefVar(DefVar* dv);
int typeDefFunc(DefFunc* df);
int typeDefVarList(DefVarL* dvl);
int typeDefList(Def* d);
int typeNameList(NameL* nl);
int typeType(Type* t);
int typeParams(Parameter* params);
int typeCommandList(CommandL* cl);
int typeBlock(Block* b);
int typeExp(Exp* e);
int typeVar(Var* v);
int typeConstant(Constant* c);
int typeExpList(ExpList* el);