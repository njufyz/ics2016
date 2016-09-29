#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute()
{
    DATA_TYPE result = op_src->val - op_dest->val; //???
    cpu.eflags.sf=MSB(result);
    if(!result) {
        cpu.eflags.zf=1;
        cpu.eflags.pf=1;
        int k = (op_src->val>0 && op_dest->val<0 && result<=0);
        int t = (op_src->val<=0 && op_dest->val >= 0&&result>0);
        cpu.eflags.of=!k&&!t;
        /*TODO*/
    }
    print_asm_template2();
}

make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"



