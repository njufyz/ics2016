#include "cpu/exec/template-start.h"

#define instr push

static void do_execute()
{
    if(DATA_BYTE==2)
    cpu.gpr[4]._32-=DATA_BYTE;
    else cpu.gpr[4]._32-=4;
    MEM_W(cpu.gpr[4]._32,op_src->val);
    print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)
#include "cpu/exec/template-end.h"
