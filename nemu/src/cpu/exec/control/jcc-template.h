#include "cpu/exec/template-start.h"


/*------------------------*/
#define instr je

static void do_execute()
{
    if(cpu.eflags.zf==1)
    {
        cpu.eip+=op_src->val;
    }
#if DATA_BYTE == 2 
    cpu.eip&=0xffff;
#endif
}

make_instr_helper(si)
#undef instr



/*------------------------*/

#define instr jbe

static void do_execute(){
    if(cpu.eflags.cf==0 || cpu.eflags.zf==0)
        cpu.eip+=op_src->val;
#if DATA_BYTE == 2
    cpu.eip&=0xffff;
#endif
}

make_instr_helper(si)

#undef instr

/*----------------------*/
