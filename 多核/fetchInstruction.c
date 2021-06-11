//
//  fetchInstruction.c
//  CPU
//
//

#include "fetchInstruction.h"
#include "Header.h"
char Code1[100][9],Code2[100][5],Code3[100][5],Code4[100][17],CODESET[100][17],codeSegment[100][33],CODE[100][32];//Code1保存指令前8位，Code2保存9-12位，Code3保存13-16位，Code4保存后16位，CODESET保存前16位，codeSegment和CODE保存完整指令

//获取指令
int fetchInstruction(int tid)
{
    FILE *fp;
    int  i=0,j=0,sum=0;
    char CODE[100][32];

    //判断文件是否打开失败
    if(tid == 1)
    {
    if ( (fp = fopen("dict1.dic", "rt")) == NULL ) {
        puts("Fail to open file!");
        exit(0);
        }
    }
    if(tid == 2)
    {
    if ( (fp = fopen("dict2.dic", "rt")) == NULL ) {
        puts("Fail to open file!");
        exit(0);
        }
    }

    //循环读取文件的每一行数据
    while( fgets(CODE[i], N, fp) != NULL ) {
        if(CODE[i][0]!='0')
            break;
        i++;
    }

    sum=i;//指令行数
    for(i=0;i<sum;i++)
    {
        for(j=0;j<8;j++)
            Code1[i][j]=CODE[i][j];//前8位
        for(j=0;j<4;j++)
            Code2[i][j]=CODE[i][j+8];//第9到13位
        for(j=0;j<4;j++)
            Code3[i][j]=CODE[i][j+12];//第13-16位
        for(j=0;j<16;j++)
            Code4[i][j]=CODE[i][j+16];//后16位 即立即数
        for(j=0;j<16;j++)
            CODESET[i][j]=CODE[i][j];//前16位
        for(j=0;j<32;j++)
        codeSegment[i][j]=CODE[i][j];//整个指令
        Code1[i][8]='\0';
        Code2[i][4]='\0';
        Code3[i][4]='\0';
        CODESET[i][16]='\0';
        codeSegment[i][32]='\0';
    }

    //操作结束后关闭文件
    fclose(fp);
    return sum;//返回行数
}
