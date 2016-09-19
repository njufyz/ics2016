#include"cpu/exec/helper.h"
make_helper(call)
{
    uint32_t op = instr_fetch(eip+1,4);
    eip+=5+op;
    return 5;
}

        

