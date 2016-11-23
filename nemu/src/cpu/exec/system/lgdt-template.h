#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute(){
    cpu.gdtr.limit = lnaddr_read(op_src->addr, 2);
    cpu.gdtr.base = lnaddr_read(op_src->addr + 2, 4);
    print_asm_template1();
}

make_instr_helper(rm);

#include "cpu/exec/template-end.h"
