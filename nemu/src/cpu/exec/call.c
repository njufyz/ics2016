#include"cpu/exec/helper.h"
make_helper(call)
{
    uint32_t op = instr_fetch(eip+1,4);
    assert(op==6);
    eip+=op;
    return 5;
}

        

