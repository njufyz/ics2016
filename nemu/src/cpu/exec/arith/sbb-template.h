#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute(){
    DATA_TYPE result = op_dest->val-(op_src->val + cpu.eflags.cf);
    OPERAND_W(op_dest,result);
    long long result_l = (long long )op_dest->val - ((long long)op_src->val + (long long) cpu.eflags.cf);
    cpu.eflags.cf = (result_l >> ( 8 * DATA_BYTE-1)) & 1;
    if(MSB(op_dest->val)!= MSB(op_src->val) && MSB(result) != MSB (op_dest->val))
        cpu.eflags.of = 1;
    else cpu.eflags.of =0;
    update_eflags(result);
    print_asm_template2();
}

make_instr_helper(rm2r);

#include"cpu/exec/template-end.h"



