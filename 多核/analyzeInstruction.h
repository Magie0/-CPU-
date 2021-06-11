//
//  analyzeInstruction.h
//  CPU
//
//

#ifndef analyzeInstruction_h
#define analyzeInstruction_h

#ifndef Header_h
#define Header_h
typedef struct inst
{
    int code; // 指令
    short p1, p2 ,p3;// 参数1、2,3
} inst_t;
typedef signed short data_t;
extern inst_t sample_code[100];
#endif
void analyzeInstruction(int sum);

#endif /* analyzeInstruction_h */
