//
//  analyzeInstruction.c
//  CPU
//
//

#include "Header.h"
#include "analyzeInstruction.h"
#include "fetchInstruction.h"
#include "translationRef.h"

inst_t sample_code[100];

int Instruct(char *Code1,char *Code2,char *Code3);//根据指令返回对应的操作，比如加减乘除，跳转等
int returnNumber1(char *Code1,char *Code2,char *Code3,char *Code4);//根据指令返回执行指令中的第一个参数，对应结构体inst_t中的p1参数
int returnNumber2(char *Code1,char *Code2,char *Code3,char *Code4);//根据指令返回执行指令中的第二个参数，对应结构体inst_t中的p2参数
int serialNumber(char *Code1,char *Code2,char *Code3);//根据指令返回执行指令中的第三个参数，对应结构体inst_t中的p3参数，这个参数用来保存指令保存数据对应的寄存器标号

//分析指令
void analyzeInstruction(int sum)
{
    int i;
    for(i=0;i<sum;i++)
    {
        sample_code[i].code = Instruct(Code1[i], Code2[i], Code3[i]);//保存指令编号
        sample_code[i].p1 = returnNumber1(Code1[i], Code2[i], Code3[i], Code4[i]);//执行指令用到的参数p1
        sample_code[i].p2 = returnNumber2(Code1[i], Code2[i], Code3[i], Code4[i]);//执行指令用到的参数p2
        sample_code[i].p3 = serialNumber(Code1[i], Code2[i], Code3[i]);//执行指令用来保存指令保存数据对应的寄存器标号
    }
}
//根据指令返回对应的操作，比如加减乘除，跳转等
int Instruct(char *Code1,char *Code2,char *Code3)
{
    int code1,code2,code3;
    code1  = translationRef(Code1);//将前8位转换成二进制
    code2  = translationRef(Code2);//将第9-12转换成二进制
    code3  = translationRef(Code3);//将前13-16位转换成二进制
    //根据对应的指令返回对应的指令编号
    if(code1==0)
        return 255;
    else if(code1==1&&code3==0)
        return 9;
    else if(code1==1&&code2<5)
        return 8;
    else if(code1==1&&code2>4)
        return 7;
    else if(code1==2&&code3==0)
        return 1;
    else if(code1==2&&code3!=0)
        return 111;
    else if(code1==3&&code3==0)
        return 2;
    else if(code1==3&&code3!=0)
        return 222;
    else if(code1==4&&code3==0)
        return 3;
    else if(code1==4&&code3!=0)
        return 333;
    else if(code1==5&&code3==0)
        return 4;
    else if(code1==5&&code3!=0)
        return 444;
    else if(code1==6&&code3==0)
        return 13;
    else if(code1==6&&code3!=0)
        return 1313;
    else if(code1==7&&code3==0)
        return 14;
    else if(code1==7&&code3!=0)
        return 1414;
    else if(code1==8&&code3==0)
        return 15;
    else if(code1==8&&code3!=0)
        return 1515;
    else if(code1==9&&code3==0)
        return 5;
    else if(code1==9&&code3!=0)
        return 555;
    else if(code1==10&&code3==0)
        return 6;
    else if(code1==10&&code3==1)
        return 66;
    else if(code1==10&&code3==2)
        return 666;
    else if(code1==10&&code3==3)
        return 6666;
    else if(code1==11)
        return 11;
    else if(code1==12)
        return 12;
    else if(code1==13)
        return 16;
    else if(code1==14)
        return 17;
    else if(code1==15)
        return 10;
    return 0;
}

//根据指令返回执行指令中的第一个参数，对应结构体inst_t中的p1参数
int returnNumber1(char *Code1,char *Code2,char *Code3,char *Code4)
{
    int code1,code2,code3,code4;
   code1  = translationRef(Code1);//将前8位转换成二进制
    code2  = translationRef(Code2);//将第9-12转换成二进制
    code3  = translationRef(Code3);//将前13-16位转换成二进制
    code4  = translationRef(Code4);//将后16位(立即数)转换成二进制
    //根据对应的指令返回执行指令需要操作的第一个数
    if(code1==0)
        return 0;
    else if(code1==1&&code3==0)
        return code4;
    else if(code1==1&&code2<5)
        return code3;
    else if(code1==1&&code2>4)
        return code3;
    else if(code1==2||code1==3||code1==4||code1==5||code1==6||code1==7)
        return code2;
    else if(code1==8&&code3!=0)
        return 0;
    else if(code1==8&&code3==0)
        return code2;
    else if(code1==9)
        return code2;
    else if(code1==10)
        return 0;
    else if(code1==11)
        return 0;
    else if(code1==12)
        return 0;
    else if(code1==13)
        return code4;
    else if(code1==14)
        return code4;
    else if(code1==15)
        return code4;
    return 0;
}

//根据指令返回执行指令中的第二个参数，对应结构体inst_t中的p2参数
int returnNumber2(char *Code1,char *Code2,char *Code3,char *Code4)
{
    int code1,code2,code3,code4;
    code1  = translationRef(Code1);//将前8位转换成二进制
    code2  = translationRef(Code2);//将第9-12转换成二进制
    code3  = translationRef(Code3);//将前13-16位转换成二进制
    code4  = translationRef(Code4);//将后16位(立即数)转换成二进制
    //根据对应的指令返回执行指令需要操作的第二个数
    if(code1==0)
        return 0;
    else if(code1==1&&code3==0)
        return 0;
    else if(code1==1&&code2<5)
        return code2;
    else if(code1==1&&code2>4)
        return code2;
    else if((code1==2&&code3==0)||(code1==3&&code3==0)||(code1==4&&code3==0)||(code1==5&&code3==0)||(code1==6&&code3==0)||(code1==7&&code3==0)||(code1==9&&code3==0)||code1==10)
        return code4;
    else if((code1==2&&code3!=0)||(code1==3&&code3!=0)||(code1==4&&code3!=0)||(code1==5&&code3!=0)||(code1==6&&code3!=0)||(code1==7&&code3!=0))
        return code3;
    else if(code1==8&&code3!=0)
        return code3;
    else if(code1==8&&code3==0)
        return 0;
    else if(code1==9&&code3!=0)
        return code3;
    else if(code1==11)
        return 0;
    else if(code1==12)
        return 0;
    else if(code1==13)
        return 0;
    else if(code1==14)
        return 0;
    else if(code1==15)
        return 0;
    return 0;
}

//根据指令返回执行指令中的第三个参数，对应结构体inst_t中的p3参数，这个参数用来保存指令保存数据对应的寄存器标号
int serialNumber(char *Code1,char *Code2,char *Code3)
{
    int code1,code2,code3;
    code1  = translationRef(Code1);//将前8位转换成二进制
    code2  = translationRef(Code2);//将第9-12转换成二进制
    code3  = translationRef(Code3);//将前13-16位转换成二进制
    //根据对应的指令返回用来保存指令保存数据对应的寄存器标号
    if(code1==0)
        return 0;
    else if(code1==1||code1==2||code1==3||code1==4||code1==5||code1==6||code1==7)
        return code2;
    else if(code1==8&&code3!=0)
        return code2;
    else if(code1==8&&code3==0)
        return code3;
    else if(code1==9)
        return 0;
    else if(code1==10)
        return 0;
    else if(code1==11)
        return code2;
    else if(code1==12)
        return code2;
    else if(code1==13)
        return 0;
    else if(code1==14)
        return 0;
    else if(code1==15)
        return 0;
    return 0;
}

