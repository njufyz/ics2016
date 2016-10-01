#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
    DATA_TYPE result = op_dest->val & op_src->val;
    cpu.eflags.cf=0;
    cpu.eflags.of=0;
    update_eflags(result);
    print_asm_template2();
}

make_instr_helper(r2rm);

#include "cpu/exec/template-end.h"


