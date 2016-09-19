#include"cpu/exec/helper.h"
make_helper(call)
{
    uint32_t op = instr_fetch(eip+1,4);
    printf("%x\n",eip);
    eip+=op;
    return 5;
}

        

