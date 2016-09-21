#include "cpu/exec/template-start.h"

#define instr call

static void do_execute()
{
    printf("%d",op_src->val);
    cpu.eip+=op_src->val;
    print_asm_template1();
}

make_instr_helper(ni)

#include "cpu/exec/template-end.h"
