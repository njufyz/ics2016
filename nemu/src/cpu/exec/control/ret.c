#include "cpu/exec/helper.h"
make_helper(ret)
{
   cpu.eip = swaddr_read(cpu.gpr[4]._32,4);
   cpu.eip -= 1;
   cpu.gpr[4]._32+=4;
   print_asm("ret");
   return 1;
}
