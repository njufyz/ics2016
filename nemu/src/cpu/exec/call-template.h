#include "cpu/exec/template-start.h"

#define instr call

static void do_execute()
{
    cpu.gpr[4]._32-=4;
    cpu.gpr[4]._32=cpu.eip;
    cpu.eip+=op_src->val;
    //print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
