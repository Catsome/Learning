/* compiler.h �Ķ��� */

#include "compiler.h"

char lineBuf[BUFLEN];	//���뻺����
int curChar = 0;	//��������ǰ�ַ����±�

char inPath[50];	//Դ�ļ���ַ
char listPath[50];	//���Token�ļ���ַ

FILE* infp;
FILE* listfp;
FILE* outfp;
