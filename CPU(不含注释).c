//多核不含注释
//  CPU.c
//  试验
//
//

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
char Code1[100][9],Code2[100][5],Code3[100][5],Code4[100][17],CODESET[100][17],codeSegment[100][33],CODE[100][32];
int Memory[32767] = {0},lag1 = 0,lag2 = 0,sum = 0;
#define N 100
#define IADD    1
#define LADD    111
#define ISUB    2
#define LSUB    222
#define IMUL    3
#define LMUL    333
#define IDIV    4
#define LDIV    444
#define ICMP    5
#define LCMP    555
#define IJMP    6
#define LJMP    66
#define NJMP    666
#define MJMP    6666
#define IMOV    7
#define IGIV    8
#define ISED    9
#define IDOR    10
#define IPUT    11
#define IOUT    12
#define ILOG    13
#define LLOG    1313
#define ILOV    14
#define LLOV    1414
#define ILON    15
#define LLON    1515
#define ILOC    16
#define IUNL    17
#define ISTOP   255

#define FNA     0
#define FEQ     1
#define FNE     -1


typedef struct inst
{
    int code;
    short p1, p2 ,p3;
} inst_t;
typedef signed short data_t;

typedef struct vm_state
{
    int ip;
    int flag;
    inst_t *code;
    short ax[9];
} vm_state_t;
inst_t sample_code[100];

int Instruct(char *Code1,char *Code2,char *Code3);
int returnNumber1(char *Code1,char *Code2,char *Code3,char *Code4);
int returnNumber2(char *Code1,char *Code2,char *Code3,char *Code4);
int serialNumber(char *Code1,char *Code2,char *Code3);
int translationRef(char *Code);
int fetchInstruction(int tid);
void analyzeInstruction(int sum);
void executeInstruction(vm_state_t *state,int tid);
void printSegment(int sum);


int Instruct(char *Code1,char *Code2,char *Code3)
{
    int code1,code2,code3;
    code1  = translationRef(Code1);
    code2  = translationRef(Code2);
    code3  = translationRef(Code3);
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

int returnNumber1(char *Code1,char *Code2,char *Code3,char *Code4)
{
    int code1,code2,code3,code4;
    code1  = translationRef(Code1);
    code2  = translationRef(Code2);
    code3  = translationRef(Code3);
    code4  = translationRef(Code4);
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

int returnNumber2(char *Code1,char *Code2,char *Code3,char *Code4)
{
    int code1,code2,code3,code4;
    code1  = translationRef(Code1);
    code2  = translationRef(Code2);
    code3  = translationRef(Code3);
    code4  = translationRef(Code4);
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


int serialNumber(char *Code1,char *Code2,char *Code3)
{
    int code1,code2,code3;
    code1  = translationRef(Code1);
    code2  = translationRef(Code2);
    code3  = translationRef(Code3);
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


int fetchInstruction(int tid) {
    FILE *fp;
    int  i=0,j=0,sum=0;
    char CODE[100][32];

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

    while( fgets(CODE[i], N, fp) != NULL ) {
        if(CODE[i][0]!='0')
            break;
        i++;
    }
    sum=i;
    for(i=0;i<sum;i++)
    {
        for(j=0;j<8;j++)
            Code1[i][j]=CODE[i][j];
        for(j=0;j<4;j++)
            Code2[i][j]=CODE[i][j+8];
        for(j=0;j<4;j++)
            Code3[i][j]=CODE[i][j+12];
        for(j=0;j<16;j++)
            Code4[i][j]=CODE[i][j+16];
        for(j=0;j<16;j++)
            CODESET[i][j]=CODE[i][j];
        for(j=0;j<32;j++)
        codeSegment[i][j]=CODE[i][j];
        Code1[i][8]='\0';
        Code2[i][4]='\0';
        Code3[i][4]='\0';
        CODESET[i][16]='\0';
        codeSegment[i][32]='\0';
    }

    fclose(fp);
    return sum;
}

void analyzeInstruction(int sum)
{
    int i;
    for(i=0;i<sum;i++)
    {
        sample_code[i].code = Instruct(Code1[i], Code2[i], Code3[i]);
        sample_code[i].p1 = returnNumber1(Code1[i], Code2[i], Code3[i], Code4[i]);
        sample_code[i].p2 = returnNumber2(Code1[i], Code2[i], Code3[i], Code4[i]);
        sample_code[i].p3 = serialNumber(Code1[i], Code2[i], Code3[i]);
    }
}

void executeInstruction(vm_state_t *state,int tid)
{
        int ir=0,i=0;
        inst_t current = state->code[state->ip];
        state->ip++;
        switch (current.code)
        {
            case IADD:
                state->ax[current.p3] = state->ax[current.p1] + current.p2;
                break;
            case LADD:
                state->ax[current.p3] = state->ax[current.p1] + Memory[state->ax[current.p2]];
                break;
            case ISUB:
                state->ax[current.p3] = state->ax[current.p1] - current.p2;
                break;
            case LSUB:
                state->ax[current.p3] = state->ax[current.p1] - Memory[state->ax[current.p2]];
                break;
            case IMUL:
                state->ax[current.p3] = state->ax[current.p1] * current.p2;
                break;
            case LMUL:
                state->ax[current.p3] = state->ax[current.p1] * Memory[state->ax[current.p2]];
                break;
            case IDIV:
                state->ax[current.p3] = state->ax[current.p1] / current.p2;
                break;
            case LDIV:
                state->ax[current.p3] = state->ax[current.p1] / Memory[state->ax[current.p2]];
                break;
            case ICMP:
                if (state->ax[current.p1] == current.p2)
                    state->flag = FNA;
                if (state->ax[current.p1] > current.p2)
                    state->flag = FEQ;
                if (state->ax[current.p1] < current.p2)
                    state->flag = FNE;
                break;
            case LCMP:
                if (state->ax[current.p1] == Memory[state->ax[current.p2]])
                    state->flag = FNA;
                if (state->ax[current.p1] > Memory[state->ax[current.p2]])
                    state->flag = FEQ;
                if (state->ax[current.p1] < Memory[state->ax[current.p2]])
                    state->flag = FNE;
                break;
            case IJMP:
                state->ip =state->ip+ current.p2/4 - 1;
                ir = translationRef(CODESET[state->ip-current.p2/4]);
                i=1;
                break;
            case LJMP:
                if(state->flag == 0){
                   state->ip =state->ip+ current.p2/4 - 1;
                    ir = translationRef(CODESET[state->ip-current.p2/4]);
                    i=1;
                }
                break;
            case NJMP:
                if(state->flag == 1){
                   state->ip =state->ip+ current.p2/4 - 1;
                    ir = translationRef(CODESET[state->ip-current.p2/4]);
                    i=1;
                }
                break;
            case MJMP:
                if(state->flag == -1){
                    state->ip =state->ip+ current.p2/4 - 1;
                    ir = translationRef(CODESET[state->ip-current.p2/4]);
                    i=1;
                }
                break;
            case IMOV:
                Memory[state->ax[current.p2]] = state->ax[current.p1];
                //state->data[current.p1] = state->data[current.p2];
                break;
            case IGIV:
                state->ax[current.p2] = Memory[state->ax[current.p1]];
                break;
            case ISED:
                state->ax[current.p3] = current.p1;
                break;
            case ILOG:
                if( state->ax[current.p1]!=0&&current.p2!=0)
                    state->ax[current.p3] = 1;
                else
                    state->ax[current.p3] = 0;
                break;
            case LLOG:
                if( state->ax[current.p1]!=0&&Memory[state->ax[current.p2]]!=0)
                    state->ax[current.p3] = 1;
                else
                    state->ax[current.p3] = 0;
                break;
            case ILOV:
                if( state->ax[current.p1]==0&&current.p2==0)
                    state->ax[current.p3] = 0;
                else
                    state->ax[current.p3] = 1;
                break;
            case LLOV:
                if( state->ax[current.p1]==0&&Memory[state->ax[current.p2]]==0)
                    state->ax[current.p3] = 0;
                else
                    state->ax[current.p3] = 1;
                break;
            case ILON:
                if( state->ax[current.p1]!=0)
                    state->ax[current.p1] = 0;
                else
                    state->ax[current.p1] = 1;
                break;
            case LLON:
                if(Memory[state->ax[current.p2]]!=0)
                    Memory[state->ax[current.p2]] = 0;
                else
                    Memory[state->ax[current.p2]] = 1;
                break;
            case IPUT:
                printf("in:\n");
                scanf("%hd",&state->ax[current.p3]);
                break;
            case IOUT:
                pthread_mutex_lock(&mutex1);
                printf("ID = %d\nout: %hd---------------------------\n",tid,state->ax[current.p3]);
                pthread_mutex_unlock(&mutex1);
                break;
            case IDOR:
                usleep(current.p1*1000);
                break;
            case ILOC:
                pthread_mutex_lock(&mutex);
                break;
            case IUNL:
                pthread_mutex_unlock(&mutex);
                break;
            case ISTOP:
            {
                if(tid == 1)
                    lag1 = 1;
                else if(tid == 2)
                    lag2 = 1;
                break;
            }
        }
        pthread_mutex_lock(&mutex1);
        if(i==0)
        ir = translationRef(CODESET[state->ip-1]);
        printf("~~~~~ID = %d~~~~~\n",tid);
        if(tid == 1)
        printf("ip = %d\n",4*state->ip);
        else
        printf("ip = %d\n",4*state->ip+256);
        printf("flag = %d\n",state->flag);
        printf("ir = %d\n",ir);
        printf("ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n",state->ax[1],state->ax[2],state->ax[3],state->ax[4]);
        printf("ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n",state->ax[5],state->ax[6],state->ax[7],state->ax[8]);
        i=0;
        printf("\n");
        pthread_mutex_unlock(&mutex1);
}
void *doit(void *threadID)
{
    vm_state_t state;
    memset(&state, 0, sizeof(state));
    int tid = *((int *)threadID);
    sum = fetchInstruction(tid);
    analyzeInstruction(sum);
    state.ip=0;
    state.flag=0;
    state.code = sample_code;
    state.ax[0]=state.ax[1]=state.ax[2]=state.ax[3]=state.ax[4]=state.ax[5]=state.ax[6]=state.ax[7]=state.ax[8]=0;
    while(lag1  == 0&&tid == 1)
       {
               executeInstruction(&state,tid);
       }
    while(lag2  == 0&&tid == 2)
       {
               executeInstruction(&state,tid);
       }
    return NULL;
}
int main(int argn, char *argv[])
{
    int ID1=1,ID2=2;
    pthread_t tid1, tid2;
    Memory[16384] = 100;
    pthread_create(&tid1,NULL,doit,(void *)&ID1);
    pthread_create(&tid2,NULL,doit,(void *)&ID2);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex1);
    printSegment(sum);
    return 0;
}

void printSegment(int sum)
{
    printf("\n");
    printf("codeSegment :\n");
    int i,j,n=0;
    for(i=0;i<8;i++)
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
    for(i=9;i<16;i++)
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
    printf("dataSegment :\n");
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

