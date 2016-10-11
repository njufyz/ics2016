#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_,SUFFIX)){
    MEM_W(cpu.esi,MEM_R(cpu.esi));
    if(cpu.eflags.df == 0)
    {
        cpu.esi += DATA_BYTE;
        cpu.edi += DATA_BYTE;
    }
    else 
    {
        cpu.esi -= DATA_BYTE;
        cpu.edi -= DATA_BYTE;
    }
    print_asm("movs");
    return 1;
}

#include "cpu/exec/template-end.h"

