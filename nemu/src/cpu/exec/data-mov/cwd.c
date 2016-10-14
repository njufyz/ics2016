#include "cpu/exec/helper.h"

make_helper(cwd){
    if(cpu.eax >> 31 == 1) 
        cpu.edx = 0xffffffff;
    else cpu.edx = 0;
    print_asm("cwd/cdq");
    return 1;
}
