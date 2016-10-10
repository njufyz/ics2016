#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute(){
    DATA_TYPE_S temp = op_src->val;
    uint32_t result = temp;
    write_operand_l(op_dest,result);
    print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
            



    