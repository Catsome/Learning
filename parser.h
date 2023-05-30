/* parser.cpp: 此文件包含语法分析(递归下降子程序方法)相关内容的声明 */
#pragma once

#include <string.h>

#include "compiler.h"

/* 数据类型 */
enum NodeKind { ProK, PheadK, TypeK, VarK, ProcDecK, StmLK, DecK, StmtK, ExpK };		//语法树节点类型枚举
enum DecKind { ArrayK, CharK, IntegerK, RecordK, IdK };			//声明类型的枚举
enum StmtKind { IfK, WhileK, AssignK, ReadK, WriteK, CallK, ReturnK };		//语句类型的枚举
enum ExpKind { OpK, ConstK, IdK0 };	//表达式类型的枚举
enum ParamtType { ValparamtType, VarparamtType };	//过程传参类型的枚举
enum VarKind { IdV, ArrayMembV, FieldMembV };	//变量类别枚举

typedef struct {
	int lineno;
	char lex[10];
	char sem[25];
} Token;

struct TreeNode;
typedef struct TreeNode {
	TreeNode* child[3];
	TreeNode* sibling;
	int lineno;
	NodeKind nodekind;	//记录语法树节点类型
	union {	//记录语法树节点的具体类型
		DecKind dec;	//记录语法树节点的声明类型，当nodekind = DecK时有效
		StmtKind stmt;	//记录语法树节点的语句类型，当nodekind = StmtK时有效
		ExpKind exp;	//记录语法树节点的表达式类型，当nodekind = ExpK时有效
	} kind;
	int idnum;			//记录一个节点中的标志符的个数
	char name[10][MAX_ID_LEN];	//节点中的标志符的名字
	//table;	//符号表指针项
	char type_name[MAX_ID_LEN];		//记录类型名，当节点为声明类型，且类型是由类型标志符表示时有效
	struct {
		/* 数组类型的属性 */
		int low;
		int up;
		DecKind childkind;	//数组成员类型

		/* 过程的属性 */
		ParamtType paramt;

		/* 表达式的属性 */
		//op;		
		/*运算符类型,当语法树节点为“关系运算表达式”对应节点时，取值LT,EQ；
		当语法树节点为“加法运算简单表达式”对应节点时，取值PLUS,MINUS；
		当语法树节点为“乘法运算项”对应节点时，取值TIMES,OVER；其它情况下无效 */
		int val;	//记录语法树节点的数值,当语法树节点为“数字因子”对应的语法树节点时有效
		VarKind varkind;
		//type;	//记录语法树节点的检查类型，取值Void, Integer, Boolean, 为类型检查ExpType类型
	} attr;
} TreeNode;

/* 变量 */
extern TreeNode* pRoot;	//语法树根节点
extern Token curToken;	//当前处理的Token

/* 函数 */
TreeNode* parse();	//创建语法分析树，进行语法分析，并返回根节点
void readToken();	//读取一个token
int Match(const char*);	//匹配终极符

/** 产生式对应的递归向下子处理程序 **/
TreeNode* Program();

TreeNode* ProgramHead();

TreeNode* DeclarePart();

TreeNode* TypeDecpart();
TreeNode* TypeDec();
TreeNode* TypeDecList();
TreeNode* TypeDecMore();
void TypeId(TreeNode*);
void TypeDef(TreeNode*);
void BaseType(TreeNode*);
void StructureType(TreeNode*);
void ArrayType(TreeNode*);
void RecType(TreeNode*);
TreeNode* FieldDecList();
TreeNode* FieldDecMore();
void IdList(TreeNode*);
void IdMore(TreeNode*);

TreeNode* VarDecpart();
TreeNode* VarDec();
TreeNode* VarDecList();
TreeNode* VarDecMore();
void VarIdList(TreeNode*);
void VarIdMore(TreeNode*);

TreeNode* ProcDecpart();
TreeNode* ProcDec();
TreeNode* ProcDecMore();
TreeNode* ProcName();

void ParamList(TreeNode*);
TreeNode* ParamDecList();
TreeNode* ParamMore();
TreeNode* Param();
void FormList(TreeNode*);
void FidMore(TreeNode*);
TreeNode* ProcDecPart(); 
TreeNode* ProcBody();

TreeNode* ProgramBody();
TreeNode* StmList();
TreeNode* StmMore();
TreeNode* Stm();
TreeNode* AssCall();
TreeNode* AssignmentRest();
TreeNode* ConditionalStm();
TreeNode* LoopStm();
TreeNode* InputStm();
TreeNode* Invar();
TreeNode* OutputStm();
TreeNode* ReturnStm();
TreeNode* CallStmRest();
TreeNode* ActParamList();
TreeNode* ActParamMore();

TreeNode* RelExp();
TreeNode* OtherRelE();
TreeNode* Exp();
TreeNode* OtherTerm();

TreeNode* Term();
TreeNode* OtherFactor();
TreeNode* Factor();
TreeNode* Variable();
void VariMore(TreeNode*);
TreeNode* FieldVar();
void FieldVarMore(TreeNode*);
TreeNode* CmpOp();
TreeNode* AddOp();
TreeNode* MultOp();
