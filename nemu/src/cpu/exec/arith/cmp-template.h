#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute()
{
    DATA_TYPE result = op_dest->val - op_src->val; //???
    long long result_l = ((long long)op_dest->val - (long long)op_src->val)>>(8*DATA_BYTE);
    cpu.eflags.cf = result_l &1;  //CF


    if(MSB(op_dest->val)!=MSB(op_src->val) && MSB(op_dest->val) != MSB(result))
        cpu.eflags.of=1;
    else cpu.eflags.of=0;

    update_eflags(result);
    print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE==4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(i2rm)
make_instr_helper(i2a)
make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"



