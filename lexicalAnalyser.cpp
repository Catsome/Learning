#pragma warning(disable : 4996)

#include "lexicalAnalyser.h"

const char* RESERVERD_WORD[21] = { "program", "type", "integer", "char", "array", "of", "record","begin", "end",  //�ؼ�������
								"var", "procedure",  "if", "then", "else", "fi", "while", "do", "endwh",
								"read", "write", "return" };

char tokenString[1000];
int strLen = 0;
token* pList = NULL;	//tokenlistͷָ��
token* pTail = NULL;	//tokenlistβָ��
int lineCount = 1;	//��¼��ǰ����

int inAlphabet(char ch)
{     
	if ((ch == '\n' || ch == '\t' || ch == '\r' || ch == '\f' || ch == '\v' || ch == '\0') ||
		(ch >= ' ' && ch <= '}' && ch != '!' && ch != '\"' && ch != '#' && ch != '$'
			&& ch != '%' && ch != '>' && ch != '?' && ch != '@' && ch != '\\' 
			&& ch != '^' && ch != '_' && ch != '`' && ch != '|') )
		return 1;
	else
		return 0;
}

int isBlank(char ch)
{
	if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r' || ch == '\f' || ch == '\v' || ch == '\0')
		return 1;
	else
		return 0;
}

int isLetter(char ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return 1;
	else
		return 0;
}

int isNum(char ch)
{
	if (ch >= '0' && ch <= '9')
		return 1;
	else
		return 0;
}

int isOther(char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == ';' ||
		ch == '[' || ch == ']' || ch == '=' || ch == '<' || ch == ',' || isBlank(ch))
		return 1;
	else
		return 0;
}

void getNextChar()
{
	if (curChar >= 0) {	//�����������ҵ�����ungetNextʱ����ʹcurCharΪ-1
		tokenString[strLen] = lineBuf[curChar];
	}
	++strLen;
	++curChar;
}

void ungetNextChar()
{
	--strLen;
	--curChar;
}

charType getType(char ch)
{
	if (isLetter(ch))
		return charType(letter);
	else if (isNum(ch))
		return charType(num);
	else if (isOther(ch))
		return charType(other);
	else if (ch == ':')
		return charType(colon);
	else if (ch == '\'')
		return charType(squote);
	else if (ch == '{')
		return charType(lcomment);
	else if (ch == '.')
		return charType(dot);
	else
		return charType(error);
}

char* lexicalAnalyse()
{
	pList = getTokenList();
	char* listPath = listToFile();
	freeList();
	return listPath;
}

token* getTokenList()
{
	/*��Դ�ļ�*/
	infp = fopen(inPath, "r");

	state s = start;
	lexType lex = errortoken;
	while (1) {
		int endIndex = 0;
		for (int i = 0; i < 256 && !feof(infp); ++i) {	//ע��feof�ж�Ϊ�����ٶ�һ�Σ�������ɹ���ȡ
			fscanf(infp, "%c", &lineBuf[i]);
			endIndex = i;
		}
		if (feof(infp))	//��������������ǰ�ļ���ȡ�ͽ�������ҪΪ�����������ֹ��
			lineBuf[endIndex] = '\0';
		curChar = 0;

		while (curChar != BUFLEN) {
			char ch = lineBuf[curChar];
			if (s == incomment || inAlphabet(ch)) {	//�ַ������ַ������ַ�����ע��״̬�����д���  
				switch (s) {
					case start:	//��ʼ״̬��״̬ת��
						switch (getType(ch)) {
							case letter: s = inid; break;
							case num: s = innum; break;
							case other:
								s = done;
								if (isBlank(ch)) {
									lex = blank;
									if (ch == '\n')
										++lineCount;
								}
								else {
									lex = sseperator;
								}
								break;
							case colon: s = inassign; break;
							case lcomment: s = incomment; break;
							case dot: s = inrange; break;
							case squote: s = inchar; break;
							case error: s = errorstate; break;
						}
						break;
					case inid:	//��ʶ��״̬��״̬ת��
						switch (getType(ch)) {
							case letter:
							case num: s = inid; break;
							default: 
								s = done; 
								lex = id;
								ungetNextChar();
								break;
						}
						break;
					case innum:	//����״̬��״̬ת��
						switch (getType(ch)) {
							case num: s = innum; break;
							default:
								s = done;
								lex = unsignedint;
								ungetNextChar();
								break;
						}
						break;
					case inassign: //��ֵ״̬��״̬ת��
						switch (ch) {
							case '=': 
								s = done; 
								lex = dseperator;
								break;
							default: s = errorstate; break;
						}
						break;
					case incomment:	//ע��״̬��״̬ת��
						switch (ch) {
							case '}': 
								s = done; 
								lex = comment;
								break;
							case '\n': ++lineCount;
							default: s = incomment; break;
						}
						break;
					case inrange:	//�±�״̬��״̬ת��
						switch (ch) {
							case '.': 
								s = done;
								lex = index;
								break;
							default: 
								s = done;
								lex = end;
								ungetNextChar();
								break;	//���������־
						}
						break;
					case inchar:	//�ַ�״̬��״̬ת��
						switch (getType(ch)) {
							case letter:
							case num:
								s = endchar;
								break;
							default:
								s = errorstate;
								break;
							}
						break;
					case endchar:	//�ַ���ֹ״̬��״̬ת��
						switch (ch) {
							case '\'': 
								s = done;
								lex = character;
								break;
							default: 
								s = errorstate;
								break;
						}
						break;
				}
			}
			else {	//���ֲ����ַ����е��ַ�������
				s = errorstate;
			}
			getNextChar();

			if (s == done) {	//����token����������ʼ״̬
				if (lex != comment && lex != blank) {	//ע�Ͳ������հײ�����
					token* t = (token*)malloc(sizeof(token));

					t->lineshow = lineCount;

					tokenString[strLen] = '\0';

					if (lex == id) {	//�жϱ�ʶ���Ƿ��ǹؼ���
						for (int i = 0; i < 21; ++i) {
							if (strcmp(RESERVERD_WORD[i], tokenString) == 0)
								lex = reserved;
						}
					}

					//�������ͽ���lex��sem������
					if (lex == id) {
						strcpy(t->lex, "id");
						strcpy(t->sem, tokenString);
					}
					else if (lex == unsignedint) {
						strcpy(t->lex, "intc");
						strcpy(t->sem, tokenString);
					}
					else if (lex == character) {
						strcpy(t->lex, "charc");
						strcpy(t->sem, tokenString);
					}
					else if (lex == errortoken) {
						strcpy(t->lex, "error");
						strcpy(t->sem, tokenString);
					}
					else if (lex == reserved || lex == sseperator || lex == dseperator || lex == index || lex == end) {
						strcpy(t->lex, tokenString);
						strcpy(t->sem, "");
					}
					lex = errortoken;	//����״̬Ϊerror

					/*���������ɵ�token*/
					if (pList == NULL) {
						pList = t;
						pTail = pList;
						pTail->pNext = NULL;
					}
					else {
						pTail->pNext = t;
						pTail = pTail->pNext;
						pTail->pNext = NULL;
					}
				}

				/*����״̬�ʹʷ������ַ�������*/
				s = start;
				strLen = 0;
			}
			if (s == errorstate) {
				//������
				fclose(infp);
				return pList;
			}

			if (ch == '\0') {	//�ļ�β���������
				token* t = (token*)malloc(sizeof(token));
				t->lineshow = lineCount;
				strcpy(t->lex, "EOF");
				strcpy(t->sem, "");
				pTail->pNext = t;
				pTail = pTail->pNext;
				pTail->pNext = NULL;
				fclose(infp);
				return pList;
			}
		}//while
		lineBuf[0] = '\0';	//ģ����ջ�����
	}
}

char* listToFile()
{
	listfp = fopen(listPath, "w");
	token* p = pList;
	while (p) {
		fprintf(listfp, "%d %s %s\n", p->lineshow, p->lex, p->sem);
		p = p->pNext;
	}
	fclose(listfp);
	return listPath;
}

void freeList()
{
	token* p = pList->pNext;
	while (p) {
		free(pList);
		pList = p;
		p = p->pNext;
	}
	free(pList);
}
