#include "cpu/exec/helper.h"

make_helper(ret)
{
   cpu.eip = cpu.gpr[4]._32;
   cpu.gpr[4]._32+=4;
   return 1;
}
