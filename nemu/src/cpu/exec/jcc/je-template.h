#include "cpu/exec/template-start.h"

#define instr je

static void do_execute()
{
    if(cpu.eflags.zf==1)
    {
        cpu.eip+=op_src->val;
    }
}

make_instr_helper(si)
