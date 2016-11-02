#include "cpu/exec/template-start.h"

#define instr add

static void do_execute(){

    DATA_TYPE result = op_dest->val + op_src->val;
    OPERAND_W(op_dest, result); 
    if( MSB(op_dest->val) == MSB(op_src->val) && MSB(result) != MSB(op_dest->val)) 
        cpu.eflags.of = 1; 
    else cpu.eflags.of = 0;

    long long result_l = (long long)op_dest->val + (long long)op_src->val;
    if(result_l >> (8 * DATA_BYTE)) cpu.eflags.cf = 1;
    else cpu.eflags.cf = 0;

    update_eflags(result);
	print_asm_template2();
}

make_instr_helper(r2rm)
#if DATA_BYTE ==2 ||DATA_BYTE==4
make_instr_helper(si2rm)
#endif
make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"
