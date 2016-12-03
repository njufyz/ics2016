#include "cpu/exec/helper.h"

make_helper(hlt){
    while(true){
    Assert(cpu.eflags.If, "Interrupt is disable now.");
    if(cpu.intr) break;
    }
    print_asm("hlt");
    return 1;
}
