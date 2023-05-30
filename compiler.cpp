/* compiler.h 的定义 */

#include "compiler.h"

char lineBuf[BUFLEN];	//输入缓冲区
int curChar = 0;	//缓冲区当前字符的下标

char inPath[50];	//源文件地址
char listPath[50];	//输出Token文件地址

FILE* infp;
FILE* listfp;
FILE* outfp;
