#include "parser.h"
#include <stdlib.h>

#pragma warning(disable : 4996)

TreeNode* pRoot;	//语法树根节点
Token curToken;	//当前处理的Token
char tmp[50];	//储存需要保存的sem

void readToken() {
	if (!feof(listfp)) {
		scanf("%d%s", &curToken.lineno, curToken.lex);
		if (strcmp(curToken.lex, "id") == 0 || strcmp(curToken.lex, "intc") == 0 || strcmp(curToken.lex, "charc") == 0) {
			scanf("%s", curToken.sem);
		}
		else {
			strcpy(curToken.sem, NULL);
		}
	}
}

int Match(const char* key)
{
	if (strcmp(curToken.sem, key) == 0) {	//匹配成功，读入下一个token
		readToken();
		strcpy(tmp, curToken.sem);
		return 1;
	}
	else
		return 0;
}

TreeNode* parse() {
	listfp = fopen(listPath, "r");
	readToken();
	pRoot = Program();
	if (strcmp(curToken.lex, "EOF") != 0) {		//文件提前结束，进入错误处理
		//error
	}
	else {
		return pRoot;
	}
}

TreeNode* Program() {
	TreeNode* p0 = (TreeNode*)malloc(sizeof(TreeNode));
	p0->lineno = curToken.lineno;
	p0->nodekind = ProK;
	TreeNode* p1 = ProgramHead();
	TreeNode* p2 = DeclarePart();
	TreeNode* p3 = ProgramBody();

	if (p1) {
		p0->child[0] = p1;
	}
	else {
		//error
	}

	if (p2) {
		p0->child[1] = p2;
	}
	else {
		//error
	}

	if (p3) {
		p0->child[2] = p3;
	}
	else {

	}

	return p0;
}

TreeNode* ProgramHead() {
	TreeNode* p0 = (TreeNode*)malloc(sizeof(TreeNode));
	p0->lineno = curToken.lineno;
	p0->nodekind = PheadK;
	p0->idnum = 1;

	if (Match("program")) {
		;
	}
	else {
		//error
	}
	
	if (Match("id")) {
		strcpy(p0->name[0], tmp);
	}
	else {
		//error
	}

	return p0;
}

TreeNode* DeclarePart()
{
	TreeNode* p0 = NULL, * p1 = NULL, * p2 = NULL;
	TreeNode* tp0, * tp1, * tp2;

	tp0 = TypeDecpart();
	tp1 = VarDecpart();
	tp2 = ProcDecpart();

	if (tp0) {
		p0 = (TreeNode*)malloc(sizeof(TreeNode));
		p0->child[0] = tp0;
		p0->nodekind = TypeK;
		p0->lineno = curToken.lineno;
	}
	if (tp1) {
		p1 = (TreeNode*)malloc(sizeof(TreeNode));
		p1->child[0] = tp1;
		p1->nodekind = VarK;
		p1->lineno = curToken.lineno;
	}
	if (tp2) {
		p2 = (TreeNode*)malloc(sizeof(TreeNode));
		p2->child[0] = tp2;
		p2->nodekind = ProcDecK;
		p2->lineno = curToken.lineno;
	}

	if (p1) {
		p1->sibling = p2;
		if (p0) {
			p0->sibling = p1;
		}
		else {
			p0 = p1;
		}
	}
	else {
		if (p0)
			p0->sibling = p2;
		else
			p0 = p2;
	}

	return p0;
}

TreeNode* TypeDecpart()
{
	TreeNode* p0;

	if (strcmp(curToken.lex, "type") == 0) {
		p0 = TypeDec();
		return 0;
	}
	else if (strcmp(curToken.lex, "var") == 0 || strcmp(curToken.lex, "procedure") == 0 || strcmp(curToken.lex, "begin") == 0)
		return nullptr;
	else {
		//error
	}
}

TreeNode* TypeDec()
{
	TreeNode* p0;

	if (Match("type")) {
		;
	}
	else {
		//error
	}

	if (p0 = TypeDecList()) {
		;
	}
	else {
		//error
	}

	return p0;
}

TreeNode* TypeDecList()
{
	TreeNode* p0 = (TreeNode*)malloc(sizeof(TreeNode));
	p0->lineno = curToken.lineno;
	p0->nodekind = DecK;

	TypeId(p0);
	Match("=");
	TypeDef(p0);
	Match(";");
	TreeNode* p1 = TypeDecMore();
	if (p1) {
		p0->sibling = p1;
	}

	return p0;
}

TreeNode* TypeDecMore()
{
	TreeNode* p0 = NULL;

	if (strcmp(curToken.lex, "var") == 0 || strcmp(curToken.lex, "procedure") == 0 || strcmp(curToken.lex, "begin") == 0) {
		return p0;
	}

	if (strcmp(curToken.lex, "id") == 0) {
		p0 = TypeDecList();
	}
	else {
		//error
	}

	return p0;
}

void TypeId(TreeNode* t)
{
	if (Match("id")) {
		strcpy(t->name[t->idnum++], tmp);
	}
}

void TypeDef(TreeNode* t)
{
	if (t) {
		if (strcmp(curToken.lex, "integer") == 0 || strcmp(curToken.lex, "char") == 0) {
			BaseType(t);
		}
		else if (strcmp(curToken.lex, "array") == 0 || strcmp(curToken.lex, "record") == 0) {
			StructureType(t);
		}
		else if (strcmp(curToken.lex, "id") == 0) {
			Match("id");
			t->kind.dec = DecKind(IdK);
			strcpy(t->type_name, tmp);
		}
		else
			return;
	}
	else
		return;
}

void BaseType(TreeNode* t)
{
	if (strcmp(curToken.lex, "integer") == 0) {
		Match("integer");
		t->kind.dec = DecKind(IntegerK);
	}
	else if (strcmp(curToken.lex, "char") == 0) {
		Match("char");
		t->kind.dec = DecKind(CharK);
	}
	return;
}

void StructureType(TreeNode* t)
{
	if (strcmp(curToken.lex, "array") == 0) {
		ArrayType(t);
	}
	else if (strcmp(curToken.lex, "record") == 0) {
		t->kind.dec = RecordK;
		RecType(t);
	}
	return;
}

void ArrayType(TreeNode* t)
{
	Match("array");
	Match("[");
	if (Match("intc")) {
		t->attr.low = atoi(tmp);
	}
	Match("..");
	if (Match("intc")) {
		t->attr.up = atoi(tmp);
	}
	Match("]");
	Match("of");
	BaseType(t);
	t->attr.childkind = t->kind.dec;
	t->kind.dec = ArrayK;
}

void RecType(TreeNode* t)
{
	Match("record");
	TreeNode* p = FieldDecList();
	if (p) {
		t->child[0] = p;
	}
	else {
		//error
	}
	Match("end");
}

TreeNode* FieldDecList()
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	t->nodekind = DecK;
	TreeNode *p = NULL;

	if (t) {
		if (strcmp(curToken.lex, "integer") == 0 || strcmp(curToken.lex, "char") == 0) {
			BaseType(t);
			IdList(t);
			Match(";");
			p = FieldDecMore();
		}
		else if (strcmp(curToken.lex, "array") == 0) {
			ArrayType(t);
			IdList(t);
			Match(";");
			p = FieldDecMore();
		}
		else
			readToken();	//error
		t->sibling = p;
	}
	return t;
}

TreeNode* FieldDecMore()
{
	TreeNode* t = NULL;
	if (strcmp(curToken.lex, "end") != 0) {
		if (strcmp(curToken.lex, "integer") == 0 || strcmp(curToken.lex, "char") == 0 || strcmp(curToken.lex, "array") == 0)
			t = FieldDecList();
		else
			readToken();	//error
	}
	return t;
}

void IdList(TreeNode* t)
{
	if (Match("id")) {
		strcpy(t->name[t->idnum], tmp);
		t->idnum++;
		IdMore(t);
	}
	else
		;//error
}

void IdMore(TreeNode* t)
{
	if (strcmp(curToken.lex, ";") != 0) {
		if (strcmp(curToken.lex, ",") == 0) {
			Match(",");
			IdList(t);
		}
		else
			readToken();
	}
}

TreeNode* VarDecpart()
{
	if (strcmp(curToken.lex, "procedure") == 0)
		return NULL;
	if (strcmp(curToken.lex, "begin") == 0)
		return NULL;
	TreeNode* t = NULL;
	if (strcmp(curToken.lex, "var") == 0)
		t = VarDec();
	else
		readToken();
	return t;
}

TreeNode* VarDec()
{
	Match("var");
	TreeNode* t = VarDecList();
	if (t == NULL)
		;//error
	return t;
}

TreeNode* VarDecList()
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	TreeNode* p = NULL;
	if (t) {
		TypeDef(t);
		VarIdList(t);
		Match(";");
		p = VarDecMore();
		t->sibling = p;
	}
	return t;
}

TreeNode* VarDecMore()
{
	TreeNode* t = NULL;
	if (strcmp(curToken.lex, "procedure") == 0 || strcmp(curToken.lex, "begin") == 0) {
		return t;
	}
	if (strcmp(curToken.lex, "integer") == 0 || strcmp(curToken.lex, "char") == 0 ||
		strcmp(curToken.lex, "array") == 0 || strcmp(curToken.lex, "record") == 0 ||
		strcmp(curToken.lex, "id") == 0) {
		t = VarDecList();
	}
	return t;
}

void VarIdList(TreeNode* t)
{
	if (strcmp(curToken.lex, "id")) {
		Match("id");
		strcpy(t->name[t->idnum], tmp);
		t->idnum++;
	}
	else {
		//error
		readToken();
	}
	VarIdMore(t);
}

void VarIdMore(TreeNode* t)
{
	if (strcmp(curToken.lex, ";") != 0) {
		if (strcmp(curToken.lex, ",") == 0) {
			Match(",");
			VarIdList(t);
		}
		else {
			//error
			readToken();
		}
	}
}

TreeNode* ProcDecpart()
{
	TreeNode* t = NULL;
	if (strcmp(curToken.lex, "begin") != 0) {
		if (strcmp(curToken.lex, "procedure") == 0) {
			t = ProcDec();
		}
		else {
			readToken();
		}
	}
	return t;
}

TreeNode* ProcDec()
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	t->nodekind = ProcDecK;
	Match("procedure");
	if (t) {
		if (Match("id")) {
			strcpy(t->name[t->idnum], tmp);
			t->idnum++;
		}
		Match("(");
		ParamList(t);
		Match(")");
		Match(";");
		TreeNode* t1, * t2;
		t1 = ProcDecPart();
		t2 = ProcBody();
		if (t1)
			t->child[0] = t1;
		if (t2)
			t->child[1] = t2;
	}
	return t;
}

void ParamList(TreeNode* t)
{
	TreeNode* p = NULL;
	if (Match(")")) {
		return;
	}

	if (strcmp(curToken.lex, "integer") == 0 || strcmp(curToken.lex, "char") == 0 ||
		strcmp(curToken.lex, "record") == 0 || strcmp(curToken.lex, "array") == 0 ||
		strcmp(curToken.lex, "id") == 0 || strcmp(curToken.lex, "var") == 0) {
		p = ParamDecList();
		if (p)
			t->child[0] = p;
	}
	else {
		readToken();
	}
	return;
}

TreeNode* ParamDecList()
{
	TreeNode* t = Param();
	TreeNode* p = ParamMore();
	if (t && p)
		t->sibling = p;
	return t;
}

TreeNode* ParamMore()
{
	TreeNode* t = NULL;
	if (strcmp(curToken.lex, ")") != 0) {
		if (Match(";")) {
			t = ParamDecList();
			if (t == NULL) {
				//error
			}
		}
		else {
			readToken();
			//error
		}
	}
	return t;
}

TreeNode* Param()
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	t->nodekind = DecK;
	if (t) {
		if (strcmp(curToken.lex, "integer") == 0 || strcmp(curToken.lex, "char") == 0 ||
			strcmp(curToken.lex, "record") == 0 || strcmp(curToken.lex, "array") == 0 || 
			strcmp(curToken.lex, "id") == 0) {
			t->attr.paramt = ValparamtType;
			TypeDef(t);
			FormList(t);
		}
		else if (strcmp(curToken.lex, "var") == 0) {
			Match("var");
			t->attr.paramt = VarparamtType;
			TypeDef(t);
			FormList(t);
		}
		else {
			readToken();
			//error
		}
	}
	return t;
}

void FormList(TreeNode* t)
{
	if (Match("id")) {
		strcpy(t->name[t->idnum], tmp);
		t->idnum++;
	}
	FidMore(t);
}

void FidMore(TreeNode* t)
{
	if (strcmp(curToken.lex, ";") == 0 || strcmp(curToken.lex, ")") == 0) {
		return;
	}

	if (strcmp(curToken.lex, ",") == 0) {
		Match(",");
		FormList(t);
	}
	else {
		//error
		readToken();
	}
	return;
}

TreeNode* ProcDecPart()
{
	TreeNode* t = DeclarePart();
	if (t)
		return t;
	else {
		//error
		return NULL;
	}
}

TreeNode* ProcBody()
{
	TreeNode* t = ProgramBody();
	if (!t) {
		//error
	}
	return t;
}

TreeNode* ProgramBody()
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	t->nodekind = StmLK;
	Match("begin");
	t->child[0] = StmList();
	Match("end");
	return t;
}

TreeNode* StmList()
{
	TreeNode* t = Stm();
	if (t) {
		TreeNode* p = StmMore();
		if (p)
			t->sibling = p;
		return t;
	}
	else
		return NULL;
}

TreeNode* StmMore()
{
	TreeNode* t = NULL;
	if (strcmp(curToken.lex, "end") == 0 || strcmp(curToken.lex, "endwh") == 0)
		return NULL;

	if (strcmp(curToken.lex, ";") == 0) {
		Match(";");
		t = StmList();
		return t;
	}
	else {
		readToken();
		return NULL;
	}
}

TreeNode* Stm()
{
	TreeNode* t = NULL;
	if (strcmp(curToken.lex, "if") == 0) {
		t = ConditionalStm();
	}
	else if (strcmp(curToken.lex, "while") == 0)
		t = LoopStm();
	else if (strcmp(curToken.lex, "return") == 0)
		t = ReturnStm();
	else if (strcmp(curToken.lex, "read") == 0)
		t = InputStm();
	else if (strcmp(curToken.lex, "write") == 0)		t = OutputStm();
	else if (Match("id")) {
		//tmp未使用
		t = AssCall();
	}
	else {
		//error
		readToken();
	}
	return t;
}

TreeNode* AssCall()
{
	TreeNode* t = NULL;
	if (strcmp(curToken.lex, ":=") == 0)
		t = AssignmentRest();
	else if (strcmp(curToken.lex, "(") == 0)
		t = CallStmRest();
	else {
		//error
		readToken();
	}
	return t;
}

TreeNode* AssignmentRest()
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	t->lineno = curToken.lineno;
	t->nodekind = StmtK;
	t->kind.stmt = AssignK;

	if (strcmp(curToken.lex, "=") == 0) {
		t->child[0] = Exp();
		strcpy(t->name[0], tmp);
		t->idnum++;
	}
	else
		return NULL;
}

TreeNode* ConditionalStm()
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	t->lineno = curToken.lineno;
	t->nodekind = StmtK;
	t->kind.stmt = IfK;

	Match("if");
	t->child[0] = Exp();
	Match("then");
	t->child[1] = StmList();
	Match("else");
	t->child[2] = StmList();
	Match("fi");
	return t;
}

TreeNode* LoopStm()
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	t->lineno = curToken.lineno;
	t->nodekind = StmtK;
	t->kind.stmt = WhileK;

	t->child[0] = Exp();
	Match("do");
	t->child[1] = StmList();
	return t;
}

TreeNode* InputStm()
{

	return nullptr;
}

TreeNode* Invar()
{
	return nullptr;
}

TreeNode* OutputStm()
{
	return nullptr;
}

TreeNode* ReturnStm()
{
	return nullptr;
}

TreeNode* CallStmRest()
{
	return nullptr;
}

TreeNode* ActParamList()
{
	return nullptr;
}

TreeNode* ActParamMore()
{
	return nullptr;
}

TreeNode* RelExp()
{
	return nullptr;
}

TreeNode* OtherRelE()
{
	return nullptr;
}

TreeNode* Exp()
{
	return nullptr;
}

TreeNode* OtherTerm()
{
	return nullptr;
}

TreeNode* Term()
{
	return nullptr;
}

TreeNode* OtherFactor()
{
	return nullptr;
}

TreeNode* Factor()
{
	return nullptr;
}

TreeNode* Variable()
{
	return nullptr;
}


void VariMore(TreeNode* t)
{
	if (strcmp(curToken.lex, ":=") == 0 || strcmp(curToken.lex, "*") == 0 ||
		strcmp(curToken.lex, "=") == 0 || strcmp(curToken.lex, "end") == 0) {
		return;
	}
	else if (strcmp(curToken.lex, "[")) {
		Match("[");
		t->child[0] = Exp();
		t->attr.varkind = ArrayMembV;
		t->child[0]->attr.varkind = IdV;
	}
	else {
		return;
	}
}

TreeNode* FieldVar()
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	t->lineno = curToken.lineno;
	t->nodekind = ExpK;
	if (Match("id")) {
		FieldVarMore(t);
	}
	return t;
}

void FieldVarMore(TreeNode* t)
{
	if (strcmp(curToken.lex, ":=") == 0 || strcmp(curToken.lex, "*") == 0 || 
		strcmp(curToken.lex, "=") == 0 || strcmp(curToken.lex, "end") == 0) {
		return;
	}
	else if (strcmp(curToken.lex, "[") == 0) {
		Match("[");
		t->child[0] = Exp();
		t->attr.varkind = ArrayMembV;
		Match("]");
	}
	else {
		//error
		readToken();
	}
}
