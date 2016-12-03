#include "cpu/exec/helper.h"
void load_segcache(uint8_t);

make_helper(ret)
{
   cpu.eip = swaddr_read(cpu.esp, 4, R_SS);
   cpu.eip -= 1;
   cpu.esp+=4;
   print_asm("ret  addr:0x%x", cpu.eip+1);
   return 1;
}

make_helper(iret){
    cpu.eip = swaddr_read(cpu.esp, 4, R_SS);
    cpu.esp += 4;
    cpu.eip -= 1;

    cpu.segreg[R_CS].val = swaddr_read(cpu.esp, 4, R_SS);
   // load_segcache(R_CS);
    cpu.esp += 4;

    cpu.eflags.val = swaddr_read(cpu.esp, 4, R_SS);
    cpu.esp += 4;

   print_asm("iret  addr:0x%x", cpu.eip);
   return 1;
}
