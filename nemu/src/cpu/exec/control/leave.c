#include "cpu/exec/helper.h"

make_helper(leave)
{
    cpu.gpr[4]._32 = cpu.gpr[5]._32;
    cpu.gpr[5]._32 = swaddr_read(cpu.gpr[4]._32,4);
    cpu.gpr[4]._32+=4;
    print_asm("leave");
    return 1;
}

