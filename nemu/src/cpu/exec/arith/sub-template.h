#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute(){
    DATA_TYPE result = op_dest->val - op_src->val;
    update_eflags(result);

    long long result_l = (((long long)op_dest->val - (long long)op_src->val)>>(8*DATA_BYTE));
    cpu.eflags.cf = result_l & 1;  //CF

    if((op_dest->val & 0x7) > (op_src->val & 0x7)) cpu.eflags.af=0;
    else cpu.eflags.af=1;    //AF

    if(MSB(op_dest->val)!=MSB(op_src->val) && MSB(op_dest->val) != MSB(result))
        cpu.eflags.of=1;
    else cpu.eflags.of=0;  //of

    print_asm_template2();
}

make_instr_helper(si2rm)

#include "cpu/exec/template-end.h"
