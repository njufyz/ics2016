#include"cpu/exec/helper.h"
make_helper(call)
{
    assert(eip == 0x10000a);
    uint32_t op = instr_fetch(eip+1,4);
    eip+=op;
    return 5;
}

        

