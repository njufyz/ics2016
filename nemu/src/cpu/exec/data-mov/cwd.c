#include "cpu/exec/helper.h"

make_helper(cwd){
    if(cpu.gpr[0]._32 <0) 
        cpu.gpr[2]._32 = 0xffffffff;
    else cpu.gpr[2]._32 = 0;
    print_asm("cwd/cdq");
    return 1;
}
