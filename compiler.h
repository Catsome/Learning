#pragma once
/* compiler.h : 存放整个编译系统的全局变量声明 */

#include <stdio.h>

#define BUFLEN 256	//缓冲区长度
#define MAX_ID_LEN 25	//标识符最大长度

extern char lineBuf[BUFLEN];	//输入缓冲区
extern int curChar;	//缓冲区当前字符的下标

extern char inPath[50];	//源文件地址
extern char listPath[50];	//输出Token文件地址

extern FILE* infp;
extern FILE* listfp;
extern FILE* outfp;
