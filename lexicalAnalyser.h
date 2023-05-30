#pragma once
//lexicalAnalyser.h : ���ļ������ʷ��������ı��������̶���

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "compiler.h"

//��������
enum charType {num, letter, colon, dot, lcomment, other, squote, error};	//����SNL�ַ�������
enum lexType {id, reserved, unsignedint, sseperator, dseperator, comment, character, index, end, errortoken, blank};	//����SNL�������͵�ö������
enum state {start, inid, innum, inassign, incomment, inchar, inrange, endchar, errorstate, done};	//����ʷ�������DFM״̬

struct token {	//����SNL��Token�ṹ��
	int lineshow;
	char lex[25];
	char sem[25];
	token* pNext;
};

//����
extern const char* RESERVERD_WORD[21];

//����
extern char tokenString[1000];
extern int strLen;
extern token* pList;	//tokenlistͷָ��
extern token* pTail;	//tokenlistβָ��
extern int lineCount;	//��¼��ǰ����

//����

int inAlphabet(char ch);	//�ж��ַ��Ƿ�λ���ַ�����
int isBlank(char ch);	//�ж��ַ��Ƿ��ǿհ��ַ�
int isLetter(char ch);	//�ж��ַ��Ƿ�����ĸ
int isNum(char ch);		//�ж��ַ��Ƿ�������
int isOther(char ch);	//�ж��ַ��Ƿ����������⴦��ĵ��ֽ��

void getNextChar();		//��ȡ��������һ�ַ�
void ungetNextChar();
charType getType(char ch);

char* lexicalAnalyse();		//���дʷ�����������tokenlist�ļ���·��
token* getTokenList();				//��ȡtokenlist
char* listToFile();					//��tokenlist�������ļ�
void freeList();					//ʩ��tokenlist�Ŀռ�
