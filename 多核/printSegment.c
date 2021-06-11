//
//  printSegment.c
//  CPU
//
//

#include "printSegment.h"
#include "fetchInstruction.h"
#include "translationRef.h"
#include "Header.h"

void printSegment(int sum)
{
    printf("\n");
    printf("codeSegment :\n");//代码段
    int i,j,n=0;
    for(i=0;i<8;i++)//线程1代码段
    {
        for(j=0;j<8;j++)
        {
            if(n<sum)
            { 
                printf("%d ",translationRef(codeSegment[n]));//有指令则打印
                n++;
            }
            else
                printf("0 ");//无则打印0
        }
        printf("\n");
    }
    n=0;
    for(i=9;i<16;i++)//线程2代码段
    {
        for(j=0;j<8;j++)
        {
            if(n<sum)
            {
                printf("%d ",translationRef(codeSegment[n]));
                n++;
            }
            else
                printf("0 ");
        }
        printf("\n");
    }
    n=0;
    printf("\n");
    printf("dataSegment :\n");//数据段
    for(i=0;i<16;i++)
    {
        for(j=0;j<16;j++)
        {
            printf("%d ",Memory[16384+n]);
            n=n+2;
        }
        printf("\n");
    }
}
