#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute(){
    uint32_t result = op_src->val;
    result&=((1<<op_src->size * 8)-1);
    write_operand_l(op_dest,result);
    print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
