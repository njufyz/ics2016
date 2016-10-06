#include "cpu/exec/template-start.h"

#define instr push

static void do_execute()
{
    int l;
    if(DATA_BYTE==2)
    {   
        l=2;
        cpu.gpr[4]._32-=DATA_BYTE;
    }
    else
    {
        l=4;
        cpu.gpr[4]._32-=4;
    }
   // MEM_W(cpu.gpr[4]._32,op_src->val);
    swaddr_write(cpu.gpr[4]._32,l,op_src->val);
    print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)
#include "cpu/exec/template-end.h"
