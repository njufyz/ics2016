#include "cpu/exec/template-start.h"

#define instr call

extern int Len;
static void do_execute()
{
    cpu.esp-=4;
    MEM_W(cpu.gpr[4]._32,cpu.eip+Len);
    cpu.eip+=op_src->val;
    print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
