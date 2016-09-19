#include"cpu/exec/helper.h"

make_helper(push)
{
    uint32_t op=instr_fetch(eip,1);
    int index = op-0x50;
    reg_l(4)-=4;
    swaddr_write(reg_l(4),4,reg_l(index));
    return 1;
}
