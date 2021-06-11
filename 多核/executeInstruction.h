//
//  executeInstruction.h
//  CPU
//
//

#ifndef executeInstruction_h
#define executeInstruction_h

#ifndef Header_h
#define Header_h
typedef struct inst
{
    int code; // 指令
    short p1, p2 ,p3;// 参数1、2,3
} inst_t;
typedef signed short data_t;
typedef struct vm_state
{
    int ip; // 指令ptr
    int flag; // 记录最后判断的标志
    inst_t *code;
    short ax[9];
} vm_state_t;
#endif
void executeInstruction(vm_state_t *state,int tid);

#endif/* executeInstruction_h */
