//
//  Header.h
//  CPU
//
//

#ifndef Header_h
#define Header_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

extern int Memory[32767],lag1,lag2,sum;//Memory用来保存内存数据  lag判断是否继续执行指令 sum代表指令行数

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
extern inst_t sample_code[100];//存指令
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

#endif /* Header_h */
