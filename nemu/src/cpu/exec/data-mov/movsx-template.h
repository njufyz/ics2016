#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute(){
    if(!ops_decoded.is_operand_size_16) {
        op_src->val &= 0xff;
        int8_t temp = op_src->val;
    OPERAND_W(op_dest,temp);
    }
    else {
        op_src->val &= 0xffff;
        int16_t temp = op_src->val;
    OPERAND_W(op_dest,temp);
    };
    print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
            



    
