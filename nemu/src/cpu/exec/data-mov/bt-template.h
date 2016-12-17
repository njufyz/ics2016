#include "cpu/exec/template-start.h"

#define instr bt

static void do_execute(){
    cpu.eflags.cf = (op_dest->val >> op_src->val) & 1;
    print_asm_template1();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"