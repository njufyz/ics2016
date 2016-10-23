#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute(){
    uint32_t result;
    if(!ops_decoded.is_operand_size_16)
    {
         result =op_src->val & 0xff;   //bw
    }
    else
        result = op_src->val & 0xffff;
   OPERAND_W(op_dest,result);
    print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
