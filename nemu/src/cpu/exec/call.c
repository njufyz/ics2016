#include"cpu/exec/helper.h"
make_helper(call)
{
    uint32_t op = instr_fetch(eip+1,4);
    cpu.eip+=op;
    printf("%x\n",eip);
    return 5;
}

        

