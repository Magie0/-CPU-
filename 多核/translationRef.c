//
//  translationRef.c
//  CPU
//
//

#include "translationRef.h"
#include <string.h>
//完成十进制转化成二进制
int translationRef(char *Code)
{
    int a=0,i=0;
    long len=strlen(Code);
    for(i=0;i<len;i++)
    {
        a=a*2+(Code[i]-'0');
    }
    return a;
}
