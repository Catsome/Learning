#pragma once
/* compiler.h : �����������ϵͳ��ȫ�ֱ������� */

#include <stdio.h>

#define BUFLEN 256	//����������
#define MAX_ID_LEN 25	//��ʶ����󳤶�

extern char lineBuf[BUFLEN];	//���뻺����
extern int curChar;	//��������ǰ�ַ����±�

extern char inPath[50];	//Դ�ļ���ַ
extern char listPath[50];	//���Token�ļ���ַ

extern FILE* infp;
extern FILE* listfp;
extern FILE* outfp;
