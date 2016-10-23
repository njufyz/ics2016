#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute(){
    if(!ops_decoded.is_operand_size_16) op_src->val &= 0xff;
    else op_src->val &= 0xffff;
    uint32_t result = (DATA_TYPE_S) op_src->val;
    OPERAND_W(op_dest,result);
    print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
            



    
