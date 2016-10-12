#include "cpu/exec/template-start.h"

#define instr lods

make_helper(concat(lods_,SUFFIX)){
    cpu.eax = MEM_R(cpu.esi);
    if(cpu.eflags.df == 0)
        cpu.esi += DATA_BYTE;
    else cpu.esi -= DATA_BYTE;
    print_asm("lods%s",str(SUFFIX));
    return 1;
}

#include "cpu/exec/template-end.h"

