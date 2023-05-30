#pragma once
//lexicalAnalyser.h : 此文件包含词法分析器的变量及过程定义

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "compiler.h"

//数据类型
enum charType {num, letter, colon, dot, lcomment, other, squote, error};	//定义SNL字符的类型
enum lexType {id, reserved, unsignedint, sseperator, dseperator, comment, character, index, end, errortoken, blank};	//定义SNL单词类型的枚举类型
enum state {start, inid, innum, inassign, incomment, inchar, inrange, endchar, errorstate, done};	//定义词法分析的DFM状态

struct token {	//定义SNL的Token结构体
	int lineshow;
	char lex[25];
	char sem[25];
	token* pNext;
};

//常量
extern const char* RESERVERD_WORD[21];

//变量
extern char tokenString[1000];
extern int strLen;
extern token* pList;	//tokenlist头指针
extern token* pTail;	//tokenlist尾指针
extern int lineCount;	//记录当前行数

//函数

int inAlphabet(char ch);	//判断字符是否位于字符表中
int isBlank(char ch);	//判断字符是否是空白字符
int isLetter(char ch);	//判断字符是否是字母
int isNum(char ch);		//判断字符是否是数字
int isOther(char ch);	//判断字符是否是无需特殊处理的单分界符

void getNextChar();		//获取缓冲区下一字符
void ungetNextChar();
charType getType(char ch);

char* lexicalAnalyse();		//进行词法分析，返回tokenlist文件的路径
token* getTokenList();				//获取tokenlist
char* listToFile();					//将tokenlist保存至文件
void freeList();					//施放tokenlist的空间
