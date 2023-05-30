// CompilerCourseDesign.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#pragma warning(disable : 4996)

#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "lexicalAnalyser.h"
#include "parser.h"

int main()
{
    extern char inPath[50];
    extern char listPath[50];
    printf("输入目标文件路径:");
    scanf("%s", inPath);
    printf("输入Token文件输出路径:");
    scanf("%s", listPath);
    /*strcpy(inPath, "in0.txt");
    strcpy(listPath, "list.txt");*/
    printf("进行词法分析...\n");
    lexicalAnalyse();
    printf("词法分析完成，输出文件 %s.\n", listPath);
    /*printf("进行语法分析...\n");
    parse();
    printf("语法分析完成.");*/
    return 0;
}
