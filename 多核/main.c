//
//  main.c
//  CPU
//
//


#include "Header.h"
#include "fetchInstruction.h"
#include "analyzeInstruction.h"
#include "executeInstruction.h"
#include "printSegment.h"

static void *doit(void *threadID);

int Memory[32767] = {0},lag1 = 0,lag2 = 0,sum = 0;
inst_t sample_code[100];

int main(int argn, char *argv[])
{
    int ID1=1,ID2=2; //线程ID
    pthread_t tid1, tid2;
    Memory[16384] = 100;//内存16384对应数据为100
    //创建两个线程
    pthread_create(&tid1,NULL,doit,(void *)&ID1);
    pthread_create(&tid2,NULL,doit,(void *)&ID2);
    //等待线程汇合
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    
    printSegment(sum);
    return 0;
}
static void *doit(void *threadID)//完成2个线程的运行
{
    vm_state_t state;
    memset(&state, 0, sizeof(state));
    int tid = *((int *)threadID);//线程ID
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
