//  多核含注释
//  CPU.c
//  试验
//
//

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//锁1
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;//锁2
char Code1[100][9],Code2[100][5],Code3[100][5],Code4[100][17],CODESET[100][17],codeSegment[100][33],CODE[100][32];//Code1保存指令前8位，Code2保存9-12位，Code3保存13-16位，Code4保存后16位，CODESET保存前16位，codeSegment和CODE保存完整指令
int Memory[32767] = {0},lag1 = 0,lag2 = 0,sum = 0;//Memory用来保存内存数据  lag判断是否继续执行指令 sum代表指令行数
#define N 100
#define IADD    1 // 加法
#define LADD    111
#define ISUB    2 // 减法
#define LSUB    222
#define IMUL    3 // 乘法
#define LMUL    333
#define IDIV    4 // 除法
#define LDIV    444
#define ICMP    5 // 判断
#define LCMP    555
#define IJMP    6 // 4种跳转
#define LJMP    66
#define NJMP    666
#define MJMP    6666
#define IMOV    7 // 赋值
#define IGIV    8 // 赋值
#define ISED    9 // 赋值
#define IDOR    10 //休眠
#define IPUT    11 // 输入
#define IOUT    12 // 输出
#define ILOG    13//逻辑指令与
#define LLOG    1313
#define ILOV    14//逻辑指令或
#define LLOV    1414
#define ILON    15 //逻辑指令非
#define LLON    1515
#define ILOC    16 //加锁
#define IUNL    17 //解锁
#define ISTOP   255 //停机

#define FNA     0 //比较后相等为0
#define FEQ     1 //寄存器1中的大 置为1
#define FNE     -1 //立即数大   置为-1


typedef struct inst
{
    int code; // 指令
    short p1, p2 ,p3;// 参数1，2,3  p1，p2为执行指令需用到的数 p3用来保存指令保存数据对应的寄存器标号
} inst_t;
typedef signed short data_t;

typedef struct vm_state
{
    int ip; // 指令ptr
    int flag; // 记录最后判断的标志
    inst_t *code;
    short ax[9];//寄存器  下标代表序号
} vm_state_t;

inst_t sample_code[100];//存指令

int Instruct(char *Code1,char *Code2,char *Code3);//根据指令返回对应的操作，比如加减乘除，跳转等
int returnNumber1(char *Code1,char *Code2,char *Code3,char *Code4);//根据指令返回执行指令中的第一个参数，对应结构体inst_t中的p1参数
int returnNumber2(char *Code1,char *Code2,char *Code3,char *Code4);//根据指令返回执行指令中的第二个参数，对应结构体inst_t中的p2参数
int serialNumber(char *Code1,char *Code2,char *Code3);//根据指令返回执行指令中的第三个参数，对应结构体inst_t中的p3参数，这个参数用来保存指令保存数据对应的寄存器标号
int translationRef(char *Code);//完成十进制转化成二进制
int fetchInstruction(int tid);//获取指令
void analyzeInstruction(int sum);//分析指令
void executeInstruction(vm_state_t *state,int tid);//执行指令
void printSegment(int sum);//打印地址


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


//获取指令
int fetchInstruction(int tid) {
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

//执行指令
void executeInstruction(vm_state_t *state,int tid)
{
        int ir=0,i=0;
        inst_t current = state->code[state->ip];
        state->ip++; // 取出指令以后自动ip后移
        switch (current.code)
        {
            case IADD:
                // 将寄存器内的数与一个立即数相加
                state->ax[current.p3] = state->ax[current.p1] + current.p2;
                break;
            case LADD:
                //将寄存器内的数与寄存器中地址所指向的内存单元里存的数相加
                state->ax[current.p3] = state->ax[current.p1] + Memory[state->ax[current.p2]];
                break;
            case ISUB:
                // 将寄存器内的数与一个立即数相减
                state->ax[current.p3] = state->ax[current.p1] - current.p2;
                break;
            case LSUB:
                //将寄存器内的数与寄存器中地址所指向的内存单元里存的数相减
                state->ax[current.p3] = state->ax[current.p1] - Memory[state->ax[current.p2]];
                break;
            case IMUL:
                // 将寄存器内的数与一个立即数相乘
                state->ax[current.p3] = state->ax[current.p1] * current.p2;
                break;
            case LMUL:
                //将寄存器内的数与寄存器中地址所指向的内存单元里存的数相乘
                state->ax[current.p3] = state->ax[current.p1] * Memory[state->ax[current.p2]];
                break;
            case IDIV:
                // 将寄存器内的数与一个立即数相除
                state->ax[current.p3] = state->ax[current.p1] / current.p2;
                break;
            case LDIV:
                //将寄存器内的数与寄存器中地址所指向的内存单元里存的数相除
                state->ax[current.p3] = state->ax[current.p1] / Memory[state->ax[current.p2]];
                break;
            case ICMP:
                // 将寄存器内的数与一个立即数比较，如两数相等，则标志寄存器被修置为0，如寄存器1大，则标志寄存器被置为1，如寄存器1小，则标志寄存器被置为-1
                if (state->ax[current.p1] == current.p2)
                    state->flag = FNA;
                if (state->ax[current.p1] > current.p2)
                    state->flag = FEQ;
                if (state->ax[current.p1] < current.p2)
                    state->flag = FNE;
                break;
            case LCMP:
                // 将寄存器内的数与寄存器中地址所指向的内存单元里存的数比较，如两数相等，则标志寄存器被置为0，如寄存器1大，则标志寄存器被置为1，如寄存器1小，则标志寄存器被置为-1
                if (state->ax[current.p1] == Memory[state->ax[current.p2]])
                    state->flag = FNA;
                if (state->ax[current.p1] > Memory[state->ax[current.p2]])
                    state->flag = FEQ;
                if (state->ax[current.p1] < Memory[state->ax[current.p2]])
                    state->flag = FNE;
                break;
            case IJMP:
                // 跳转，指令根据p2(立即数)进行偏移
                state->ip =state->ip+ current.p2/4 - 1;
                ir = translationRef(CODESET[state->ip-current.p2/4]);
                i=1;
                break;
            case LJMP:
                // 如果标志寄存器内的值为0则转移至程序计数器加一个立即数处执行。
                if(state->flag == 0){
                   state->ip =state->ip+ current.p2/4 - 1;
                    ir = translationRef(CODESET[state->ip-current.p2/4]);
                    i=1;
                }
                break;
            case NJMP:
                // 如果标志寄存器内的值为1则转移至程序计数器加一个立即数处执行
                if(state->flag == 1){
                   state->ip =state->ip+ current.p2/4 - 1;
                    ir = translationRef(CODESET[state->ip-current.p2/4]);
                    i=1;
                }
                break;
            case MJMP:
                // 如果标志寄存器内的值为-1则转移至程序计数器加一个立即数处执行
                if(state->flag == -1){
                    state->ip =state->ip+ current.p2/4 - 1;
                    ir = translationRef(CODESET[state->ip-current.p2/4]);
                    i=1;
                }
                break;
            case IMOV:
                // 把寄存器1到4中的数传送到寄存器5到8所指向的内存单元
                Memory[state->ax[current.p2]] = state->ax[current.p1];
                break;
            case IGIV:
                //把寄存器5到8所指向的内存单元中的数传送到寄存器1到4中
                state->ax[current.p2] = Memory[state->ax[current.p1]];
                break;
            case ISED:
                //把立即数传送给寄存器1到8；
                state->ax[current.p3] = current.p1;
                break;
            case ILOG:
                //将寄存器内的数与一个立即数做逻辑与
                if( state->ax[current.p1]!=0&&current.p2!=0)
                    state->ax[current.p3] = 1;
                else
                    state->ax[current.p3] = 0;
                break;
            case LLOG:
                // 将寄存器内的数与寄存器中地址所指向的内存单元里存的数做逻辑与
                if( state->ax[current.p1]!=0&&Memory[state->ax[current.p2]]!=0)
                    state->ax[current.p3] = 1;
                else
                    state->ax[current.p3] = 0;
                break;
            case ILOV:
                //将寄存器内的数与一个立即数做逻辑或
                if( state->ax[current.p1]==0&&current.p2==0)
                    state->ax[current.p3] = 0;
                else
                    state->ax[current.p3] = 1;
                break;
            case LLOV:
                // 将寄存器内的数与寄存器中地址所指向的内存单元里存的数做逻辑或
                if( state->ax[current.p1]==0&&Memory[state->ax[current.p2]]==0)
                    state->ax[current.p3] = 0;
                else
                    state->ax[current.p3] = 1;
                break;
            case ILON:
                // 将寄存器内的数与一个立即数做逻辑非
                if( state->ax[current.p1]!=0)
                    state->ax[current.p1] = 0;
                else
                    state->ax[current.p1] = 1;
                break;
            case LLON:
                // 将寄存器内的数与寄存器中地址所指向的内存单元里存的数做逻辑非
                if(Memory[state->ax[current.p2]]!=0)
                    Memory[state->ax[current.p2]] = 0;
                else
                    Memory[state->ax[current.p2]] = 1;
                break;
            case IPUT:
                // 输入 从输入端口读入一个整数并保存在寄存器中
                printf("in:\n");
                scanf("%hd",&state->ax[current.p3]);
                break;
            case IOUT:
                // 输出寄存器ax[n]中的数值
                pthread_mutex_lock(&mutex1);
                printf("ID = %d\nout: %hd---------------------------\n",tid,state->ax[current.p3]);
                pthread_mutex_unlock(&mutex1);
                break;
            case IDOR:
                // 休眠
                usleep(current.p1*1000);
                break;
            case ILOC:
                // 上锁
                pthread_mutex_lock(&mutex);
                break;
            case IUNL:
                // 解锁
                pthread_mutex_unlock(&mutex);
                break;
            case ISTOP:
                // 停机
                if(tid == 1)
                    lag1 = 1;
                else if(tid == 2)
                    lag2 = 1;
                break;
        }
        pthread_mutex_lock(&mutex1);//输出寄存器状态上锁
        if(i==0) // i=0 代表没有执行跳转指令 需要给ir赋值  i=1 代表执行了跳转指令 跳转指令中ir已经赋值 防止覆盖ir的值
        ir = translationRef(CODESET[state->ip-1]);
        printf("~~~~~ID = %d~~~~~\n",tid);//打印线程ID
        if(tid == 1) //线程1的ip
        printf("ip = %d\n",4*state->ip);
        else //线程2的ip
        printf("ip = %d\n",4*state->ip+256);
        printf("flag = %d\n",state->flag);
        printf("ir = %d\n",ir);
        printf("ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n",state->ax[1],state->ax[2],state->ax[3],state->ax[4]);//寄存器1-4
        printf("ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n",state->ax[5],state->ax[6],state->ax[7],state->ax[8]);//寄存器5-8
        i=0;//刷新i的值 使执行下一个指令i=0
        printf("\n");
        pthread_mutex_unlock(&mutex1);//解锁
}
void *doit(void *threadID) //完成2个线程的运行
{
    vm_state_t state;
    memset(&state, 0, sizeof(state));
    int tid = *((int *)threadID); //线程ID
    sum = fetchInstruction(tid);//取指令，返回指令行数
    analyzeInstruction(sum);//分析指令
    //将各种数值初始化
    state.ip=0;
    state.flag=0;
    state.code = sample_code;
    state.ax[0]=state.ax[1]=state.ax[2]=state.ax[3]=state.ax[4]=state.ax[5]=state.ax[6]=state.ax[7]=state.ax[8]=0;
    while(lag1  == 0&&tid == 1)
    {
            executeInstruction(&state,tid);//执行指令
    }
    while(lag2  == 0&&tid == 2)
    {
            executeInstruction(&state,tid);//执行指令
    }
    return NULL;
}
int main(int argn, char *argv[])
{
    int ID1=1,ID2=2; //线程ID
    pthread_t tid1, tid2;
    Memory[16384] = 100; //内存16384对应数据为100
    //创建两个线程
    pthread_create(&tid1,NULL,doit,(void *)&ID1);
    pthread_create(&tid2,NULL,doit,(void *)&ID2);
    //等待线程汇合
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    //销毁mutex锁
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex1);
    printSegment(sum);//打印地址
    return 0;
}

//打印地址
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

