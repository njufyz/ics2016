#include "cpu/exec/helper.h"
make_helper(ret)
{
   cpu.eip = swaddr_read(cpu.gpr[4]._32,4)+4;
   cpu.gpr[4]._32+=4;
   print_asm_template1();
   return 1;
}
