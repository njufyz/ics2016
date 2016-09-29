#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
    DATA_TYPE result = op_dest->val & op_src->val;
    cpu.eflags.cf=0;
    cpu.eflags.of=0;
    if(!result) {
        cpu.eflags.zf = 1;
        cpu.eflags.sf=0;
        cpu.eflags.pf=1;
    }
    else {
        cpu.eflags.zf=0;
        DATA_TYPE num = result;
        int count = 0;
        while(num){
        {
            num=num&(num-1);
            ++count;
        }
        }
        if(count%2) cpu.eflags.pf = 1;
        else cpu.eflags.pf=0;
       /* result>>=(8*DATA_BYTE-1);
        result&=1;
        if(result) cpu.eflags.sf = 1;
        else cpu.eflags.sf = 0;*/
        cpu.eflags.sf= MSB(result);
        }
}

make_instr_helper(rm2r);

#include "cpu/exec/template-end.h"


