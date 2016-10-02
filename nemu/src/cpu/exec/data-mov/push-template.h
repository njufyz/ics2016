#include "cpu/exec/template-start.h"

#define instr push

static void do_execute()
{
    cpu.gpr[4]._32-=DATA_BYTE;
    MEM_W(cpu.gpr[4]._32,op_src->val);
}

make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)
#include "cpu/exec/template-end.h"
