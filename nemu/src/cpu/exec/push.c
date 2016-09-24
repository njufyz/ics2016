#include"cpu/exec/helper.h"

make_helper(push)
{
    uint32_t op=instr_fetch(eip,1);
    int index = op-0x50;
    cpu.gpr[4]._32-=4;
    uint32_t k = swaddr_read(cpu.gpr[index]._32,4);
    swaddr_write(cpu.gpr[4]._32,4,k);
    return 1;
}
