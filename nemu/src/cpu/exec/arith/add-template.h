#include "cpu/exec/template-start.h"

#define instr add

static void do_execute(){

    DATA_TYPE result = op_dest->val + op_src->val;
    update_eflags(result);
    cpu.eflags.af = ((op_dest->val & 0x7) + (op_src->val & 0x7)) > 0x7 ? 0 : 1;
    if( MSB(op_dest->val) == MSB(op_src->val) && MSB(result) != MSB(op_dest->val)) 
        cpu.eflags.of = 1; 
    else cpu.eflags.of = 0;

    long long result_l = (long long)op_dest->val + (long long)op_src->val;
    if(result_l >> (8 * DATA_BYTE)) cpu.eflags.cf = 1;
    else cpu.eflags.cf = 0;

    OPERAND_W(op_dest, result); 
	print_asm_template2();
}

make_instr_helper(r2rm)
#if DATA_BYTE ==2 ||DATA_BYTE==4
make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"
