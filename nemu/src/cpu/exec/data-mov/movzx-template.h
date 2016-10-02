#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute(){
    DATA_TYPE result = op_src->val;
    result&=((1<<op_src->size * 8)-1);
    swaddr_write(op_dest->val,op_dest->size,result);
    print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
