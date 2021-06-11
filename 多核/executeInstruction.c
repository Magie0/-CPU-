//
//  executeInstruction.c
//  CPU
//
//

#include "Header.h"
#include "executeInstruction.h"
#include "fetchInstruction.h"
#include "translationRef.h"
#include <unistd.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//锁1
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;//锁2

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
