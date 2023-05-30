/* parser.cpp: ���ļ������﷨����(�ݹ��½��ӳ��򷽷�)������ݵ����� */
#pragma once

#include <string.h>

#include "compiler.h"

/* �������� */
enum NodeKind { ProK, PheadK, TypeK, VarK, ProcDecK, StmLK, DecK, StmtK, ExpK };		//�﷨���ڵ�����ö��
enum DecKind { ArrayK, CharK, IntegerK, RecordK, IdK };			//�������͵�ö��
enum StmtKind { IfK, WhileK, AssignK, ReadK, WriteK, CallK, ReturnK };		//������͵�ö��
enum ExpKind { OpK, ConstK, IdK0 };	//���ʽ���͵�ö��
enum ParamtType { ValparamtType, VarparamtType };	//���̴������͵�ö��
enum VarKind { IdV, ArrayMembV, FieldMembV };	//�������ö��

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
	NodeKind nodekind;	//��¼�﷨���ڵ�����
	union {	//��¼�﷨���ڵ�ľ�������
		DecKind dec;	//��¼�﷨���ڵ���������ͣ���nodekind = DecKʱ��Ч
		StmtKind stmt;	//��¼�﷨���ڵ��������ͣ���nodekind = StmtKʱ��Ч
		ExpKind exp;	//��¼�﷨���ڵ�ı��ʽ���ͣ���nodekind = ExpKʱ��Ч
	} kind;
	int idnum;			//��¼һ���ڵ��еı�־���ĸ���
	char name[10][MAX_ID_LEN];	//�ڵ��еı�־��������
	//table;	//���ű�ָ����
	char type_name[MAX_ID_LEN];		//��¼�����������ڵ�Ϊ�������ͣ��������������ͱ�־����ʾʱ��Ч
	struct {
		/* �������͵����� */
		int low;
		int up;
		DecKind childkind;	//�����Ա����

		/* ���̵����� */
		ParamtType paramt;

		/* ���ʽ������ */
		//op;		
		/*���������,���﷨���ڵ�Ϊ����ϵ������ʽ����Ӧ�ڵ�ʱ��ȡֵLT,EQ��
		���﷨���ڵ�Ϊ���ӷ�����򵥱��ʽ����Ӧ�ڵ�ʱ��ȡֵPLUS,MINUS��
		���﷨���ڵ�Ϊ���˷��������Ӧ�ڵ�ʱ��ȡֵTIMES,OVER�������������Ч */
		int val;	//��¼�﷨���ڵ����ֵ,���﷨���ڵ�Ϊ���������ӡ���Ӧ���﷨���ڵ�ʱ��Ч
		VarKind varkind;
		//type;	//��¼�﷨���ڵ�ļ�����ͣ�ȡֵVoid, Integer, Boolean, Ϊ���ͼ��ExpType����
	} attr;
} TreeNode;

/* ���� */
extern TreeNode* pRoot;	//�﷨�����ڵ�
extern Token curToken;	//��ǰ�����Token

/* ���� */
TreeNode* parse();	//�����﷨�������������﷨�����������ظ��ڵ�
void readToken();	//��ȡһ��token
int Match(const char*);	//ƥ���ռ���

/** ����ʽ��Ӧ�ĵݹ������Ӵ������ **/
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
