#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_,SUFFIX)){
    MEM_W(cpu.gpr[7]._32,swaddr_read(cpu.gpr[2]._32,DATA_BYTE));
    if(cpu.eflags.df == 0)
    {
        cpu.gpr[7]._32 += DATA_BYTE;
        cpu.gpr[2]._32 += DATA_BYTE;
    }
    else 
    {
        cpu.gpr[7]._32 -= DATA_BYTE;
        cpu.gpr[2]._32 -= DATA_BYTE;
    }
    print_asm("movs");
    return 1;
}

#include "cpu/exec/template-end.h"

