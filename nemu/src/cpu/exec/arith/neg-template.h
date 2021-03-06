#include "cpu/exec/template-start.h"

#define instr neg

static void do_execute() {
	DATA_TYPE result = -op_src->val;
	OPERAND_W(op_src, result);

	/* There is no need to update EFLAGS, since no other instructions 
	 * in PA will test the flags updated by this instruction.
	 */
    if(op_src->val == 0) cpu.eflags.cf = 0;
    else cpu.eflags.cf = 1;
    if((DATA_BYTE == 1 && op_src->val == 0xff) || (DATA_BYTE == 2 && op_src->val == 0xffff) || (DATA_BYTE == 4 && op_src->val == 0xffffffff))
        cpu.eflags.of = 1;
    else cpu.eflags.of = 0;

    update_eflags(result);
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
