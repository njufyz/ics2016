#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_,SUFFIX)){
    DATA_TYPE result = (DATA_TYPE)cpu.eax - MEM_R(cpu.edi);
    long long result_l = (long long )(DATA_TYPE)cpu.eax - (long long)MEM_R(cpu.edi);
    if(MSB((DATA_TYPE)cpu.eax) != MEM_R(cpu.edi) && MSB(result) != MSB((DATA_TYPE)cpu.eax))  cpu.eflags.of = 1;
    else cpu.eflags.of = 0;
    cpu.eflags.cf = (result_l >> (8* DATA_BYTE)) & 1;
    update_eflags(result);
   
    if(cpu.eflags.df == 0)
        cpu.edi +=DATA_BYTE;
    else cpu.edi -=DATA_BYTE;
    print_asm("scas%s",str(SUFFIX));
    return 1;
}

#include "cpu/exec/template-end.h"
