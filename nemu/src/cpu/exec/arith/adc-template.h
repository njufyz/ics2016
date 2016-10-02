#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute(){
    DATA_TYPE result = op_dest->val + op_src->val + cpu.eflags.cf ;
    OPERAND_W(op_dest,result);
    long long result_l = (long long)op_dest->val + (long long)op_src->val +(long long) cpu.eflags.cf; 
    if(result_l>>(8*DATA_BYTE)) cpu.eflags.cf = 1;
    else cpu.eflags.cf = 0;
    if(MSB(op_dest->val)==MSB(op_src->val) && MSB(op_dest->val)!= MSB(result)) 
        cpu.eflags.of=1;
    else cpu.eflags.of=0;
    cpu.eflags.af = ((op_dest->val &0xf) + (op_src->val & 0xf) + cpu.eflags.cf ) > 0xf ? 0 : 1;
    update_eflags(result);
    print_asm_template2();

}

make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"
