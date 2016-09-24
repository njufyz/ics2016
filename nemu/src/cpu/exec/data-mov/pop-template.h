#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
    printf("%d",op_src->val);
    OPERAND_W(op_dest,op_src->val);
    cpu.gpr[4]._32+=4;
    print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
