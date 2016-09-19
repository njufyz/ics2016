#include"cpu/exec/helper.h"
make_helper(call)
{
    uint32_t op = instr_fetch(eip+1,4);
    eip+=op;
    printf("\n%x\n",eip);
    return 5;
}

        

