#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
    if(MSB(op_src->val == 0) && MSB(result) != 0)
        cpu.eflags.of = 1;
    else cpu.eflags.of = 0;
    if( result == ~(-1ll<<(8* DATA_BYTE))) cpu.eflags.of = 1;
    else cpu.eflags.of = 0;
    update_eflags(result);
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
