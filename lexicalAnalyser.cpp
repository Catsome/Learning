#pragma warning(disable : 4996)

#include "lexicalAnalyser.h"

const char* RESERVERD_WORD[21] = { "program", "type", "integer", "char", "array", "of", "record","begin", "end",  //关键字数组
								"var", "procedure",  "if", "then", "else", "fi", "while", "do", "endwh",
								"read", "write", "return" };

char tokenString[1000];
int strLen = 0;
token* pList = NULL;	//tokenlist头指针
token* pTail = NULL;	//tokenlist尾指针
int lineCount = 1;	//记录当前行数

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
	if (curChar >= 0) {	//缓冲区更新且调用了ungetNext时可能使curChar为-1
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
	/*打开源文件*/
	infp = fopen(inPath, "r");

	state s = start;
	lexType lex = errortoken;
	while (1) {
		int endIndex = 0;
		for (int i = 0; i < 256 && !feof(infp); ++i) {	//注意feof判断为真后会再读一次，但不会成功读取
			fscanf(infp, "%c", &lineBuf[i]);
			endIndex = i;
		}
		if (feof(infp))	//若在填满缓冲区前文件读取就结束，需要为缓冲区添加终止符
			lineBuf[endIndex] = '\0';
		curChar = 0;

		while (curChar != BUFLEN) {
			char ch = lineBuf[curChar];
			if (s == incomment || inAlphabet(ch)) {	//字符属于字符表中字符或处于注释状态，进行处理  
				switch (s) {
					case start:	//开始状态的状态转换
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
					case inid:	//标识符状态的状态转换
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
					case innum:	//数字状态的状态转换
						switch (getType(ch)) {
							case num: s = innum; break;
							default:
								s = done;
								lex = unsignedint;
								ungetNextChar();
								break;
						}
						break;
					case inassign: //赋值状态的状态转换
						switch (ch) {
							case '=': 
								s = done; 
								lex = dseperator;
								break;
							default: s = errorstate; break;
						}
						break;
					case incomment:	//注释状态的状态转换
						switch (ch) {
							case '}': 
								s = done; 
								lex = comment;
								break;
							case '\n': ++lineCount;
							default: s = incomment; break;
						}
						break;
					case inrange:	//下标状态的状态转换
						switch (ch) {
							case '.': 
								s = done;
								lex = index;
								break;
							default: 
								s = done;
								lex = end;
								ungetNextChar();
								break;	//程序结束标志
						}
						break;
					case inchar:	//字符状态的状态转换
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
					case endchar:	//字符终止状态的状态转换
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
			else {	//出现不在字符表中的字符，出错
				s = errorstate;
			}
			getNextChar();

			if (s == done) {	//保存token并重置至开始状态
				if (lex != comment && lex != blank) {	//注释不处理，空白不处理
					token* t = (token*)malloc(sizeof(token));

					t->lineshow = lineCount;

					tokenString[strLen] = '\0';

					if (lex == id) {	//判断标识符是否是关键字
						for (int i = 0; i < 21; ++i) {
							if (strcmp(RESERVERD_WORD[i], tokenString) == 0)
								lex = reserved;
						}
					}

					//根据类型进行lex和sem的生成
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
					lex = errortoken;	//重置状态为error

					/*保存新生成的token*/
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

				/*重置状态和词法分析字符串长度*/
				s = start;
				strLen = 0;
			}
			if (s == errorstate) {
				//错误处理
				fclose(infp);
				return pList;
			}

			if (ch == '\0') {	//文件尾，处理结束
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
		lineBuf[0] = '\0';	//模拟清空缓冲区
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
